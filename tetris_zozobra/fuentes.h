#ifndef FUENTES_H_INCLUDED
#define FUENTES_H_INCLUDED
#include "GBT/gbt.h"
#define ALTO_LETRA 12
#define ALTO_LETRA2 18

typedef struct
{
    const int* matriz;
    int ancho;
} t_letra;


void dibujarTextoLetra1(uint16_t x, uint16_t y, const char *texto, uint8_t colorIndice, uint8_t espaciado);
void dibujarTextoLetra2(uint16_t x, uint16_t y, const char *texto, uint8_t colorIndice, uint8_t espaciado);
void dibujarLetra(uint16_t x_inicio, uint16_t y_inicio, const int *matrizLetra, uint8_t anchoLetra, uint8_t colorIndice);
void dibujarLetra2(uint16_t x_inicio, uint16_t y_inicio, const int *matrizLetra, uint8_t anchoLetra, uint8_t colorIndice);
void inputTeclado(uint16_t x, uint16_t y, char* usuario, int max_chars, uint8_t colorTexto);
t_letra obtenerLetra2(char c);
t_letra obtenerLetra(char c);
void dibujarTeclado(uint16_t x, uint16_t y, char* usuario, int max_chars, uint8_t colorTexto);

#endif // FUENTES_H_INCLUDED
