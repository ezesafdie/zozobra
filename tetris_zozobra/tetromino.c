#include "tetromino.h"

// Definimos un array constante con todas las piezas posibles, con la estructura ya creada:
// { x, y, tam, color, { matriz } }
static const Pieza CATALOGO_PIEZAS[7] = {
    // 1: Cuadrado 'O' (amarillo)
    {0, 0, 2, 14, {
        {1, 1},
        {1, 1}
    }},

    // 2: Palo 'I' (cian)
    {0, 0, 4, 3, {
        {0, 0, 0, 0},
        {1, 1, 1, 1}
    }},

     // 3: Letra 'T' (purpura)
     {0, 0, 3, 5, {
         {0, 1, 0},
         {1, 1, 1}
    }},

    // 4: Letra 'L' (naranja)
    {0, 0, 3, 6, {
        {0, 0, 1},
        {1, 1, 1}
    }},

    // 5: Letra 'J' (azul)
    {0, 0, 3, 1, {
        {1, 0, 0},
        {1, 1, 1}
    }},

    // 6: Letra 'S' (verde)
    {0, 0, 3, 2, {
        {0, 1, 1},
        {1, 1, 0}
    }},

    // 7: Letra 'Z' (rojo)
    {0, 0, 3, 4, {
        {1, 1, 0},
        {0, 1, 1}
    }}
};

void generarPieza(Pieza* pieza, int tipoPieza, int anchoTablero)
{
    *pieza = CATALOGO_PIEZAS[tipoPieza - 1];

    pieza->x = (anchoTablero - pieza->tam) / 2;
    pieza->y = 0;

}
