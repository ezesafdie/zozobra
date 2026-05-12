#include "render.h"
#include "GBT/gbt_graficos.h"

//Colores VGA estandar
#define COLOR_NEGRO       0
#define COLOR_GRIS_OSCURO 8
#define COLOR_BLANCO      15

static void dibujarBloque(int coordX, int coordY, int tam, uint8_t colorRelleno, uint8_t colorBorde)
{
    for (int i = 0; i < tam; i++)
    {
        for (int j = 0; j < tam; j++)
        {
            // Pintamos los bordes
            if (i == 0 || j == 0 || i == tam - 1 || j == tam - 1)
            {
                gbt_dibujar_pixel(coordX + j, coordY + i, colorBorde);
            }
            else
            {
                // Pintamos el relleno
                gbt_dibujar_pixel(coordX + j, coordY + i, colorRelleno);
            }
        }
    }
}

void dibujarGrillaTablero(Tablero* t, int anchoVentana, int altoVentana)
{
    if (t == NULL || t->matriz == NULL) return;

    // Dividimos el alto total de la ventana por 22 para dejar un margen arriba y abajo.
    int tamBloque = altoVentana / 22;

    // Centramos el tablero horizontal y verticalmente
    int margenX = (anchoVentana - (t->ancho * tamBloque)) / 2;
    int margenY = (altoVentana - ((t->alto - 4) * tamBloque)) / 2;

    // Empezamos a dibujar desde la fila 4 del tablero, ya que las primeras 4 filas son de spawn y no se muestran en pantalla.
    for (int i = 4; i < t->alto; i++)
    {
        for (int j = 0; j < t->ancho; j++)
        {
            int coordX = margenX + (j * tamBloque);
            int coordY = margenY + ((i - 4) * tamBloque);

            if (t->matriz[i][j] == 0)
            {
                dibujarBloque(coordX, coordY, tamBloque, COLOR_NEGRO, COLOR_GRIS_OSCURO);
            }
            else
            {
                dibujarBloque(coordX, coordY, tamBloque, t->matriz[i][j], COLOR_BLANCO);
            }
        }
    }
}
