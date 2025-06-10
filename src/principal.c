#include "raylib.h"
#include "juego.h"
#include "menu.h"
#include "puntaje.h"
#include "recursos.h"
#include <stdlib.h>

#define NUM_AUDIO_UPDATES 6
#define STATE_MENU 0
#define STATE_SELECCION_NIVEL 1
#define STATE_JUEGO 2
#define STATE_PRESENTACION_JEFE 3 /* NUEVO: Presentación del jefe */
#define STATE_BOSS 4              /* NUEVO: Batalla contra el jefe */

int main(void)
{
    // Inicializa ventana y audio.
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "LA SERPIENTE MATEMÁTICA");
    InitAudioDevice();
    SetTargetFPS(120); // FPS inicial.

    // Cargar fondos y recursos gráficos comunes.
    fondoMenu = LoadTexture("assets/imagenes/fondo_menu.png");
    fondoModos = LoadTexture("assets/imagenes/fondo_modos.png");
    // Fondos de juego se cargarán según el nivel seleccionado.
    fondoJuegoFacil = LoadTexture("assets/imagenes/fondo_juego_facil.png");
    fondoJuegoMedio = LoadTexture("assets/imagenes/fondo_juego_medio.png");
    fondoJuegoDificil = LoadTexture("assets/imagenes/fondo_juego_dificil.png");
    fondoJuegoExtremo = LoadTexture("assets/imagenes/fondo_juego_extremo.png");

    // Cargar la imagen de la manzana.
    manzana = LoadTexture("assets/imagenes/manzana.png");

    // Cargar la fuente personalizada.
    fuentePersonalizada = LoadFont("assets/fuentes/mi_fuente.ttf");

    // Cargar la música del menú.
    musicaMenu = LoadMusicStream("assets/sonidos/musica_menu.ogg");
    PlayMusicStream(musicaMenu);

    // Declarar variables para la música de juego según el nivel.
    Music musicaFacil = {0};
    Music musicaMedio = {0};
    Music musicaDificil = {0};
    Music musicaExtremo = {0};
    Music musicaJuego = {0}; // Música de juego actual.

    /* NUEVO: Cargar recursos para el jefe y obstáculo personalizable */
    bossImage = LoadTexture("assets/imagenes/jefe.png");            // Imagen configurable para el jefe.
    musicaBoss = LoadMusicStream("assets/sonidos/musica_jefe.ogg"); // Música especial para el jefe.
    obstaculoImagen = LoadTexture("assets/imagenes/obstaculo.png"); // Imagen para cuadros rojos personalizable.

    EstadoJuego juego;
    int estadoPrograma = STATE_MENU;
    int nivelSeleccionado = 0; // 0: Fácil, 1: Medio, 2: Difícil, 3: Extra Difícil.

    while (!WindowShouldClose())
    {
        // Actualiza la música de streaming según el estado.
        if (estadoPrograma == STATE_MENU)
        {
            for (int i = 0; i < NUM_AUDIO_UPDATES; i++)
            {
                UpdateMusicStream(musicaMenu);
            }
        }
        else if (estadoPrograma == STATE_JUEGO)
        {
            for (int i = 0; i < NUM_AUDIO_UPDATES; i++)
            {
                UpdateMusicStream(musicaJuego);
            }
        }
        else if (estadoPrograma == STATE_BOSS)
        {
            for (int i = 0; i < NUM_AUDIO_UPDATES; i++)
            {
                UpdateMusicStream(musicaBoss);
            }
        }

        if (estadoPrograma == STATE_MENU)
        {
            int opcion = ActualizarMenu();
            if (opcion == 0)
            { // Selecciona "Jugar"
                StopMusicStream(musicaMenu);
                estadoPrograma = STATE_SELECCION_NIVEL;
            }
        }
        else if (estadoPrograma == STATE_SELECCION_NIVEL)
        {
            int nivel = ActualizarMenuNivel();
            if (nivel >= 0)
            {
                nivelSeleccionado = nivel;
                estadoPrograma = STATE_JUEGO;
                juego.dificultad = nivelSeleccionado;

                // Cargar imágenes de serpiente según nivel y asignar el fondo de juego.
                if (nivelSeleccionado == 0)
                {
                    cabezaSerpiente = LoadTexture("assets/imagenes/cabeza_serpiente_facil.png");
                    cuerpoSerpiente = LoadTexture("assets/imagenes/cuerpo_serpiente_facil.png");
                    fondoJuego = fondoJuegoFacil;
                }
                else if (nivelSeleccionado == 1)
                {
                    cabezaSerpiente = LoadTexture("assets/imagenes/cabeza_serpiente_medio.png");
                    cuerpoSerpiente = LoadTexture("assets/imagenes/cuerpo_serpiente_medio.png");
                    fondoJuego = fondoJuegoMedio;
                }
                else if (nivelSeleccionado == 2)
                {
                    cabezaSerpiente = LoadTexture("assets/imagenes/cabeza_serpiente_dificil.png");
                    cuerpoSerpiente = LoadTexture("assets/imagenes/cuerpo_serpiente_dificil.png");
                    fondoJuego = fondoJuegoDificil;
                }
                else
                {
                    cabezaSerpiente = LoadTexture("assets/imagenes/cabeza_serpiente_extremo.png");
                    cuerpoSerpiente = LoadTexture("assets/imagenes/cuerpo_serpiente_extremo.png");
                    fondoJuego = fondoJuegoExtremo;
                }

                // Cargar y asignar la música de juego según nivel.
                if (nivelSeleccionado == 0)
                {
                    musicaFacil = LoadMusicStream("assets/sonidos/musica_facil.ogg");
                    musicaJuego = musicaFacil;
                }
                else if (nivelSeleccionado == 1)
                {
                    musicaMedio = LoadMusicStream("assets/sonidos/musica_medio.ogg");
                    musicaJuego = musicaMedio;
                }
                else if (nivelSeleccionado == 2)
                {
                    musicaDificil = LoadMusicStream("assets/sonidos/musica_dificil.ogg");
                    musicaJuego = musicaDificil;
                }
                else
                {
                    musicaExtremo = LoadMusicStream("assets/sonidos/musica_extremo.ogg");
                    musicaJuego = musicaExtremo;
                }
                PlayMusicStream(musicaJuego);

                IniciarJuego(&juego);
            }
        }
        else if (estadoPrograma == STATE_JUEGO)
        {
            // Si se alcanza puntos y el jefe aún no se ha activado, pasa a la presentación.
            if (juego.puntaje >= 1 && juego.boss.activo == 0)
            {
                estadoPrograma = STATE_PRESENTACION_JEFE;
            }
            else
            {
                if (juego.mostrarPregunta == 1)
                {
                    ProcesarEntrada(&juego);
                    ProcesarRespuesta(&juego);
                    ActualizarJuego(&juego);
                }
                else if (juego.mostrarPregunta == 0)
                {
                    ProcesarEntrada(&juego);
                    ActualizarJuego(&juego);
                }
                else if (juego.mostrarPregunta == 2)
                {
                    if (IsKeyPressed(KEY_ENTER))
                    {
                        estadoPrograma = STATE_MENU;
                        PlayMusicStream(musicaMenu);
                        StopMusicStream(musicaJuego);
                    }
                }
            }
        }
        else if (estadoPrograma == STATE_PRESENTACION_JEFE)
        {
            // Pausar el juego y mostrar la pantalla de presentación del jefe.
            // Se muestra una pantalla con el fondo de juego y el jefe.
            if (IsKeyPressed(KEY_ENTER))
            {
                // Inicia la batalla del jefe.
                juego.boss.activo = 2;
                juego.boss.vida = 5; // Se vence con 5 preguntas correctas.
                juego.boss.x = rand() % (ANCHO_PANTALLA / TAM_CELDA);
                juego.boss.y = 0;
                juego.boss.dirX = 1;
                juego.boss.dirY = 1;
                estadoPrograma = STATE_BOSS;
                StopMusicStream(musicaJuego);
                PlayMusicStream(musicaBoss);
            }
        }
        else if (estadoPrograma == STATE_BOSS)
        {
            // Si se produce GAME OVER durante la batalla contra el jefe.
            if (juego.mostrarPregunta == 2)
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    estadoPrograma = STATE_MENU;
                    PlayMusicStream(musicaMenu);
                    StopMusicStream(musicaBoss);
                }
            }
            // Si el jefe fue vencido, mostrar mensaje de victoria y permitir volver al menú.
            else if (juego.boss.activo == 2 && juego.boss.vida <= 0)
            {
                Vector2 textSize = MeasureTextEx(fuentePersonalizada, "GANASTE!!", 100, 1);
                DrawTextEx(fuentePersonalizada, "GANASTE!!",
                           (Vector2){ANCHO_PANTALLA / 2 - textSize.x / 2, ALTO_PANTALLA / 2 - textSize.y / 2},
                           100, 1, RED);
                // Se espera que el usuario presione ENTER para volver al menú.
                if (IsKeyPressed(KEY_ENTER))
                {
                    estadoPrograma = STATE_MENU;
                    PlayMusicStream(musicaMenu);
                    StopMusicStream(musicaBoss);
                }
            }
            else
            {
                // Procesa la entrada, la respuesta y la actualización general (movimiento de la serpiente, colisiones, etc.)
                ProcesarEntrada(&juego);
                ProcesarRespuesta(&juego);
                ActualizarJuego(&juego);

                //--------------------------------------------------------------------
                // Movimiento del jefe: controlado por delta time según la dificultad.
                static double lastBossMoveTime = 0.0;
                double currentTime = GetTime();
                double bossMoveDelay;
                // Define el retardo del movimiento del jefe en función del nivel.
                switch (juego.dificultad)
                {
                case 0: // En niveles bajos (aunque el jefe probablemente aparezca en niveles altos)
                    bossMoveDelay = 0.5;
                    break;
                case 1:
                    bossMoveDelay = 0.4;
                    break;
                case 2:
                    bossMoveDelay = 0.2;
                    break;
                case 3: // Extremo: se mueve más rápido (menor retardo)
                    bossMoveDelay = 0.15;
                    break;
                }
                if (currentTime - lastBossMoveTime >= bossMoveDelay)
                {
                    int moveX = 0, moveY = 0;
                    // Con 50% de probabilidad, el jefe se acerca a la cabeza de la serpiente.
                    if (rand() % 2 == 0)
                    {
                        if (juego.boss.x < juego.segmentos[0].x)
                            moveX = 1;
                        else if (juego.boss.x > juego.segmentos[0].x)
                            moveX = -1;
                        if (juego.boss.y < juego.segmentos[0].y)
                            moveY = 1;
                        else if (juego.boss.y > juego.segmentos[0].y)
                            moveY = -1;
                    }
                    // Se añade un componente aleatorio: un pequeño desplazamiento entre -1 y 1.
                    moveX += (rand() % 3) - 1;
                    moveY += (rand() % 3) - 1;

                    juego.boss.x += moveX;
                    juego.boss.y += moveY;
                    lastBossMoveTime = currentTime;
                }
                //--------------------------------------------------------------------

                // Verifica colisión del jefe con la cabeza de la serpiente.
                // Verificar si cualquier segmento de la serpiente toca el jefe

                // Procesa la respuesta en batalla: si la cabeza toca una "comida" de respuesta,
                // y es correcta, el jefe pierde una vida.
                for (int i = 0; i < 5; i++)
                {
                    if (juego.segmentos[0].x == juego.comidas[i].x &&
                        juego.segmentos[0].y == juego.comidas[i].y)
                    {
                        if (juego.comidas[i].valor == juego.pregunta.respuestaCorrecta)
                        {
                            juego.boss.vida--;
                            GenerarPregunta(&juego);
                            GenerarOpcionesComida(&juego);
                        }
                        else
                        {
                            juego.mostrarPregunta = 2;
                        }
                        break;
                    }
                }
            }

            // Dibujo del jefe: se escala de forma imponente (por ejemplo, con un ancho deseado de 800 píxeles).
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (estadoPrograma == STATE_MENU)
        {
            DibujarMenu();
        }
        else if (estadoPrograma == STATE_SELECCION_NIVEL)
        {
            DibujarMenuNivel();
        }
        else if (estadoPrograma == STATE_JUEGO)
        {
            DibujarJuego(&juego);
        }
        else if (estadoPrograma == STATE_PRESENTACION_JEFE)
        {
            // Dibujo de pantalla de presentación del jefe.
            DrawTexturePro(fondoJuego, (Rectangle){0, 0, (float)fondoJuego.width, (float)fondoJuego.height},
                           (Rectangle){0, 0, (float)ANCHO_PANTALLA, (float)ALTO_PANTALLA},
                           (Vector2){0, 0}, 0, WHITE);
            DrawTextEx(fuentePersonalizada, "PREPARATE!!!.", (Vector2){ANCHO_PANTALLA / 2.5 - 50, ALTO_PANTALLA / 2 - 100}, 80, 1, BLACK);
            DrawTextEx(fuentePersonalizada, "El Jefe esta entrando...", (Vector2){ANCHO_PANTALLA / 2.5 - 100, ALTO_PANTALLA / 2}, 55, 1, RED);
            // Se puede dibujar también la imagen del jefe.
            float escalaBoss = (float)TAM_CELDA / (float)bossImage.width;
            Vector2 posBoss = {ANCHO_PANTALLA / 4 - (bossImage.width * escalaBoss) / 1, ALTO_PANTALLA / 2 - (bossImage.height * escalaBoss) / 2 - 100};
            DrawTextureEx(bossImage, posBoss, 0, escalaBoss, WHITE);
        }
        else if (estadoPrograma == STATE_BOSS)
        {
            // Dibuja el juego normal y el jefe encima.
            DibujarJuego(&juego);
            // Dibujo del jefe:
            if (juego.boss.activo == 2)
            {
                // Configuración y dibujo del boss:
                float desiredBossWidth = 150.0f; // Ancho deseado del boss (en pixeles).
                float escalaBoss = desiredBossWidth / (float)bossImage.width;
                Vector2 posBoss = {juego.boss.x * TAM_CELDA, juego.boss.y * TAM_CELDA};
                DrawTextureEx(bossImage, posBoss, 0, escalaBoss, WHITE);
                DrawTextEx(fuentePersonalizada, TextFormat("MATH BOSS: %d", juego.boss.vida),
                           (Vector2){posBoss.x, posBoss.y - 20}, 20, 1, RED);

                // Definir el rectángulo de colisión del boss:
                float bossWidth = bossImage.width * escalaBoss;
                float bossHeight = bossImage.height * escalaBoss;
                Rectangle bossRect = {posBoss.x, posBoss.y, bossWidth, bossHeight};

                // Comprobar, para cada segmento de la serpiente, si colisiona con el boss:
                for (int i = 0; i < juego.longitud; i++)
                {
                    // Convertir la celda del segmento en un rectángulo
                    Rectangle segRect = {juego.segmentos[i].x * TAM_CELDA,
                                         juego.segmentos[i].y * TAM_CELDA,
                                         TAM_CELDA, TAM_CELDA};

                    if (CheckCollisionRecs(bossRect, segRect))
                    {
                        // Si se tiene immunidad (por ejemplo, POWERUP_IMMUNITY) se ignora, sino: GAME OVER.
                        if (!(juego.powerup.activo == 1 && juego.powerup.tipo == POWERUP_IMMUNITY))
                        {
                            juego.mostrarPregunta = 2; // GAME OVER.
                        }
                    }
                }
            }
        }
        EndDrawing();
    }

    // Descargar recursos gráficos.
    UnloadTexture(fondoMenu);
    UnloadTexture(fondoModos);
    UnloadTexture(fondoJuegoFacil);
    UnloadTexture(fondoJuegoMedio);
    UnloadTexture(fondoJuegoDificil);
    UnloadTexture(fondoJuegoExtremo);
    UnloadTexture(fondoJuego);
    UnloadTexture(cabezaSerpiente);
    UnloadTexture(cuerpoSerpiente);
    UnloadTexture(manzana);
    UnloadTexture(bossImage);
    UnloadTexture(obstaculoImagen);

    // Descargar música.
    UnloadMusicStream(musicaMenu);
    UnloadMusicStream(musicaFacil);
    UnloadMusicStream(musicaMedio);
    UnloadMusicStream(musicaDificil);
    UnloadMusicStream(musicaExtremo);
    UnloadMusicStream(musicaBoss);

    // Descargar la fuente.
    UnloadFont(fuentePersonalizada);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
