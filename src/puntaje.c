#include "puntaje.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_PUNTAJES 10
#define MAX_NOMBRE_LENGTH 50

typedef struct
{
    char nombre[MAX_NOMBRE_LENGTH];
    int puntaje;
} RegistroPuntaje;

static RegistroPuntaje registros[MAX_PUNTAJES];
static int numRegistros = 0;

// Lee los puntajes almacenados en el archivo "puntajes.txt".
// Se espera que cada línea contenga: [nombre] [puntaje]
void LeerPuntajes(const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        numRegistros = 0;
        return;
    }

    numRegistros = 0;
    while (numRegistros < MAX_PUNTAJES &&
           fscanf(archivo, "%s %d", registros[numRegistros].nombre, &registros[numRegistros].puntaje) == 2)
    {
        numRegistros++;
    }
    fclose(archivo);
}

// Guarda el puntaje actual en el archivo "puntajes.txt".
// Se utiliza un nombre fijo ("Jugador") junto con el puntaje.
// La firma utiliza const para el puntero a EstadoJuego,
// de modo que concuerda con la declaración en juego.h.
void GuardarPuntaje(const EstadoJuego *juego, const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "a");
    if (archivo == NULL)
        return;

    fprintf(archivo, "Jugador %d\n", juego->puntaje);
    fclose(archivo);
}

// Dibuja la pantalla de puntajes.
// Se leen los puntajes del archivo y se muestran en pantalla con un formato simple.
void DibujarPuntajes(void)
{
    // Actualiza la lista de puntajes leyendo desde "puntajes.txt".
    LeerPuntajes("puntajes.txt");

    // Dibuja el título de la sección.
    DrawText("PUNTAJES", 250, 100, 40, BLACK);

    // Dibuja cada registro, uno debajo del otro.
    int y = 120;
    char buffer[100];
    for (int i = 0; i < numRegistros; i++)
    {
        sprintf(buffer, "%d. %s - %d", i + 1, registros[i].nombre, registros[i].puntaje);
        DrawText(buffer, 250, y, 50, BLACK);
        y += 50;
    }

    DrawText("Presiona ENTER para regresar", 400, 800, 50, RED);
}
