#include <stdio.h>
#include "menu_principal.h"

EstadoJuego procesarMenuPrincipal(eGBT_Tecla tecla, EstadoJuego estadoActual)
{
    switch (tecla)
    {
    case GBTK_1:
        printf("Cambiando a estado: JUEGO_CLASICO\n");
        return ESTADO_JUEGO_CLASICO;
    case GBTK_2:
        printf("Cambiando a estado: JUEGO_DX\n");
        return ESTADO_JUEGO_DX;
    default:
        break;
    }
    return estadoActual;
}

void dibujarMenuPrincipal(void)
{
    gbt_borrar_backbuffer(1); // Azul
}