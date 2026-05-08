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
#include "utils.h"
#define ANCHO_VGA 640
#define ALTO_VGA 480
#define ANCHO_CGA 320
#define ALTO_CGA 200
#define ESCALA_VENTANA 5
#define CANT_COLORES 16
#define TAM_GRILLA 11

int main(int argc, char* argv[])
{
    printf("Hello world!\n");

    int anchoVentana = ANCHO_VGA;
    int altoVentana = ALTO_VGA;
    int escala = 1;

    leerArgumentos(argc, argv, &anchoVentana, &altoVentana, &escala);

    if (gbt_iniciar() != 0) {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return -1;
    }

    char nombreVentana[128];
    sprintf(nombreVentana, "Ventana %dx%d | Escala: x%d", anchoVentana, altoVentana, escala);
    printf("Ventana %dx%d | Escala: x%d \n", anchoVentana, altoVentana, escala);

    if (gbt_crear_ventana(nombreVentana, anchoVentana, altoVentana, escala) != 0) {
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
