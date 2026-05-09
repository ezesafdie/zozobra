#ifndef PAUSA_H_INCLUDED
#define PAUSA_H_INCLUDED

#include "estados.h"

EstadoJuego procesarPausa(eGBT_Tecla tecla, EstadoJuego estadoActual, EstadoJuego estadoPrevioPausa);
void dibujarPausa(void);

#endif // PAUSA_H_INCLUDED
