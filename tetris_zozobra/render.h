#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED
#include "tablero.h"
#include "tetromino.h"

void dibujarGrillaTablero(Tablero* t, int anchoVentana, int altoVentana);
void dibujarPiezaActiva(Pieza* p, Tablero* t, int anchoVentana, int altoVentana);

#endif // RENDER_H_INCLUDED
