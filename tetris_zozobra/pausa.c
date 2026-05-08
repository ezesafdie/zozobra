#include <stdio.h>
#include "pausa.h"

EstadoJuego procesarPausa(eGBT_Tecla tecla, EstadoJuego estadoActual, EstadoJuego estadoPrevioPausa)
{
    switch (tecla)
    {
    case GBTK_p:
        printf("Reanudando juego...\n");
        return estadoPrevioPausa;
    case GBTK_m:
        printf("Cambiando a estado: MENU_PRINCIPAL\n");
        return ESTADO_MENU_PRINCIPAL;
    default:
        break;
    }
    return estadoActual;
}

void dibujarPausa(void)
{
    gbt_borrar_backbuffer(4); // Amarillo
}