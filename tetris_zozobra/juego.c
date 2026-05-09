#include <stdio.h>
#include "juego.h"

EstadoJuego procesarJuego(eGBT_Tecla tecla, EstadoJuego estadoActual, EstadoJuego* estadoPrevioPausa)
{
    switch (tecla)
    {
    case GBTK_p:
        *estadoPrevioPausa = estadoActual;
        printf("Cambiando a estado: PAUSA\n");
        return ESTADO_PAUSA;
    case GBTK_f:
        printf("Cambiando a estado: FIN_JUEGO\n");
        return ESTADO_FIN_JUEGO;
    default:
        break;
    }
    return estadoActual;
}

void dibujarJuegoClasico(void)
{
    gbt_borrar_backbuffer(2); // Verde
}

void dibujarJuegoDX(void)
{
    gbt_borrar_backbuffer(3); // Cian
}
