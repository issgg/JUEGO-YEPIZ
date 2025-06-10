#include "puntaje.h"
#include <stdio.h>
#include <stdlib.h>

void MostrarPuntajes(const char *archivo) {
    FILE *fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("No se pudo abrir el archivo %s para leer los puntajes.\n", archivo);
        return;
    }
    char linea[256];
    while (fgets(linea, sizeof(linea), fp)) {
        printf("%s", linea);
    }
    fclose(fp);
}
