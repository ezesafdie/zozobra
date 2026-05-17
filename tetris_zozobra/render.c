#include "render.h"
#include "GBT/gbt_graficos.h"

//Colores VGA estandar
#define COLOR_NEGRO       0
#define COLOR_GRIS_OSCURO 8
#define COLOR_BLANCO      15

#define BLOQUES_TOTALES_CON_MARGEN 22

// TODO: Fue lo primero que se me ocurrio para no estar recalculando constamente. No me convence del todo tener el if asi igualmente
// TODO: Capaz podemos repensarlo en el futuro
static int cache_anchoVentana = -1; // -1 fuerza a que se calcule la primera vez
static int cache_altoVentana = -1;
static int cache_tamBloque = 0;
static int cache_margenX = 0;
static int cache_margenY = 0;

static void actualizarCacheSiEsNecesario(Tablero* t, int anchoVentana, int altoVentana)
{
    if (cache_anchoVentana != anchoVentana || cache_altoVentana != altoVentana)
    {
        cache_anchoVentana = anchoVentana;
        cache_altoVentana = altoVentana;

        cache_tamBloque = altoVentana / BLOQUES_TOTALES_CON_MARGEN;
        cache_margenX = (anchoVentana - (t->ancho * cache_tamBloque)) / 2;
        cache_margenY = (altoVentana - ((t->alto - 4) * cache_tamBloque)) / 2;
    }
}

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

    actualizarCacheSiEsNecesario(t, anchoVentana, altoVentana);

    // Empezamos a dibujar desde la fila 4 del tablero, ya que las primeras 4 filas son de spawn y no se muestran en pantalla.
    for (int i = 4; i < t->alto; i++)
    {
        for (int j = 0; j < t->ancho; j++)
        {
            int coordX = cache_margenX + (j * cache_tamBloque);
            int coordY = cache_margenY + ((i - 4) * cache_tamBloque);

            if (t->matriz[i][j] == 0)
            {
                dibujarBloque(coordX, coordY, cache_tamBloque, COLOR_NEGRO, COLOR_GRIS_OSCURO);
            }
            else
            {
                dibujarBloque(coordX, coordY, cache_tamBloque, t->matriz[i][j], COLOR_BLANCO);
            }
        }
    }
}

void dibujarPiezaActiva(Pieza* pieza, Tablero* tablero, int anchoVentana, int altoVentana)
{
    if(pieza == NULL || tablero == NULL) return;

    actualizarCacheSiEsNecesario(tablero, anchoVentana, altoVentana);

    for(int i = 0; i < pieza->tam; i++)
    {
        for(int j = 0; j <pieza->tam; j++)
        {
            if(pieza->matrizDeForma[i][j] == 1)
            {
                int filaTablero = pieza->y + i;
                int columnaTablero = pieza->x + j;

                if(filaTablero >= 4)
                {
                    int coordenadaX = cache_margenX + (columnaTablero * cache_tamBloque);
                    int coordenadaY = cache_margenY + ((filaTablero - 4) * cache_tamBloque);

                    dibujarBloque(coordenadaX, coordenadaY, cache_tamBloque, pieza->color, COLOR_BLANCO);
                }
            }
        }
    }
}
