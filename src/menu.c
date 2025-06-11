#include "juego.h" // Para usar ANCHO_PANTALLA y ALTO_PANTALLA.
#include "menu.h"
#include "recursos.h" // Para usar los fondos del menú y selección.
#include "raylib.h"
#include <stdlib.h>

static int seleccionActual = 0;
static const int numOpciones = 3;
static const char *opciones[] = {"Jugar", "PUNTAJE", "SALIR"};

int ActualizarMenu(void)
{
    // Actualiza la opción seleccionada según las flechas del teclado
    if (IsKeyPressed(KEY_DOWN))
    {
        seleccionActual = (seleccionActual + 1) % numOpciones;
    }
    if (IsKeyPressed(KEY_UP))
    {
        seleccionActual = (seleccionActual - 1 + numOpciones) % numOpciones;
    }

    // Cuando se presiona ENTER, se retorna el índice de la opción seleccionada.
    if (IsKeyPressed(KEY_ENTER))
    {
        if (seleccionActual == 0)
        {
            return 0; // Opción "Jugar"
        }
        else if (seleccionActual == 1)
        {
            return 1; // Opción "Puntajes"
        }
        else if (seleccionActual == 2)
        {
            CloseWindow();
            exit(0); // Opción "Salir"
        }
    }
    return -1; // Ninguna opción confirmada
}

void DibujarMenu(void)
{
    // Dibuja el fondo del menú escalado a la ventana.
    Rectangle fuente = {0, 0, (float)fondoMenu.width, (float)fondoMenu.height};
    Rectangle destino = {0, 0, (float)ANCHO_PANTALLA, (float)ALTO_PANTALLA};
    DrawTexturePro(fondoMenu, fuente, destino, (Vector2){0, 0}, 0, WHITE);

    int centroX = ANCHO_PANTALLA / 2;
    int centroY = ALTO_PANTALLA / 2;
    DrawTextEx(fuentePersonalizada, "THE MATHEMATICAL SNAKE ", (Vector2){centroX - MeasureTextEx(fuentePersonalizada, "THE MATHEMATICAL SNAKE", 100, 1).x / 2, centroY - 300}, 100, 1, DARKBLUE);
    for (int i = 0; i < numOpciones; i++)
    {
        Color color = (i == seleccionActual) ? RED : BLACK;
        DrawTextEx(fuentePersonalizada, opciones[i], (Vector2){centroX - MeasureTextEx(fuentePersonalizada, opciones[i], 80, 1).x / 2, centroY + i * 80}, 80, 1, color);
    }
}

static int seleccionNivel = 0;
static const int numNiveles = 4;
static const char *niveles[] = {"FACIL", "MEDIO", "DIFICIL", "EXTRA DIFICIL"};

int ActualizarMenuNivel(void)
{
    if (IsKeyPressed(KEY_DOWN))
    {
        seleccionNivel = (seleccionNivel + 1) % numNiveles;
    }
    if (IsKeyPressed(KEY_UP))
    {
        seleccionNivel = (seleccionNivel - 1 + numNiveles) % numNiveles;
    }
    if (IsKeyPressed(KEY_ENTER))
    {
        return seleccionNivel;
    }
    return -1;
}

void DibujarMenuNivel(void)
{
    // Dibuja el fondo de modos escalado a la ventana.
    Rectangle fuente = {0, 0, (float)fondoModos.width, (float)fondoModos.height};
    Rectangle destino = {0, 0, (float)ANCHO_PANTALLA, (float)ALTO_PANTALLA};
    DrawTexturePro(fondoModos, fuente, destino, (Vector2){0, 0}, 0, WHITE);

    int centroX = ANCHO_PANTALLA / 2;
    int centroY = ALTO_PANTALLA / 2;
    DrawTextEx(fuentePersonalizada, "Seleccione Nivel", (Vector2){centroX - MeasureTextEx(fuentePersonalizada, "Seleccione Nivel", 100, 1).x / 2, centroY - 100}, 100, 1, PURPLE);
    for (int i = 0; i < numNiveles; i++)
    {
        Color color = (i == seleccionNivel) ? RED : WHITE;
        DrawTextEx(fuentePersonalizada, niveles[i], (Vector2){centroX - MeasureTextEx(fuentePersonalizada, niveles[i], 70, 1).x / 2, centroY + i * 60}, 70, 1, color);
    }
}
