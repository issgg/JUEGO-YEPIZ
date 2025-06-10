#include "juego.h"
#include "recursos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void GenerarPregunta(EstadoJuego *juego)
{
    memset(juego->pregunta.enunciado, 0, sizeof(juego->pregunta.enunciado));
    int a, b, c, result;
    char op1, op2;
    int tipo;

    switch (juego->dificultad)
    {
    case 0: // Fácil: solo sumas de 1 dígito
        a = 1 + rand() % 9;
        b = 1 + rand() % 9;
        result = a + b;
        sprintf(juego->pregunta.enunciado, "%d + %d = ? ", a, b);
        break;

    case 1: // Medio: sumas o restas de 1 dígito
        a = 1 + rand() % 9;
        b = 1 + rand() % 9;
        if (rand() % 2 == 0)
        {
            result = a + b;
            sprintf(juego->pregunta.enunciado, "%d + %d = ? ", a, b);
        }
        else
        {
            result = a - b;
            sprintf(juego->pregunta.enunciado, "%d - %d = ? ", a, b);
        }
        break;

    case 2: // Difícil: suma, resta o multiplicación de 1 dígito
        tipo = rand() % 3;
        a = 1 + rand() % 9;
        b = 1 + rand() % 9;
        if (tipo == 0)
        {
            result = a + b;
            sprintf(juego->pregunta.enunciado, "%d + %d = ? ", a, b);
        }
        else if (tipo == 1)
        {
            result = a - b;
            sprintf(juego->pregunta.enunciado, "%d - %d = ? ", a, b);
        }
        else
        {
            result = a * b;
            sprintf(juego->pregunta.enunciado, "%d * %d = ? ", a, b);
        }
        break;

    case 3: // Extremo: (a + b + c) o (a - b - c) o (a * b)
        tipo = rand() % 3;
        if (tipo == 0)
        {
            a = 1 + rand() % 9;
            b = 1 + rand() % 9;
            c = 1 + rand() % 9;
            result = a + b + c;
            sprintf(juego->pregunta.enunciado, "%d + %d + %d = ? ", a, b, c);
        }
        else if (tipo == 1)
        {
            a = 1 + rand() % 9;
            b = 1 + rand() % 9;
            c = 1 + rand() % 9;
            result = a - b - c;
            sprintf(juego->pregunta.enunciado, "%d - %d - %d = ? ", a, b, c);
        }
        else
        {
            a = 2 + rand() % 9;
            b = 2 + rand() % 9;
            result = a * b;
            sprintf(juego->pregunta.enunciado, "%d * %d = ? ", a, b);
        }
        break;
    }

    juego->pregunta.respuestaCorrecta = result;

    // Colocar respuesta correcta en una posición aleatoria
    int indiceCorrecto = rand() % 5;
    juego->pregunta.correctoIndice = indiceCorrecto;

    for (int i = 0; i < 5; i++)
    {
        if (i == indiceCorrecto)
        {
            juego->pregunta.opciones[i] = result;
        }
        else
        {
            int delta;
            do
            {
                delta = (rand() % 11) - 5; // -5 a +5
            } while (delta == 0 || result + delta < -99 || result + delta > 999);
            juego->pregunta.opciones[i] = result + delta;
        }
    }
}

void GenerarOpcionesComida(EstadoJuego *juego)
{
    int maxX = ANCHO_PANTALLA / TAM_CELDA;
    int maxY = ALTO_PANTALLA / TAM_CELDA;
    for (int i = 0; i < 5; i++)
    {
        juego->comidas[i].x = rand() % maxX;
        juego->comidas[i].y = rand() % maxY;
        juego->comidas[i].valor = juego->pregunta.opciones[i];
    }
}

