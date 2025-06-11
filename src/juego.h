#ifndef JUEGO_H
#define JUEGO_H

#include "raylib.h"

// Constantes generales del juego.
#define ANCHO_PANTALLA 1590
#define ALTO_PANTALLA 890
#define TAM_CELDA 28
#define MAX_SEGMENTOS 100
#define ANCHO_PANTALLA_COMIDA 1024
#define ALTO_PANTALLA_COMIDA 850
/* NUEVO: Constantes para obstáculos y powerups */
#define MAX_OBSTACULOS 100

// Estructura para cada segmento de la serpiente.
typedef struct _Serpiente
{
    int x;
    int y;
} SegmentoSerpiente;

// Estructura para cada "comida" (opción) que aparece en la pantalla.
typedef struct _comida
{
    int x;
    int y;
    int valor; // Valor numérico mostrado en la comida.
} FoodOpcion;

// Estructura que almacena una pregunta matemática.
typedef struct _pregunta
{
    char enunciado[64]; // Ej.: "3 + 4 = ?"
    int respuestaCorrecta;
    int opciones[5];    // Cinco opciones numéricas.
    int correctoIndice; // Índice (0..4) de la opción correcta.
} Pregunta;

/* NUEVO: Estructura para obstáculos */
typedef struct _obstacule
{
    int x;
    int y;
} Obstacle;

/*Tipos y estructura para powerups */
typedef enum _Poder
{
    POWERUP_NONE,
    POWERUP_DECREASE, // Disminuye el tamaño de la serpiente.
    POWERUP_IMMUNITY  // Inmunidad frente a trampas/obstáculos.
} TipoPowerUp;

typedef struct _Poderes
{
    TipoPowerUp tipo;
    int activo;           // 0: inactivo, 1: activo.
    float tiempoRestante; // Duración en segundos.
} PowerUp;

/*Estructura para el jefe */
typedef struct _Yepiz
{
    int x;
    int y;
    int activo; // 0: no iniciado, 1: presentación, 2: activo en batalla.
    int vida;   // Se vence al contestar 5 preguntas correctamente.
    int dirX;   // Dirección horizontal.
    int dirY;   // Dirección vertical.
} Boss;

// Estado completo del juego.
typedef struct _juego
{
    SegmentoSerpiente segmentos[MAX_SEGMENTOS];
    int longitud;
    int dirX; // -1: izquierda, 1: derecha, 0: sin movimiento.
    int dirY; // -1: arriba, 1: abajo, 0: sin movimiento.
    int puntaje;
    FoodOpcion comidas[5]; // Las 5 opciones (comida) que se generan.
    int dificultad;        // 0 = Fácil, 1 = Medio, 2 = Difícil, 3 = Extra Difícil.
    Pregunta pregunta;     // Pregunta matemática activa.
    int mostrarPregunta;   // 0 = juego normal, 1 = mostrando desafío, 2 = GAME OVER.
    int comidasCorrectas;  // Aciertos acumulados.

    /*Obstáculos */
    Obstacle obstaculos[MAX_OBSTACULOS];
    int numObstaculos;
    int lastObstacleSpawn; // Para evitar spawns repetidos en el mismo lugar.

    /*Powerup */
    PowerUp powerup;
    int powerupX, powerupY;
    int powerupOnField; // 0 = no hay powerup en el campo, 1 = hay.

    /*Jefe Final */
    Boss boss;
} EstadoJuego;

// Prototipos de funciones del juego.
void IniciarJuego(EstadoJuego *juego);
void ProcesarEntrada(EstadoJuego *juego);
void ActualizarJuego(EstadoJuego *juego);
void DibujarJuego(EstadoJuego *juego);
void GuardarPuntaje(const EstadoJuego *juego, const char *archivo);

// Funciones para el desafío matemático.
void GenerarPregunta(EstadoJuego *juego);
void GenerarOpcionesComida(EstadoJuego *juego);
void ProcesarRespuesta(EstadoJuego *juego);

#endif // JUEGO_H
