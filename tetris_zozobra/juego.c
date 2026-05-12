#include <stdio.h>
#include "juego.h"
#include "tablero.h"
#include "codigos_retorno.h"
#include "render.h"

#define ANCHO_MODO_CLASICO 10
#define ALTO_MODO_CLASICO 24

// Variables estaticas privadas de este modulo
static Tablero miTablero;
static int tableroInicializado = 0;

EstadoJuego procesarJuego(eGBT_Tecla tecla, EstadoJuego estadoActual, EstadoJuego* estadoPrevioPausa)
{
    // Inicializacion del tablero (se ejecuta solo la primera vez)
    if (!tableroInicializado)
    {
        CodigoRetorno resultadoCreacion = crearTablero(&miTablero, ANCHO_MODO_CLASICO, ALTO_MODO_CLASICO);
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
        }
    }

    // 2. Procesamiento de las teclas
    switch (tecla)
    {
    case GBTK_p:
        *estadoPrevioPausa = estadoActual;
        printf("Cambiando a estado: PAUSA\n");
        return ESTADO_PAUSA;
    case GBTK_f:
        printf("Cambiando a estado: FIN_JUEGO\n");

        if (tableroInicializado)
        {
            destruirTablero(&miTablero);
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
        dibujarGrillaTablero(&miTablero, anchoVentana, altoVentana);
    }
}

void dibujarJuegoDX(int anchoVentana, int altoVentana)
{
    gbt_borrar_backbuffer(3); // Cian
}