void IniciarJuego(EstadoJuego *juego)
{
    juego->longitud = 3;
    juego->dirX = 1;
    juego->dirY = 0;
    juego->puntaje = 0;
    juego->mostrarPregunta = 1;
    juego->comidasCorrectas = 0;
    juego->numObstaculos = 10;
    juego->lastObstacleSpawn = 10;
    juego->powerupOnField = 0;
    juego->powerup.activo = 0;
    juego->powerup.tiempoRestante = 0;
    juego->boss.activo = 0;
    srand(time(NULL));
    int inicioX = (ANCHO_PANTALLA / TAM_CELDA) / 2;
    int inicioY = (ALTO_PANTALLA / TAM_CELDA) / 2;
    for (int i = 0; i < juego->longitud; i++)
    {
        juego->segmentos[i].x = inicioX - i;
        juego->segmentos[i].y = inicioY;
    }
    GenerarPregunta(juego);
    GenerarOpcionesComida(juego);
}

void ProcesarEntrada(EstadoJuego *juego)
{
    if (IsKeyDown(KEY_RIGHT) && juego->dirX != -1)
    {
        juego->dirX = 1;
        juego->dirY = 0;
    }
    else if (IsKeyDown(KEY_LEFT) && juego->dirX != 1)
    {
        juego->dirX = -1;
        juego->dirY = 0;
    }
    else if (IsKeyDown(KEY_UP) && juego->dirY != 1)
    {
        juego->dirX = 0;
        juego->dirY = -1;
    }
    else if (IsKeyDown(KEY_DOWN) && juego->dirY != -1)
    {
        juego->dirX = 0;
        juego->dirY = 1;
    }
}

void ProcesarRespuesta(EstadoJuego *juego)
{
    if (IsKeyPressed(KEY_ONE))
    {
        juego->segmentos[0].x = juego->comidas[0].x;
        juego->segmentos[0].y = juego->comidas[0].y;
    }
    if (IsKeyPressed(KEY_TWO))
    {
        juego->segmentos[0].x = juego->comidas[1].x;
        juego->segmentos[0].y = juego->comidas[1].y;
    }
    if (IsKeyPressed(KEY_THREE))
    {
        juego->segmentos[0].x = juego->comidas[2].x;
        juego->segmentos[0].y = juego->comidas[2].y;
    }
    if (IsKeyPressed(KEY_FOUR))
    {
        juego->segmentos[0].x = juego->comidas[3].x;
        juego->segmentos[0].y = juego->comidas[3].y;
    }
    if (IsKeyPressed(KEY_FIVE))
    {
        juego->segmentos[0].x = juego->comidas[4].x;
        juego->segmentos[0].y = juego->comidas[4].y;
    }
}

