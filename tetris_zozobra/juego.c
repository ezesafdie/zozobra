#include <stdio.h>
#include <time.h>
#include "juego.h"
#include "tablero.h"
#include "codigos_retorno.h"
#include "render.h"
#include "tetromino.h"
#include "GBT/gbt_temporizador.h"

#define ANCHO_MODO_CLASICO 10
#define ALTO_MODO_CLASICO 24

// Variables estaticas privadas de este modulo
static Tablero tablero;
static Pieza piezaActual;
static Pieza piezaSiguiente;
static int tableroInicializado = 0;

static tGBT_Temporizador* temporizadorGravedad = NULL;
static int piezasTotalesCaidas = 0;
static int nivelActual = 1;
static double velocidadActual = VELOCIDAD_INICIAL;
static int puntajeTotal = 0;

static tGBT_Temporizador* temporizadorFijacion = NULL;
static int enFaseDeFijacion = 0;
static int reseteosDeFijacion = 0;

//Funciones privadas de este modulo
static int esMovimientoValido(Pieza* pieza, Tablero* tablero, int offsetX, int offsetY);
static int fijarPiezaYGenerarNueva();
static void validarFilasCompletas();
static void calcularNivel();
static void calcularPuntaje(int lineasBorradas);
static void reiniciarEstadisticas();
static void gestionarFaseDeFijacion(int movimientoExitoso);
static void dibujarPiezaFantasma(int anchoVentana, int altoVentana);

EstadoJuego procesarJuego(eGBT_Tecla tecla, EstadoJuego estadoActual, EstadoJuego* estadoPrevioPausa)
{
    // Inicializacion del tablero (se ejecuta solo la primera vez)
    if (!tableroInicializado)
    {
        reiniciarEstadisticas();
        CodigoRetorno resultadoCreacion = crearTablero(&tablero, ANCHO_MODO_CLASICO, ALTO_MODO_CLASICO);
        if (resultadoCreacion != TODO_OK)
        {
            if (resultadoCreacion == ERROR_SIN_MEMORIA)
            {
                printf("Error critico: No se pudo reservar memoria para el tablero.\n");
            }
            else
            {
                printf("Error critico: Fallo al crear el tablero (codigo %d).\n", resultadoCreacion);
            }
            //TODO: Habria que retornar una estructura que tenga el estado del juego junto con un codigo de error.
        }
        else
        {
            tableroInicializado = 1;
            printf("Tablero inicializado correctamente en memoria.\n");
            srand(time(NULL));

            //TODO: Esto hay que definirlo, lo dejo creado en 0.3 segundos para que sea facil de probar.
            //Me parece que es muy rapido. Cuando tengamos lo de mantener apretado lo podemos subir.
            temporizadorGravedad = gbt_temporizador_crear(VELOCIDAD_INICIAL);

            //TODO: Cambiar esta generacion por el random sin sesgos.
            int tipoPiezaAleatorio = (rand() % 7) + 1;
            generarPieza(&piezaActual, tipoPiezaAleatorio, tablero.ancho);

            int tipoPiezaSiguiente = (rand() % 7) + 1;
            generarPieza(&piezaSiguiente, tipoPiezaSiguiente, tablero.ancho);
        }
    }

    int movimientoExitoso = 0;
    if(enFaseDeFijacion)
    {
        if(gbt_temporizador_consumir(temporizadorFijacion))
        {
            if(!fijarPiezaYGenerarNueva())
            {
                printf("GAME OVER");
                destruirTablero(&tablero);
                gbt_temporizador_destruir(temporizadorGravedad);
                tableroInicializado = 0;
                return ESTADO_FIN_JUEGO;
            }
        }
    }
    else if(tableroInicializado && gbt_temporizador_consumir(temporizadorGravedad))
    {
        if(esMovimientoValido(&piezaActual, &tablero, 0, 1))
        {
            piezaActual.y++;
        }
    }

    // Procesamiento de las teclas
    switch (tecla)
    {
    case GBTK_IZQUIERDA:
        if(esMovimientoValido(&piezaActual, &tablero, -1, 0))
        {
            piezaActual.x--;
            movimientoExitoso = 1;
        }
        break;
    case GBTK_DERECHA:
        if(esMovimientoValido(&piezaActual, &tablero, 1, 0))
        {
            piezaActual.x++;
            movimientoExitoso = 1;
        }
        break;
    case GBTK_ABAJO:
        if(esMovimientoValido(&piezaActual, &tablero, 0, 1))
        {
            piezaActual.y++;
            //TODO: El enunciado dice "A mayor velocidad, mayor los puntos obtenidos por el jugador."
            // Eso ya se contempla cuando borar una linea. Tambien se puede hacer para que el valor de bajarlo a mano
            //sea algo como puntajeTotal += nivel. Que piensan?
            puntajeTotal++;

            printf("Soft Drop! +1 pt | SCORE: %d\n", puntajeTotal);
        }
        break;
    case GBTK_ARRIBA:
        //Como la rotacion destruye la matriz de la pieza, hacemos la prueba de validez sobre una copia
    {
        Pieza piezaCopia = piezaActual;
        rotarPieza(&piezaCopia);

        if(esMovimientoValido(&piezaCopia, &tablero, 0, 0))
        {
            piezaActual = piezaCopia;
            movimientoExitoso = 1;
        }
    }
    break;
    case GBTK_p:
        *estadoPrevioPausa = estadoActual;
        printf("Cambiando a estado: PAUSA\n");
        return ESTADO_PAUSA;
    case GBTK_f:
        printf("Cambiando a estado: FIN_JUEGO\n");

        if (tableroInicializado)
        {
            destruirTablero(&tablero);
            tableroInicializado = 0;
            printf("Memoria del tablero liberada.\n");
        }

        return ESTADO_FIN_JUEGO;
    default:
        break;
    }

    if(tableroInicializado)
    {
        gestionarFaseDeFijacion(movimientoExitoso);
    }

    return estadoActual;
}

