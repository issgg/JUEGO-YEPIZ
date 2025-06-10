#ifndef PUNTAJE_H
#define PUNTAJE_H

#include "juego.h"

// Prototipos para la gestión de puntajes.
void GuardarPuntaje(const EstadoJuego *juego, const char *archivo);
void MostrarPuntajes(const char *archivo);

#endif // PUNTAJE_H
