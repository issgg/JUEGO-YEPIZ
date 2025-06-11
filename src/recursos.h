#ifndef RECURSOS_H
#define RECURSOS_H

#include "raylib.h"

// Declaración de las texturas usadas
extern Texture2D fondoMenu;
extern Texture2D fondoModos;
// Fondos de juego según dificultad:
extern Texture2D fondoJuegoFacil;
extern Texture2D fondoJuegoMedio;
extern Texture2D fondoJuegoDificil;
extern Texture2D fondoJuegoExtremo;
// Esta variable contendrá el fondo de juego que se usará (según nivel)
extern Texture2D fondoJuego;
extern Texture2D bossImage;
extern Texture2D obstaculoImagen;

extern Texture2D cabezaSerpiente; // Se cargará según el nivel
extern Texture2D cuerpoSerpiente; // Se cargará según el nivel
extern Texture2D manzana;

extern Texture2D posion;
extern Texture2D escudo;
// Declaración de la música.
extern Music musicaMenu;
extern Music musicaFacil;
extern Music musicaMedio;
extern Music musicaDificil;
extern Music musicaExtremo;
extern Music musicaBoss;

// Declaración de la fuente personalizada.
extern Font fuentePersonalizada;

#endif // RECURSOS_H
