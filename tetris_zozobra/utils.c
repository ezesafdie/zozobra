#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void leerArgumentos(int argc, char* argv[], int* anchoVentana, int* altoVentana, int* escala)
{
    int contArgs = 0;
    for (int i = 1; i < argc; i++)
    {
        int codError = NO_MATCHEA;

        if(strncmp(argv[i], "--resolucion=", 13) == 0)
        {
            char* val = argv[i] + 13;
            if(strcmp(val, "cga") == 0)
            {
                *anchoVentana = ANCHO_CGA;
                *altoVentana = ALTO_CGA;
                codError = TODO_OK;
                contArgs++;
                continue;
            }
            else if(strcmp(val, "vga") == 0)
            {
                codError = TODO_OK;
                contArgs++;
                continue;
            }
            codError = VALOR_INVALIDO;
        }

        if(strncmp(argv[i], "--escala=", 9) == 0)
        {
            int val = atoi(argv[i]+9);
            if (val > 0 && val <= 10)
            {
                *escala = val;
                codError = TODO_OK;
                contArgs++;
                continue;
            }
            else
            {
                codError = VALOR_FUERA_DEL_RANGO;
            }
        }

        if(strcmp(argv[i], "--ayuda") == 0)
        {
            puts("----Tetris Zozobra---- \n"
                 "Configuraciones base para la inicialización del juego. "
                 "Las posibles configuraciones son: \n"
                 "--resolucion : Acepta únicamente los valores 'vga' para una resolucion de 640x480 o 'cga' para una resolución de 320x200 \n"
                 "--escala : Factor de escala de ventana. Acepta valores del 1 al 10 \n"
                );
            contArgs++;
            continue;
        }

        switch(codError)
        {
        case VALOR_INVALIDO:
            printf("El valor para el comando \" %s \" es invalido. Ejecute el comando '--ayuda' para mas detalles \n", argv[i]);
            contArgs++;
            break;
        case VALOR_FUERA_DEL_RANGO:
            printf("El valor para el comando \" %s \" esta fuera del rango. Ejecute el comando '--ayuda' para mas detalles \n", argv[i]);
            contArgs++;
            break;
        default:
            printf("Comando \" %s \" no existe. Ejecute el comando '--ayuda' para visualizar las funcionalidades diponibles \n", argv[i]);
        }


    }
    if(contArgs == 0)
    {
        puts("No se detectaron argumentos en la ejecucion. Corriendo por defecto a resolucion VGA y escala x1 \n");
    }
}
