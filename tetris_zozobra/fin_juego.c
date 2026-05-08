#include <stdio.h>
#include "fin_juego.h"

EstadoJuego procesarFinJuego(eGBT_Tecla tecla, EstadoJuego estadoActual)
{
    switch (tecla)
    {
    case GBTK_n:
        printf("Cambiando a estado: INGRESO_NOMBRE\n");
        return ESTADO_INGRESO_NOMBRE;
    case GBTK_m:
        printf("Cambiando a estado: MENU_PRINCIPAL\n");
        return ESTADO_MENU_PRINCIPAL;
    default:
        break;
    }
    return estadoActual;
}

void dibujarFinJuego(void)
{
    gbt_borrar_backbuffer(5); // Rojo
}