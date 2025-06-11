#ifndef PUNTAJE_H
#define PUNTAJE_H

#include "raylib.h"
#include "juego.h"  // Se asume que aquí se define la estructura EstadoJuego

// Función para dibujar la pantalla de puntajes.
void DibujarPuntajes(void);

// Funciones para el manejo de puntajes en archivo de texto.
void GuardarPuntaje(const EstadoJuego *juego, const char *nombreArchivo);
void LeerPuntajes(const char *nombreArchivo);

#endif // PUNTAJE_H
