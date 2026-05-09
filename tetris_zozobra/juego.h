#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "estados.h"

EstadoJuego procesarJuego(eGBT_Tecla tecla, EstadoJuego estadoActual, EstadoJuego* estadoPrevioPausa);
void dibujarJuegoClasico(void);
void dibujarJuegoDX(void);

#endif // JUEGO_H_INCLUDED
