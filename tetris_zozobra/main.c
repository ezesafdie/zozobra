/**
    INTEGRANTES DEL EQUIPO
    -----------------
    Apellido: Bentancur, Nahuel
    DNI: 42998043
    Usuario: NahuelBent
    Entrega: Si
    -----------------
    Apellido: Safdie, Ezequiel
    DNI: 43016790
    Usuario: ezesafdie
    Entrega: Si
    -----------------
    Apellido: Scevola, Emiliano
    DNI: 42818488
    Usuario: emilianosce
    Entrega: Si
    -----------------
*/
#include <stdio.h>
#include <stdlib.h>
#include "GBT/gbt.h"
#define ANCHO_VENTANA 128
#define ALTO_VENTANA 128
#define ESCALA_VENTANA 5
#define CANT_COLORES 16
#define TAM_GRILLA 11

int main()
{
    printf("Hello world!\n");

    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    char nombreVentana[128];
    sprintf(nombreVentana, "Ventana %dx%d", ANCHO_VENTANA, ALTO_VENTANA);

    if (gbt_crear_ventana(nombreVentana, ANCHO_VENTANA, ALTO_VENTANA, ESCALA_VENTANA) != 0) {
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    int corriendo = 1;
     while (corriendo) {

        gbt_procesar_entrada();
        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();

        if (tecla == GBTK_ESCAPE) {

            corriendo = 0;
            printf("Saliendo del ejemplo\n");
        }
     }

    gbt_destruir_ventana();
    gbt_cerrar();


    return 0;
}
