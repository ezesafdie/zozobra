#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "estados.h"
#include "tetromino.h"
#include "tablero.h"

//TODO: Definir bien estas constantes
#define PIEZAS_POR_NIVEL 2
#define VELOCIDAD_INICIAL 1
#define FACTOR_INCREMENTO_VELOCIDAD 0.97 //Un 3% mas rapido
#define FACTOR_VELOCIDAD_FIJACION 0.5 // 50% de la velocidad actual
#define MAX_RESETEOS_DE_FIJACION 15

#define COLOR_GRIS 8

EstadoJuego procesarJuego(eGBT_Tecla tecla, EstadoJuego estadoActual, EstadoJuego* estadoPrevioPausa);
void dibujarJuegoClasico(int anchoVentana, int altoVentana);
void dibujarJuegoDX(int anchoVentana, int altoVentana);

#endif // JUEGO_H_INCLUDED
