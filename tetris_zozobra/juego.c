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
static int tableroInicializado = 0;
static tGBT_Temporizador* temporizadorGravedad = NULL;

//Funciones privadas de este modulo
static int esMovimientoValido(Pieza* pieza, Tablero* tablero, int offsetX, int offsetY);
static int fijarPiezaYGenerarNueva();

EstadoJuego procesarJuego(eGBT_Tecla tecla, EstadoJuego estadoActual, EstadoJuego* estadoPrevioPausa)
{
    // Inicializacion del tablero (se ejecuta solo la primera vez)
    if (!tableroInicializado)
    {
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

            //TODO: Esto hay que definirlo, lo dejo creado en 0.2 segundos para que sea facil de probar.
            //Me parece que es muy rapido. Cuando tengamos lo de mantener apretado lo podemos subir.
            temporizadorGravedad = gbt_temporizador_crear(0.2);

            //TODO: Cambiar esta generacion por el random sin sesgos.
            int tipoAleatorio = (rand() % 7) + 1;
            generarPieza(&piezaActual, tipoAleatorio, tablero.ancho);
        }
    }

    //Fisica de gravedad
    if(tableroInicializado && gbt_temporizador_consumir(temporizadorGravedad))
    {
        if(esMovimientoValido(&piezaActual, &tablero, 0, 1))
        {
            piezaActual.y++;
        }
        else
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

    // Procesamiento de las teclas
    switch (tecla)
    {
    case GBTK_IZQUIERDA:
        if(esMovimientoValido(&piezaActual, &tablero, -1, 0))
        {
            piezaActual.x--;
        }
        break;
    case GBTK_DERECHA:
        if(esMovimientoValido(&piezaActual, &tablero, 1, 0))
        {
            piezaActual.x++;
        }
        break;
    case GBTK_ABAJO:
        if(esMovimientoValido(&piezaActual, &tablero, 0, 1))
        {
            piezaActual.y++;
        }
        else if(!fijarPiezaYGenerarNueva())

        {
            printf("GAME OVER");
            destruirTablero(&tablero);
            gbt_temporizador_destruir(temporizadorGravedad);
            tableroInicializado = 0;
            return ESTADO_FIN_JUEGO;
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
    return estadoActual;
}

void dibujarJuegoClasico(int anchoVentana, int altoVentana)
{
    gbt_borrar_backbuffer(0); // Negro

    if (tableroInicializado)
    {
        dibujarGrillaTablero(&tablero, anchoVentana, altoVentana);
        dibujarPiezaActiva(&piezaActual, &tablero, anchoVentana, altoVentana);
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

                if(filaTablero >= 0 && filaTablero)
                {
                    tablero.matriz[filaTablero][columnaTablero] = piezaActual.color;
                }
            }
        }
    }

    //2. Validamos si se completo alguna fila.
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
    }

    //3. Generamos la siguiente pieza y validamos que no sea game over
    //TODO: Esto tiene que tener el generador que haga Emi.
    int tipoPiezaAleatoria = (rand() % 7) + 1;
    generarPieza(&piezaActual, tipoPiezaAleatoria, tablero.ancho);

    if(!esMovimientoValido(&piezaActual, &tablero, 0, 0))
    {
        return 0;
    }

    return 1;
}
