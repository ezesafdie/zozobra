#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "codigos_retorno.h"


#define ANCHO_VGA 640
#define ALTO_VGA 480
#define ANCHO_CGA 320
#define ALTO_CGA 200

#define NO_MATCHEA 6
#define VALOR_INVALIDO 7
#define VALOR_FUERA_DEL_RANGO 8

void leerArgumentos(int argc, char* argv[], int* anchoVentana, int* altoVentana, int* escala);

#endif // UTILS_H_INCLUDED
