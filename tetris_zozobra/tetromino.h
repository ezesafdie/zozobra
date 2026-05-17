#ifndef TETROMINO_H_INCLUDED
#define TETROMINO_H_INCLUDED

#define VALOR_MAXIMO_PIEZA 5

typedef struct
{
    int x;
    int y;
    int tam; // 2, 3, 4, 5. Usado para saber hasta que parte de la matriz usar
    int color;
    int matrizDeForma[VALOR_MAXIMO_PIEZA][VALOR_MAXIMO_PIEZA]; //Matriz del valor maximo que puede tener
} Pieza;

void generarPieza(Pieza* pieza, int tipoPieza, int anchoTablero);
void rotarPieza(Pieza* pieza);

#endif // TETROMINO_H_INCLUDED