void dibujarJuegoClasico(int anchoVentana, int altoVentana)
{
    //gbt_borrar_backbuffer(18); // Negro
    //dibujarFondoTexturadoConLineas(anchoVentana, altoVentana);
    dibujarFondoTexturadoConCuadrados(anchoVentana, altoVentana);

    if (tableroInicializado)
    {
        dibujarGrillaTablero(&tablero, anchoVentana, altoVentana);

        dibujarPiezaFantasma(anchoVentana, altoVentana);
        dibujarPiezaActiva(&piezaActual, &tablero, anchoVentana, altoVentana);

        dibujarInterfazClasica(&piezaSiguiente, &tablero);
    }
}

void dibujarJuegoDX(int anchoVentana, int altoVentana)
{
    gbt_borrar_backbuffer(3); // Cian
}

/**
* El objetivo de esta funcion es validar si el siguiente movimiento de la pieza va a ser valido.
* Recibe la pieza, el tablero, y un offset para x e y.
* El offset representa la cantidad de distancia que recorre la pieza: \n
* - Si el jugador aprieta la flecha izquierda: Le pasamos offsetX = -1 y offsetY = 0.\n
* - Si aprieta la flecha derecha: Le pasamos offsetX = 1 y offsetY = 0. \n
* - Si aprieta la flecha abajo: Le pasamos offsetX = 0 y offsetY = 1. \n
* - Si rota la pieza: Le pasamos offsetX = 0 y offsetY = 0.
* Validaciones que se hacen: \n
* - 1: Si choca contra las paredes laterales, o el piso \n
* - 2: Si choca contra un bloque que ya esta puesto en el tablero.
**/
static int esMovimientoValido(Pieza* pieza, Tablero* tablero, int offsetX, int offsetY)
{
    for(int i = 0; i < pieza->tam; i++)
    {
        for(int j = 0; j < pieza->tam; j++)
        {
            if(pieza->matrizDeForma[i][j] != 0)
            {
                int nuevaFila = pieza->y + i + offsetY;
                int nuevaColumna = pieza->x + j + offsetX;

                if(nuevaColumna < 0 || nuevaColumna >= tablero->ancho || nuevaFila >= tablero->alto)
                {
                    return 0;
                }

                if(nuevaFila >= 0 && nuevaFila < tablero ->alto)
                {
                    if(tablero->matriz[nuevaFila][nuevaColumna] != 0)
                    {
                        return 0;
                    }
                }
            }
        }
    }

    return 1;
}

