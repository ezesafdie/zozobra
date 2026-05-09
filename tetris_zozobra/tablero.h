#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

typedef struct {
    int ancho;
    int alto;
    int **matriz;
} Tablero;


int crearTablero(Tablero* t, int ancho, int alto);
void destruirTablero(Tablero* t);
void vaciarTablero(Tablero* t);
int detectarFilasCompletas(Tablero* t, int filaInicio, int filaFin, int filasLlenas[4]);
void ejecutarBorradoFilas(Tablero* t, int filasLlenas[4], int cantidad);

#endif // TABLERO_H_INCLUDED
