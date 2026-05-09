#include <stdio.h>
#include "ingreso_nombre.h"

EstadoJuego procesarIngresoNombre(eGBT_Tecla tecla, EstadoJuego estadoActual)
{
    switch (tecla)
    {
    case GBTK_ENTER:
        printf("Cambiando a estado: MENU_PRINCIPAL (nombre ingresado)\n");
        return ESTADO_MENU_PRINCIPAL;
    case GBTK_m:
        printf("Cambiando a estado: MENU_PRINCIPAL\n");
        return ESTADO_MENU_PRINCIPAL;
    default:
        break;
    }
    return estadoActual;
}

void dibujarIngresoNombre(void)
{
    gbt_borrar_backbuffer(6); // Magenta
}
