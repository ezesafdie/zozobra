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
#include "estados.h"
#include "menu_principal.h"
#include "juego.h"
#include "pausa.h"
#include "fin_juego.h"
#include "ingreso_nombre.h"
#include "codigos_retorno.h"
#define ANCHO_VGA 640
#define ALTO_VGA 480
#define ANCHO_CGA 320
#define ALTO_CGA 200

int main(int argc, char* argv[])
{
    printf("Hello world!\n");

    int anchoVentana = ANCHO_VGA;
    int altoVentana = ALTO_VGA;
    int escala = 1;

    leerArgumentos(argc, argv, &anchoVentana, &altoVentana, &escala);

    if (gbt_iniciar() != 0)
    {
        fprintf(stderr, "Error al iniciar GBT: %s\n", gbt_obtener_log());
        return ERROR_INICIALIZACION_GBT;
    }

    char nombreVentana[128];
    sprintf(nombreVentana, "Ventana %dx%d | Escala: x%d", anchoVentana, altoVentana, escala);
    printf("Ventana %dx%d | Escala: x%d \n", anchoVentana, altoVentana, escala);

    if (gbt_crear_ventana(nombreVentana, anchoVentana, altoVentana, escala) != 0)
    {
        fprintf(stderr, "Error al iniciar el modulo de graficos de GBT: %s\n", gbt_obtener_log());
        return ERROR_GRAFICOS_GBT;
    }

    EstadoJuego estadoActual = ESTADO_MENU_PRINCIPAL;
    int estadoAnterior = -1; // para no repetir el print del menú
    EstadoJuego estadoPrevioPausa = ESTADO_JUEGO_CLASICO;
    int corriendo = 1;

    while (corriendo)
    {
        if (estadoActual != estadoAnterior)
        {
            printf("\n========================================\n");
            switch (estadoActual)
            {
            case ESTADO_MENU_PRINCIPAL:
                printf("ESTADO: MENU PRINCIPAL\n");
                printf(" - Presiona [1] para Juego Clasico\n");
                printf(" - Presiona [2] para Juego DX\n");
                break;
            case ESTADO_JUEGO_CLASICO:
                printf("ESTADO: JUEGO CLASICO\n");
                printf(" - Presiona [P] para Pausar\n");
                printf(" - Presiona [F] para Fin de Juego\n");
                break;
            case ESTADO_JUEGO_DX:
                printf("ESTADO: JUEGO DX\n");
                printf(" - Presiona [P] para Pausar\n");
                printf(" - Presiona [F] para Fin de Juego\n");
                break;
            case ESTADO_PAUSA:
                printf("ESTADO: PAUSA\n");
                printf(" - Presiona [P] para Reanudar su juego actual \n");
                printf(" - Presiona [M] para volver al Menu Principal\n");
                break;
            case ESTADO_FIN_JUEGO:
                printf("ESTADO: FIN DEL JUEGO\n");
                printf(" - Presiona [N] para Ingresar tu Nombre\n");
                printf(" - Presiona [M] para volver al Menu Principal\n");
                break;
            case ESTADO_INGRESO_NOMBRE:
                printf("ESTADO: INGRESO DE NOMBRE\n");
                printf(" - Ingresa tu nombre y presiona [ENTER] para confirmar\n");
                printf(" - Presiona [M] para volver al Menu Principal\n");
                break;
            case ESTADO_SALIR:
                printf("ESTADO: SALIENDO...\n");
                break;
            }
            printf(" - Presiona [ESC] en cualquier momento para salir\n");
            printf("========================================\n");

            estadoAnterior = estadoActual;
        }

        gbt_procesar_entrada();
        eGBT_Tecla tecla = gbt_obtener_tecla_presionada();

        if (tecla == GBTK_ESCAPE)
        {
            corriendo = 0;
            printf("Saliendo...\n");
        }

        switch (estadoActual)
        {
        case ESTADO_MENU_PRINCIPAL:
            estadoActual = procesarMenuPrincipal(tecla, estadoActual);
            break;

        case ESTADO_JUEGO_CLASICO:
        case ESTADO_JUEGO_DX:
            estadoActual = procesarJuego(tecla, estadoActual, &estadoPrevioPausa);
            break;

        case ESTADO_PAUSA:
            estadoActual = procesarPausa(tecla, estadoActual, estadoPrevioPausa);
            break;

        case ESTADO_FIN_JUEGO:
            estadoActual = procesarFinJuego(tecla, estadoActual);
            break;

        case ESTADO_INGRESO_NOMBRE:
            estadoActual = procesarIngresoNombre(tecla, estadoActual);
            break;

        case ESTADO_SALIR:
            corriendo = 0;
            printf("Saliendo del ejemplo\n");
            break;
        }

        switch (estadoActual)
        {
        case ESTADO_MENU_PRINCIPAL:
            dibujarMenuPrincipal();
            break;
        case ESTADO_JUEGO_CLASICO:
            dibujarJuegoClasico(anchoVentana, altoVentana);
            break;
        case ESTADO_JUEGO_DX:
            dibujarJuegoDX(anchoVentana, altoVentana);
            break;
        case ESTADO_PAUSA:
            dibujarPausa();
            break;
        case ESTADO_FIN_JUEGO:
            dibujarFinJuego();
            break;
        case ESTADO_INGRESO_NOMBRE:
            dibujarIngresoNombre();
            break;
        default:
            gbt_borrar_backbuffer(0);
            break;
        }

        gbt_volcar_backbuffer();
        gbt_esperar(16);
    }

    gbt_destruir_ventana();
    gbt_cerrar();


    return 0;
}
