#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include "funcoes.h"
#include <time.h>
#include <string.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    Game game;
    game.screenWidth = 1280;
    game.screenHeight = 720;

    char name[MAX_CHARS + 1] = "\0";
    int letterCount = 0;

    Rectangle textBox = {(game.screenWidth / 2) + 200, game.screenHeight / 2 + 100, 328, 80};
    bool mouseOnText = false;

    Sound sfx = LoadSound("sounds/SFX.mp3");
    InitWindow(game.screenWidth, game.screenHeight, "Urubu do pix");
    SetTargetFPS(60);
    InitAudioDevice();

    carregaPosicoes(game.info);
    int framesCounter = 0;

    time_t inicio_jogo, fim_jogo;

    Sound Caul = LoadSound("sounds/Caul.mp3");
    PlaySound(Caul);
    SetSoundVolume(Caul, 1);

    Texture2D nome = LoadTexture("Background/cabeca1.png");
    Texture2D saymyname = LoadTexture("Background/Say.png");
    Sound say = LoadSound("sounds/meme.mp3");

    Texture2D pills = LoadTexture("Background/pills.png");

    Font fonte;
    fonte = LoadFont("fontes/SaulB.ttf");
    Font bit = LoadFont("fontes/8bit.TTF");
    Texture2D deserto = LoadTexture("Background/Felps1.png");
    Vector2 tam_texto = {(game.screenWidth - 170) / 3, game.screenHeight / 3 - 150};
    Vector2 fonteT = {game.screenWidth / 2 - 100, game.screenHeight / 2};
    Vector2 P = {game.screenWidth / 2 + 50, game.screenHeight / 2 + 250};
    Vector2 P2 = {game.screenWidth / 2 - 250, game.screenHeight / 3 - 50};
    Vector2 Ponto = {GetScreenWidth() / 2 - MeasureText("Selecione o modo de jogo: ", 45), GetScreenHeight() / 2 - 25};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(deserto, 0, 0, RAYWHITE);

        DrawTextEx(bit, "PRESS F TO START", P, 35, 0, YELLOW);
        EndDrawing();
        if (IsKeyPressed(KEY_F))
        {
            UnloadFont(bit);
            UnloadTexture(deserto);
            PlaySound(sfx);

            break;
        }
    }

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (CheckCollisionPointRec(GetMousePosition(), textBox))
            mouseOnText = true;
        else
            mouseOnText = false;

        if (mouseOnText)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Get char pressed (unicode character) on the queue
            int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_CHARS))
                {
                    name[letterCount] = (char)key;
                    name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }

                key = GetCharPressed(); // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                letterCount--;
                if (letterCount < 0)
                    letterCount = 0;
                name[letterCount] = '\0';
            }
        }
        else
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnText)
            framesCounter++;
        else
            framesCounter = 0;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTexture(nome, 0, 0, RAYWHITE);

        DrawTexture(saymyname, 750, 100, RAYWHITE);

        DrawRectangleRec(textBox, (Color){28, 58, 47, 255});
        if (mouseOnText)
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RAYWHITE);
        else
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RAYWHITE);

        DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 60, WHITE);

        if (mouseOnText)
        {
            if (letterCount < MAX_CHARS)
            {
                if (((framesCounter / 20) % 2) == 0)
                    DrawText("_", (int)textBox.x + 8 + MeasureText(name, 60), (int)textBox.y + 12, 60, WHITE);
            }
        }

        EndDrawing();
        if (IsKeyPressed(KEY_ENTER) && letterCount > 0)
        {
            UnloadFont(fonte);
            mouseOnText = false;
            PauseSound(Caul);
            PlaySound(say);
            SetSoundVolume(say, 4);
            break;
        }

        else if (IsKeyPressed(KEY_ESCAPE))
        {
            game.gameover = 1;
            UnloadFont(fonte);
        }
    }

    strcpy(game.info[3].nome, name);

    printf("%s\n", game.info[3].nome);

    bit = LoadFont("fontes/8bit.TTF");
    Rectangle dificil = {(game.screenWidth / 2) + 150, game.screenHeight / 2 + 90, 328, 80};
    Rectangle facil = {(game.screenWidth / 2) - 500, game.screenHeight / 2 + 90, 328, 80};
    Vector2 facil2 = {(facil.width / 2) + facil.x - 95, facil.height / 2 + facil.y - 25};
    Vector2 dificil2 = {(dificil.width / 2) + dificil.x - 120, dificil.height / 2 + dificil.y - 25};
    Vector2 Dificuldade = {game.screenWidth / 2 - 470, game.screenHeight / 2 - 200};

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        sleep(2);
        ResumeSound(Caul);
        // Update
        //----------------------------------------------------------------------------------
        if (CheckCollisionPointRec(GetMousePosition(), facil))
        {
            
            mouseOnText = true;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                PlaySound(sfx);
                game.dificuldade = 0;
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                break;
            }
        }
        else
            mouseOnText = false;

        if (CheckCollisionPointRec(GetMousePosition(), dificil))
        {
            
            mouseOnText = true;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                PlaySound(sfx);
                game.dificuldade = 1;
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                break;
            }
        }
        else
            mouseOnText = false;

        if (CheckCollisionPointRec(GetMousePosition(), facil) || CheckCollisionPointRec(GetMousePosition(), dificil))
        {
            // Set the window's cursor to the I-Beam

            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }
        else
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTexture(pills, 0, 0, RAYWHITE);

        DrawRectangleRec(facil, LIGHTGRAY);
        if (CheckCollisionPointRec(GetMousePosition(), facil))
        {
            DrawRectangleLines((int)facil.x, (int)facil.y, (int)facil.width, (int)facil.height, RED);
            DrawTextEx(bit, "FACIL", facil2, 45, 0, BLUE);
        }
        else
        {
            DrawRectangleLines((int)facil.x, (int)facil.y, (int)facil.width, (int)facil.height, DARKGRAY);
            DrawTextEx(bit, "FACIL", facil2, 45, 0, YELLOW);
        }

        DrawRectangleRec(dificil, LIGHTGRAY);
        if (CheckCollisionPointRec(GetMousePosition(), dificil))
        {
            DrawRectangleLines((int)dificil.x, (int)dificil.y, (int)dificil.width, (int)dificil.height, RED);
            DrawTextEx(bit, "DIFICIL", dificil2, 45, 0, RED);
        }
        else
        {
            DrawRectangleLines((int)dificil.x, (int)dificil.y, (int)dificil.width, (int)dificil.height, DARKGRAY);
            DrawTextEx(bit, "DIFICIL", dificil2, 45, 0, YELLOW);
        }

        DrawTextEx(bit, "SELECIONE A DIFICULDADE", Dificuldade, 45, 0, RED);

        EndDrawing();
    }

    InitGame(&game);
    StopSound(Caul);
    Sound intro = LoadSound("sounds/intro.mp3");
    PlaySound(intro);
    SetSoundVolume(intro, 1);
    inicio_jogo = time(NULL);

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }

        UpdateDrawFrame(&game);
        if (game.gameover)
        {
            StopSound(intro);
            Sound morte = LoadSound("sounds/morte.mp3");
            PlaySound(morte);
            SetSoundVolume(morte, 7);
            break;
        }
    }

    fim_jogo = time(NULL);

    double duracao_jogo = difftime(fim_jogo, inicio_jogo);
    int minutos = duracao_jogo / 60;
    int segundos = (int)duracao_jogo % 60;
    game.info[3].pontuacao = duracao_jogo;

    printf("%lf\n", duracao_jogo);
    if (game.maps[7].enemies[0].draw_enemy == 0)
        salvarPosicoes(game.info);
    char tempo_texto[50];

    while (!IsKeyDown(KEY_ENTER))
    {
        BeginDrawing();
        ClearBackground(BLACK);
        if (game.gameover == 2)
        {
            DrawText("YOU LIVED", GetScreenWidth() / 2 - MeasureText("YOU LIVED", 200) / 2, GetScreenHeight() / 2 - 100, 200, RED);
        }
        else
        {
            DrawText("YOU DIED", GetScreenWidth() / 2 - MeasureText("YOU DIED", 200) / 2, GetScreenHeight() / 2 - 100, 200, RED);
        }

        char plcar[200];
        snprintf(plcar, sizeof(plcar), "1°: %s - %d\n2°: %s - %d\n3°: %s - %d", game.info[0].nome, game.info[0].pontuacao, game.info[1].nome, game.info[1].pontuacao,
                 game.info[2].nome, game.info[2].pontuacao);

        snprintf(tempo_texto, sizeof(tempo_texto), "Tempo de jogo %d minutos e %.2d segundos", minutos, segundos);
        Vector2 tempo = {GetScreenWidth() / 2 - MeasureText(tempo_texto, 20) / 2 - 130, GetScreenHeight() / 2 + 300};

        DrawTextEx(bit, tempo_texto, tempo, 20, 0, YELLOW);

        DrawRectangle(GetScreenWidth() / 2 - MeasureText(plcar, 20) / 2 - 90, 3 * GetScreenHeight() / 4 - 20, 290, 110, RAYWHITE); // quadro
        DrawRectangleLines(GetScreenWidth() / 2 - MeasureText(plcar, 20) / 2 - 90, 3 * GetScreenHeight() / 4 - 20, 290, 110, RED);

        DrawText(plcar, GetScreenWidth() / 2 - MeasureText(plcar, 20) / 2 - 80, 3 * GetScreenHeight() / 4 - 13, 25, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}