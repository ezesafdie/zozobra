#include "render.h"
#include "GBT/gbt_graficos.h"

//Colores VGA estandar
#define COLOR_NEGRO       0
#define COLOR_GRIS_OSCURO 8
#define COLOR_BLANCO      15
#define COLOR_GRIS_CLARO  7

#define BLOQUES_TOTALES_CON_MARGEN 22

// TODO: Fue lo primero que se me ocurrio para no estar recalculando constamente. No me convence del todo tener el if asi igualmente
// TODO: Capaz podemos repensarlo en el futuro
static int cache_anchoVentana = -1; // -1 fuerza a que se calcule la primera vez
static int cache_altoVentana = -1;
static int cache_tamBloque = 0;
static int cache_margenX = 0;
static int cache_margenY = 0;
static int cache_anchoTablero = -1;
static int cache_altoTablero = -1;

//TODO: Esta funcion probablemente no tenga que ser static en el futuro. Se puede usar en muchas pantallas.
static void dibujarRecuadro(int x, int y, int ancho, int alto, uint8_t color);
static void dibujarPiezaSiguiente(Pieza* piezaSiguiente, int panelX, int panelAncho, int cajaSiguienteY, int cajaSiguienteAlto);

static void actualizarCacheSiEsNecesario(Tablero* t, int anchoVentana, int altoVentana)
{
    if (cache_anchoVentana != anchoVentana || cache_altoVentana != altoVentana ||
        cache_anchoTablero != t->ancho || cache_altoTablero != t->alto)
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
    /*
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
    */

    for(int i = 0; i < tam; i++)
    {
        for(int j = 0; j < tam; j++)
        {
            gbt_dibujar_pixel(coordX + j, coordY + i, colorRelleno);
        }
    }

    //Diferenciamos el tablero
    if(colorRelleno == COLOR_NEGRO)
    {
        for(int i = 0; i < tam; i++)
        {
            gbt_dibujar_pixel(coordX + i, coordY, colorBorde); // Arriba
            gbt_dibujar_pixel(coordX, coordY + i, colorBorde); // Izquierda
            gbt_dibujar_pixel(coordX + i, coordY + tam - 1, colorBorde); // Abajo
            gbt_dibujar_pixel(coordX + tam - 1, coordY + i, colorBorde); // Derecha
        }
    }
    else
    {
        for(int i = 0; i < tam - 1; i++)
        {
            gbt_dibujar_pixel(coordX + i, coordY, COLOR_BLANCO);
            gbt_dibujar_pixel(coordX, coordY + i, COLOR_BLANCO);
        }
        for(int i = 0; i < tam; i++)
        {
            gbt_dibujar_pixel(coordX + i, coordY + tam - 1, COLOR_NEGRO);
            gbt_dibujar_pixel(coordX + tam - 1, coordY + i, COLOR_NEGRO);
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

//TODO: Revisar que los valores no queden hardcodeados, tiene que ser variable tanto por tablero como por deluxe
void dibujarInterfazClasica(Pieza* piezaSiguiente, Tablero* tablero)
{
    if(piezaSiguiente == NULL || tablero == NULL)
        return;

    int anchoTableroEnPixels = tablero->ancho * cache_tamBloque;

    //Configs generales
    int margenPanel = 20;
    int panelAncho = cache_tamBloque * 6;
    int altoCajaTexto = cache_tamBloque * 3;
    int separacionY = 15;


    //PANEL DERECHO
    int panelDerechoX = cache_margenX + anchoTableroEnPixels + margenPanel;

    //Preview siguiente pieza
    int cajaSiguienteY = cache_margenY;
    int cajaSiguienteAlto = cache_tamBloque * 5;

    dibujarRecuadro(panelDerechoX, cajaSiguienteY, panelAncho, cajaSiguienteAlto, COLOR_GRIS_OSCURO);
    dibujarPiezaSiguiente(piezaSiguiente, panelDerechoX, panelAncho, cajaSiguienteY, cajaSiguienteAlto);

    //Nivel actual
    int cajaLevelY = cajaSiguienteY + cajaSiguienteAlto + separacionY;
    dibujarRecuadro(panelDerechoX, cajaLevelY, panelAncho, altoCajaTexto, COLOR_GRIS_OSCURO);

    //Lineas borradas
    int cajaLinesY = cajaLevelY + altoCajaTexto + separacionY;
    dibujarRecuadro(panelDerechoX, cajaLinesY, panelAncho, altoCajaTexto, COLOR_GRIS_OSCURO);

    //Piezas caidas
    int cajaCaidasY = cajaLinesY + altoCajaTexto + separacionY;
    dibujarRecuadro(panelDerechoX, cajaCaidasY, panelAncho, altoCajaTexto, COLOR_GRIS_OSCURO);

    // PANEL IZQUIERDO
    int panelIzquierdoX = cache_margenX - margenPanel - panelAncho;
    if(panelIzquierdoX >= 0)
    {
        int cajaScoreY = cache_margenY;
        dibujarRecuadro(panelIzquierdoX, cajaScoreY, panelAncho, altoCajaTexto, COLOR_GRIS_OSCURO);

        int cajaHistoricoY = cajaScoreY + altoCajaTexto + separacionY;
        int cajaHistoricoAlto = (cache_tamBloque * 8) + separacionY;
        dibujarRecuadro(panelIzquierdoX, cajaHistoricoY, panelAncho, cajaHistoricoAlto, COLOR_GRIS_OSCURO);
    }


}

//REVISAR
static void dibujarRecuadro(int x, int y, int ancho, int alto, uint8_t color)
{
    for(int i = 0; i < ancho; i++)
    {
        gbt_dibujar_pixel(x + i, y, COLOR_GRIS_CLARO);
    }
    for(int i = 0; i < alto; i++)
    {
        gbt_dibujar_pixel(x, y + i, COLOR_GRIS_CLARO);
    }

    for(int i = 0; i < ancho; i++)
    {
        gbt_dibujar_pixel(x + i, y + alto - 1, COLOR_GRIS_OSCURO);
    }
    for(int i = 0; i < alto; i++)
    {
        gbt_dibujar_pixel(x + ancho - 1, y + i, COLOR_GRIS_OSCURO);
    }
}

static void dibujarPiezaSiguiente(Pieza* piezaSiguiente, int panelX, int panelAncho, int cajaSiguienteY, int cajaSiguienteAlto)
{
    for(int i = 1; i < cajaSiguienteAlto - 1; i++)
    {
        for(int j = 1; j < panelAncho - 1; j++)
        {
            gbt_dibujar_pixel(panelX + j, cajaSiguienteY + i, COLOR_NEGRO);
        }
    }

    int minX = piezaSiguiente->tam;
    int maxX = -1;
    int minY = piezaSiguiente->tam;
    int maxY = -1;

    for(int i = 0; i < piezaSiguiente->tam; i++)
    {
        for(int j = 0; j < piezaSiguiente->tam; j++)
            {
                if(piezaSiguiente->matrizDeForma[i][j] != 0)
                {
                    if(j < minX) minX = j;
                    if(j > maxX) maxX = j;
                    if(i < minY) minY = i;
                    if(i > maxY) maxY = i;
                }
            }
    }

    int anchoVisualBloques = maxX - minX + 1;
    int altoVisualBloques = maxY - minY + 1;

    int anchoVisualPixels = anchoVisualBloques * cache_tamBloque;
    int altoVisualPixels = altoVisualBloques * cache_tamBloque;

    int startX = panelX + (panelAncho - anchoVisualPixels) / 2;
    int startY = cajaSiguienteY + (cajaSiguienteAlto - altoVisualPixels) / 2;

    for(int i = 0; i < piezaSiguiente->tam; i++)
    {
        for(int j = 0; j < piezaSiguiente->tam; j++)
            {
                if(piezaSiguiente->matrizDeForma[i][j] != 0)
                {
                    int bloqueX = startX + (j - minX) * cache_tamBloque;
                    int bloqueY = startY + (i - minY) * cache_tamBloque;

                    dibujarBloque(bloqueX, bloqueY, cache_tamBloque, piezaSiguiente->color, COLOR_BLANCO);
                }
            }
    }
}

void dibujarFondoTexturadoConLineas(int anchoVentana, int altoVentana)
{
    uint8_t colorBase = COLOR_NEGRO;
    uint8_t colorTramado = 1;
    int grosorLinea = 5; // Líneas de 3 píxeles de alto

    for (int y = 0; y < altoVentana; y++)
    {
        // Alterna colores cada 3 píxeles
        uint8_t colorFila = ((y / grosorLinea) % 2 == 0) ? colorTramado : colorBase;

        for (int x = 0; x < anchoVentana; x++)
        {
            gbt_dibujar_pixel(x, y, colorFila);
        }
    }
}

void dibujarFondoTexturadoConCuadrados(int anchoVentana, int altoVentana)
{
    uint8_t colorBase = COLOR_NEGRO;
    uint8_t colorTramado = 1;

    // Agrandamos el "píxel" del patrón a 4x4
    int tamPatron = 8;

    for (int y = 0; y < altoVentana; y++)
    {
        for (int x = 0; x < anchoVentana; x++)
        {
            // Dividimos por el tamaño del patrón para escalar la grilla
            if (((x / tamPatron) + (y / tamPatron)) % 2 == 0)
            {
                gbt_dibujar_pixel(x, y, colorTramado);
            }
            else
            {
                gbt_dibujar_pixel(x, y, colorBase);
            }
        }
    }
}

