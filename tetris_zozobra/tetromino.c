#include "tetromino.h"

// Definimos un array constante con todas las piezas posibles, con la estructura ya creada:
// { x, y, tam, color, spawnY, { matriz } }
static const Pieza CATALOGO_PIEZAS[7] = {
    // 1: Cuadrado 'O' (amarillo)
    {0, 0, 2, 14, 3,{
        {1, 1},
        {1, 1}
    }},

    // 2: Palo 'I' (cian)
    {0, 0, 4, 3, 2,{
        {0, 0, 0, 0},
        {1, 1, 1, 1}
    }},

     // 3: Letra 'T' (violeta)
     {0, 0, 3, 5, 3,{
         {0, 1, 0},
         {1, 1, 1}
    }},

    // 4: Letra 'L' (naranja)
    {0, 0, 3, 6, 3,{
        {0, 0, 1},
        {1, 1, 1}
    }},

    // 5: Letra 'J' (azul)
    {0, 0, 3, 1, 3,{
        {1, 0, 0},
        {1, 1, 1}
    }},

    // 6: Letra 'S' (verde)
    {0, 0, 3, 2, 3,{
        {0, 1, 1},
        {1, 1, 0}
    }},

    // 7: Letra 'Z' (rojo)
    {0, 0, 3, 4, 3,{
        {1, 1, 0},
        {0, 1, 1}
    }}
};

void generarPieza(Pieza* pieza, int tipoPieza, int anchoTablero)
{
    *pieza = CATALOGO_PIEZAS[tipoPieza - 1];

    pieza->x = (anchoTablero - pieza->tam) / 2;
    pieza->y = pieza->spawnY;

}

void rotarPieza(Pieza* pieza)
{
    Pieza copia = *pieza;

    //Limpiemos la matriz a 0
    for(int i = 0; i < VALOR_MAXIMO_PIEZA; i++)
    {
        for(int j = 0; j <VALOR_MAXIMO_PIEZA; j++)
        {
            pieza->matrizDeForma[i][j] = 0;
        }
    }

    //Rotacion propiamente dicha
    for(int i = 0; i < pieza->tam; i++)
    {
        for(int j = 0; j < pieza->tam; j++)
        {
            pieza->matrizDeForma[j][pieza->tam - 1 - i] = copia.matrizDeForma[i][j];
        }
    }
}