void ActualizarJuego(EstadoJuego *juego)
{
    // --- Control de velocidad de movimiento de la serpiente basados en tiempo ---
    static double lastMoveTime = 0.0;
    double currentTime = GetTime();
    double baseDelay;

    // Define el retardo base según la dificultad
    switch (juego->dificultad)
    {
    case 0: // Fácil
        baseDelay = 0.15;
        break;
    case 1: // Medio
        baseDelay = 0.10;
        break;
    case 2: // Difícil
        baseDelay = 0.07;
        break;
    case 3: // Extremo
        baseDelay = 0.04;
        break;
    }

    // Cada respuesta correcta reduce el retardo en 0.005 segundos.
    double factor = 0.005;
    double minDelay = 0.05; // No permitimos que el delay baje de 0.05 segundos.
    double moveDelay = baseDelay - (juego->comidasCorrectas * factor);
    if (moveDelay < minDelay)
        moveDelay = minDelay;

    // Sólo se actualiza el movimiento si ha pasado el tiempo suficiente.
    if (currentTime - lastMoveTime >= moveDelay)
    {
        // Actualiza la posición de la serpiente (mueve los segmentos)
        for (int i = juego->longitud - 1; i > 0; i--)
        {
            juego->segmentos[i] = juego->segmentos[i - 1];
        }
        juego->segmentos[0].x += juego->dirX;
        juego->segmentos[0].y += juego->dirY;

        int maxX = ANCHO_PANTALLA / TAM_CELDA;
        int maxY = ALTO_PANTALLA / TAM_CELDA;
        if (juego->segmentos[0].x < 0 || juego->segmentos[0].x >= maxX ||
            juego->segmentos[0].y < 0 || juego->segmentos[0].y >= maxY)
        {
            juego->mostrarPregunta = 2; // GAME OVER si la cabeza toca el borde.
        }

        // Colisión con obstáculos (solo si no se tiene el powerup de inmunidad).
        for (int j = 0; j < juego->numObstaculos; j++)
        {
            if (juego->segmentos[0].x == juego->obstaculos[j].x &&
                juego->segmentos[0].y == juego->obstaculos[j].y)
            {
                if (!(juego->powerup.activo == 1 && juego->powerup.tipo == POWERUP_IMMUNITY))
                {
                    juego->mostrarPregunta = 2;
                }
            }
        }

        // Colisión con powerup en el campo.
        if (juego->powerupOnField &&
            juego->segmentos[0].x == juego->powerupX &&
            juego->segmentos[0].y == juego->powerupY)
        {
            juego->powerup.activo = 1;
            juego->powerup.tiempoRestante = 10.0f; // Dura 10 segundos.
            juego->powerupOnField = 0;
            // Efecto inmediato para disminuir el tamaño de la serpiente.
            if (juego->powerup.tipo == POWERUP_DECREASE && juego->longitud > 3)
            {
                juego->longitud = 3;
            }
        }

        // Procesa colisiones y la lógica del desafío (pregunta y respuestas)
        if (juego->mostrarPregunta == 1)
        {
            for (int i = 0; i < 5; i++)
            {
                if (juego->segmentos[0].x == juego->comidas[i].x &&
                    juego->segmentos[0].y == juego->comidas[i].y)
                {
                    // Si la respuesta es correcta:
                    if (juego->comidas[i].valor == juego->pregunta.respuestaCorrecta)
                    {
                        if (juego->boss.activo == 2)
                        {
                            juego->boss.vida--; // Cuando el jefe está activo.
                        }
                        else
                        {
                            juego->puntaje++;
                        }
                        if (juego->longitud < MAX_SEGMENTOS)
                        {
                            juego->segmentos[juego->longitud] = juego->segmentos[juego->longitud - 1];
                            juego->longitud++;
                        }
                        juego->comidasCorrectas++; // Cada acierto aumenta el contador (y afecta la velocidad).

                        // Spawneo de obstáculos según dificultad.
                        int threshold = (juego->dificultad == 2) ? 10 : (juego->dificultad == 3 ? 20 : 999);
                        if (juego->comidasCorrectas >= threshold &&
                            juego->comidasCorrectas - juego->lastObstacleSpawn >= threshold)
                        {
                            if (juego->numObstaculos < MAX_OBSTACULOS)
                            {
                                juego->obstaculos[juego->numObstaculos].x = rand() % maxX;
                                juego->obstaculos[juego->numObstaculos].y = rand() % maxY;
                                juego->numObstaculos++;
                                juego->lastObstacleSpawn = juego->comidasCorrectas;
                            }
                        }

                        // Posible aparición de un powerup (20% de probabilidad).
                        if (!juego->powerupOnField && (rand() % 100) < 20)
                        {
                            juego->powerupX = rand() % maxX;
                            juego->powerupY = rand() % maxY;
                            if (rand() % 2 == 0)
                                juego->powerup.tipo = POWERUP_DECREASE;
                            else
                                juego->powerup.tipo = POWERUP_IMMUNITY;
                            juego->powerup.activo = 0;
                            juego->powerup.tiempoRestante = 0;
                            juego->powerupOnField = 1;
                        }

                        // Regenera un nuevo desafío.
                        GenerarPregunta(juego);
                        GenerarOpcionesComida(juego);
                    }
                    else
                    {
                        juego->mostrarPregunta = 2; // Respuesta incorrecta -> GAME OVER.
                    }
                    break;
                }
            }
        }

        lastMoveTime = currentTime; // Se actualiza el tiempo del último movimiento.
    }

    // Actualiza continuamente el temporizador del powerup (sin depender del movimiento).
    if (juego->powerup.activo == 1)
    {
        juego->powerup.tiempoRestante -= GetFrameTime();
        if (juego->powerup.tiempoRestante <= 0)
            juego->powerup.activo = 0;
    }
}