static int fijarPiezaYGenerarNueva()
{
    //1. Recorremos la pieza (solo el tam) y si es distinto de 0
    //lo seteamos en el tablero, con su color correspondiente.
    for(int i = 0; i < piezaActual.tam; i++)
    {
        for(int j = 0; j < piezaActual.tam; j++)
        {
            if(piezaActual.matrizDeForma[i][j] != 0)
            {
                int filaTablero = piezaActual.y + i;
                int columnaTablero = piezaActual.x + j;

                if(filaTablero >= 0 && filaTablero < tablero.alto)
                {
                    tablero.matriz[filaTablero][columnaTablero] = piezaActual.color;
                }
            }
        }
    }

    //2. Validamos si se completo alguna fila.
    validarFilasCompletas();

    //3. Calculamos el nivel y aumentamos la velocidad si corresponde
    calcularNivel();

    //4. Actualizamos la pieza actual con la siguiente, volvemos a generar la siguiente y validamos que no sea game over
    piezaActual = piezaSiguiente;
    //TODO: Esto tiene que tener el generador que haga Emi.
    int tipoPiezaAleatoria = (rand() % 7) + 1;
    generarPieza(&piezaSiguiente, tipoPiezaAleatoria, tablero.ancho);

    if(!esMovimientoValido(&piezaActual, &tablero, 0, 0))
    {
        return 0;
    }

    return 1;
}

static void validarFilasCompletas()
{
    int filaInicio = piezaActual.y;
    if(filaInicio < 0)
        filaInicio = 0;
    int filaFin = piezaActual.y + piezaActual.tam - 1;
    if(filaFin >= tablero.alto)
        filaFin = tablero.alto - 1;

    int filasLlenas[4];
    int cantidadABorrar = detectarFilasCompletas(&tablero, filaInicio, filaFin, filasLlenas);
    if(cantidadABorrar > 0)
    {
        ejecutarBorradoFilas(&tablero, filasLlenas, cantidadABorrar);
        calcularPuntaje(cantidadABorrar);
    }
}

static void calcularNivel()
{
    piezasTotalesCaidas++;
    int nuevoNivel = (piezasTotalesCaidas / PIEZAS_POR_NIVEL) + 1;

    if(nuevoNivel > nivelActual)
    {
        nivelActual = nuevoNivel;
        velocidadActual = velocidadActual * FACTOR_INCREMENTO_VELOCIDAD;

        //TODO: Poner alguna validacion para que no sea imposible jugar? capaz dejar el minimo en 0.05 o algo asi

        gbt_temporizador_destruir(temporizadorGravedad);
        temporizadorGravedad = gbt_temporizador_crear(velocidadActual);

        printf("NIVEL %d! Piezas: %d | Velocidad: %.2fs\n", nivelActual, piezasTotalesCaidas, velocidadActual);
    }
}

static void gestionarFaseDeFijacion(int movimientoExitoso)
{
    if(!esMovimientoValido(&piezaActual, &tablero, 0, 1))
    {
        if(!enFaseDeFijacion)
        {
            enFaseDeFijacion = 1;
            reseteosDeFijacion = 0;

            if(temporizadorFijacion)
            {
                gbt_temporizador_destruir(temporizadorFijacion);
            }

            temporizadorFijacion = gbt_temporizador_crear(velocidadActual * FACTOR_VELOCIDAD_FIJACION);
        }
        else if(movimientoExitoso && reseteosDeFijacion < MAX_RESETEOS_DE_FIJACION)
        {
            reseteosDeFijacion++;

            gbt_temporizador_destruir(temporizadorFijacion);
            temporizadorFijacion = gbt_temporizador_crear(velocidadActual * FACTOR_VELOCIDAD_FIJACION);
        }
    }
    else
    {
        enFaseDeFijacion = 0;
    }
}

static void dibujarPiezaFantasma(int anchoVentana, int altoVentana)
{
    Pieza piezaFantasma = piezaActual;
    while(esMovimientoValido(&piezaFantasma, &tablero, 0, 1))
    {
        piezaFantasma.y++;
    }
    piezaFantasma.color = COLOR_GRIS;

    dibujarPiezaActiva(&piezaFantasma, &tablero, anchoVentana, altoVentana);
}

static void calcularPuntaje(int lineasBorradas)
{
    if(lineasBorradas <= 0)
        return;

    int puntosBase = 0;
    switch (lineasBorradas)
    {
        case 1: puntosBase = 40; break;
        case 2: puntosBase = 100; break;
        case 3: puntosBase = 300; break;
        case 4: puntosBase = 1200; break;
    }

    puntajeTotal += puntosBase * nivelActual;
    printf("%d LINEAS! | +%d pts | SCORE: %d\n", lineasBorradas, puntosBase * nivelActual, puntajeTotal);
}

static void reiniciarEstadisticas()
{
    nivelActual = 1;
    velocidadActual = VELOCIDAD_INICIAL;
    piezasTotalesCaidas = 0;
    enFaseDeFijacion = 0;
    reseteosDeFijacion = 0;
    puntajeTotal = 0;

    if(temporizadorFijacion != NULL)
    {
        gbt_temporizador_destruir(temporizadorFijacion);
        temporizadorFijacion = NULL;
    }
}
