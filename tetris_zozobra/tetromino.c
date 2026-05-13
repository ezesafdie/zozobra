#include "tetromino.h"

void generarPieza(Pieza* pieza, int tipoPieza, int anchoTablero)
{

    //Limpiamos (seteamos a 0) toda la pieza por seguridad
    for(int i = 0; i < VALOR_MAXIMO_PIEZA; i++)
    {
        for(int j = 0; j < VALOR_MAXIMO_PIEZA; j++)
        {
            pieza->matrizDeForma[i][j] = 0;
        }
    }

    switch(tipoPieza)
    {
    case 1: // Pieza 'O' (Cuadrado - Amarillo)
        pieza->tam = 2;
        pieza->color = 14;
        pieza->matrizDeForma[0][0] = 1;
        pieza->matrizDeForma[0][1] = 1;
        pieza->matrizDeForma[1][0] = 1;
        pieza->matrizDeForma[1][1] = 1;
        break;

    case 2: // Pieza 'I' (Palo - Cian)
        pieza->tam = 4;
        pieza->color = 3;
        // En el estandar, el palo nace acostado en la segunda fila
        pieza->matrizDeForma[1][0] = 1;
        pieza->matrizDeForma[1][1] = 1;
        pieza->matrizDeForma[1][2] = 1;
        pieza->matrizDeForma[1][3] = 1;
        break;

    case 3: // Pieza 'T' (Fucsia)
        pieza->tam = 3;
        pieza->color = 5;
        pieza->matrizDeForma[0][1] = 1;
        pieza->matrizDeForma[1][0] = 1;
        pieza->matrizDeForma[1][1] = 1;
        pieza->matrizDeForma[1][2] = 1;
        break;

    case 4: // Pieza 'L' (Naranja / Marron claro)
        pieza->tam = 3;
        pieza->color = 6;
        pieza->matrizDeForma[0][2] = 1;
        pieza->matrizDeForma[1][0] = 1;
        pieza->matrizDeForma[1][1] = 1;
        pieza->matrizDeForma[1][2] = 1;
        break;

    case 5: // Pieza 'J' (Azul)
        pieza->tam = 3;
        pieza->color = 1;
        pieza->matrizDeForma[0][0] = 1;
        pieza->matrizDeForma[1][0] = 1;
        pieza->matrizDeForma[1][1] = 1;
        pieza->matrizDeForma[1][2] = 1;
        break;

    case 6: // Pieza 'S' (Verde)
        pieza->tam = 3;
        pieza->color = 2;
        pieza->matrizDeForma[0][1] = 1;
        pieza->matrizDeForma[0][2] = 1;
        pieza->matrizDeForma[1][0] = 1;
        pieza->matrizDeForma[1][1] = 1;
        break;

    case 7: // Pieza 'Z' (Rojo)
        pieza->tam = 3;
        pieza->color = 4;
        pieza->matrizDeForma[0][0] = 1;
        pieza->matrizDeForma[0][1] = 1;
        pieza->matrizDeForma[1][1] = 1;
        pieza->matrizDeForma[1][2] = 1;
        break;
    }

    pieza->x = (anchoTablero - pieza->tam) / 2;
    pieza->y = 0;

}
