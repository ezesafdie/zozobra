#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED
#include "tablero.h"
#include "tetromino.h"

void dibujarGrillaTablero(Tablero* t, int anchoVentana, int altoVentana);
void dibujarPiezaActiva(Pieza* p, Tablero* t, int anchoVentana, int altoVentana);
void dibujarInterfazClasica(Pieza* piezaSiguiente, Tablero* tablero);

void dibujarFondoTexturadoConLineas(int anchoVentana, int altoVentana);
void dibujarFondoTexturadoConCuadrados(int anchoVentana, int altoVentana);

#endif // RENDER_H_INCLUDED
