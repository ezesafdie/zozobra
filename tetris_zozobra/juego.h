#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "estados.h"
#include "tetromino.h"
#include "tablero.h"

EstadoJuego procesarJuego(eGBT_Tecla tecla, EstadoJuego estadoActual, EstadoJuego* estadoPrevioPausa);
void dibujarJuegoClasico(int anchoVentana, int altoVentana);
void dibujarJuegoDX(int anchoVentana, int altoVentana);

#endif // JUEGO_H_INCLUDED
