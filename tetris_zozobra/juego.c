#include <stdio.h>
#include "juego.h"
#include "tablero.h"
#include "codigos_retorno.h"
#include "render.h"
#include "tetromino.h"

#define ANCHO_MODO_CLASICO 10
#define ALTO_MODO_CLASICO 24

// Variables estaticas privadas de este modulo
static Tablero tablero;
static Pieza piezaActual;
static int tableroInicializado = 0;

static int esMovimientoValido(Pieza* pieza, Tablero* tablero, int offsetX, int offsetY);

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

            //TODO: Mas alla de borrar el y=8. Al momento de que se tenga que generar una nueva pieza, cuando la primera ya llego al final, hay que hacer la logica de spawn con temporizadores
            int tipoAleatorio = (rand() % 7) + 1;
            generarPieza(&piezaActual, tipoAleatorio, tablero.ancho);
            piezaActual.y = 8;
        }
    }

    // 2. Procesamiento de las teclas
    /*
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
    */

    if (gbt_tecla_sostenida(GBTK_IZQUIERDA))
    {
        if(esMovimientoValido(&piezaActual, &tablero, -1, 0))
        {
            piezaActual.x--;
        }
    }

    if (gbt_tecla_sostenida(GBTK_DERECHA))
    {
        if(esMovimientoValido(&piezaActual, &tablero, 1, 0))
        {
            piezaActual.x++;
        }
    }

    if (gbt_tecla_sostenida(GBTK_ABAJO))
    {
        if(esMovimientoValido(&piezaActual, &tablero, 0, 1))
        {
            piezaActual.y++;
        }
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
