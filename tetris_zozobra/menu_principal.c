#include <stdio.h>
#include "menu_principal.h"
#include "fuentes.h"

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

    char user[10]={0};
    dibujarTextoLetra1(200, 100, "abcdefghijklmnopqrstuvwxyz", 5,2);
    dibujarTextoLetra2(200, 200, "Tetris zozobra", 5,5);
    dibujarTeclado(200, 400, user, 10, 5);
    dibujarTextoLetra2(200, 250, "Tetris zozobra", 5,5);
    printf("%s",user);
}
