#include "tablero.h"

#include <stdlib.h>
#include <stdbool.h>

CodigoRetorno crearTablero(Tablero* t, int ancho, int alto)
{
    t->ancho = ancho;
    t->alto = alto;

    //Reservamos memoria para el array de punteros a filas
    t->matriz = (int**)malloc(alto * sizeof(int*));
    if (t->matriz == NULL)
    {
        return ERROR_SIN_MEMORIA;
    }

    //Reservamos memoria para cada fila e inicializamos en 0
    for (int i = 0; i < alto; i++)
    {
        t->matriz[i] = (int*)malloc(ancho * sizeof(int));

        if (t->matriz[i] != NULL)
        {
            for (int j = 0; j < ancho; j++)
            {
                t->matriz[i][j] = 0;
            }
        }
        else
        {
            // En el caso de que falle una fila, tenemos que liberar las filas anteriores que si se crearon
            for (int k = 0; k < i; k++)
            {
                free(t->matriz[k]);
            }

            free(t->matriz);
            t->matriz = NULL;

            return ERROR_SIN_MEMORIA;
        }
    }

    return TODO_OK;
}

int detectarFilasCompletas(Tablero* t, int filaInicio, int filaFin, int filasLlenas[4])
{
    int cantidad = 0;

    if (filaInicio < 0) filaInicio = 0;
    if (filaFin >= t->alto) filaFin = t->alto - 1;

    for (int i = filaInicio; i <= filaFin; i++)
    {
        bool filaLlena = true;

        for (int j = 0; j < t->ancho; j++)
        {
            if (t->matriz[i][j] == 0)
            {
                filaLlena = false;
                break;
            }
        }

        if (filaLlena)
        {
            filasLlenas[cantidad] = i;
            cantidad++;
        }
    }

    return cantidad;
}

void ejecutarBorradoFilas(Tablero* t, int filasLlenas[4], int cantidad)
{
    for (int f = 0; f < cantidad; f++)
    {
        int filaObjetivo = filasLlenas[f];

        //Guardamos el puntero de la fila a borrar
        int* filaReciclada = t->matriz[filaObjetivo];

         //Bajamos los punteros de las filas que estan por encima de la fila objetivo
         for (int k = filaObjetivo; k > 0; k--)
        {
            t->matriz[k] = t->matriz[k - 1];
        }

        //Ponemos la fila "borrada" en la fila 0
        t->matriz[0] = filaReciclada;
        for (int j = 0; j < t->ancho; j++)
        {
            t->matriz[0][j] = 0;
        }
    }
}

void destruirTablero(Tablero* t)
{
    if (t != NULL)
    {
        for (int i = 0; i < t->alto; i++)
        {
            if (t->matriz[i] != NULL)
            {
                free(t->matriz[i]);
            }
        }
        free(t->matriz);
        t->matriz = NULL;
    }
}

void vaciarTablero(Tablero* t)
{
    if (t != NULL && t->matriz != NULL)
    {
        for (int i = 0; i < t->alto; i++)
        {
            for (int j = 0; j < t->ancho; j++)
            {
                t->matriz[i][j] = 0;
            }
        }
    }
}