void DibujarJuego(EstadoJuego *juego)
{
    // Dibuja el fondo escalado.
    Rectangle fuenteFondo = {0, 0, (float)fondoJuego.width, (float)fondoJuego.height};
    Rectangle destinoFondo = {0, 0, (float)ANCHO_PANTALLA, (float)ALTO_PANTALLA};
    DrawTexturePro(fondoJuego, fuenteFondo, destinoFondo, (Vector2){0, 0}, 0, WHITE);

    float escalaCabeza = (float)TAM_CELDA / (float)cabezaSerpiente.width;
    float escalaCuerpo = (float)TAM_CELDA / (float)cuerpoSerpiente.width;
    for (int i = 0; i < juego->longitud; i++)
    {
        Vector2 pos = {juego->segmentos[i].x * TAM_CELDA, juego->segmentos[i].y * TAM_CELDA};
        if (i == 0)
        {
            DrawTextureEx(cabezaSerpiente, pos, 0, escalaCabeza, WHITE);
        }
        else
        {
            DrawTextureEx(cuerpoSerpiente, pos, 0, escalaCuerpo, WHITE);
        }
    }
    if (juego->mostrarPregunta == 1)
    {
        DrawTextEx(fuentePersonalizada, juego->pregunta.enunciado, (Vector2){700, 10}, 30, 1, WHITE);
        char buffer[32];
        for (int i = 0; i < 5; i++)
        {
            int fontSize = 16;
            Vector2 foodPos = {juego->comidas[i].x * TAM_CELDA, juego->comidas[i].y * TAM_CELDA};
            float escalaManzana = (float)TAM_CELDA / (float)manzana.width;
            DrawTextureEx(manzana, foodPos, 0, escalaManzana, YELLOW);
            sprintf(buffer, "%d", juego->pregunta.opciones[i]);
            Vector2 textSize = MeasureTextEx(fuentePersonalizada, buffer, fontSize, 1);
            Vector2 posicionTexto;
            posicionTexto.x = foodPos.x + (TAM_CELDA - textSize.x) / 2;
            posicionTexto.y = foodPos.y + (TAM_CELDA - textSize.y) / 2;
            DrawTextEx(fuentePersonalizada, buffer, posicionTexto, fontSize, 1, WHITE);
        }
    }
    else if (juego->mostrarPregunta == 2)
    {
        Vector2 textSize = MeasureTextEx(fuentePersonalizada, "GAME OVER", 40, 1);
        DrawTextEx(fuentePersonalizada, "GAME OVER", (Vector2){ANCHO_PANTALLA / 3.8 - textSize.x / 2, ALTO_PANTALLA / 2 - 20}, 100, 1, BLACK);
    }
    for (int i = 0; i < juego->numObstaculos; i++)
    {
        Vector2 obstPos = {juego->obstaculos[i].x * TAM_CELDA, juego->obstaculos[i].y * TAM_CELDA};
        if (obstaculoImagen.id)
        {
            float escalaObst = (float)TAM_CELDA / (float)obstaculoImagen.width;
            DrawTextureEx(obstaculoImagen, obstPos, 0, escalaObst, BLACK);
        }
        else
        {
            DrawRectangle(obstPos.x, obstPos.y, TAM_CELDA, TAM_CELDA, RED);
        }
    }
    if (juego->powerupOnField)
    {
        Color pColor = (juego->powerup.tipo == POWERUP_DECREASE) ? WHITE : GREEN;
        DrawRectangle(juego->powerupX * TAM_CELDA, juego->powerupY * TAM_CELDA, TAM_CELDA, TAM_CELDA, pColor);
    }
    DrawTextEx(fuentePersonalizada, TextFormat("Puntaje: %d", juego->puntaje), (Vector2){40, ALTO_PANTALLA - 30}, 30, 1, WHITE);
}
