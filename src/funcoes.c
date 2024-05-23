#include "funcoes.h"

// Initialize game variables
void InitGame(Game *g)
{

    g->curr_map = 0;
    g->num_maps = 10;
    g->hero.pos = (Rectangle){15, 15, 49, 111};
    g->hero.color = RAYWHITE;
    g->hero.speed = 5;
    g->hero.special = 0;
    g->hero.direction = 1;
    g->gameover = 0;
    map0_setup(g);
    map1_setup(g);
    map2_setup(g);
    map3_setup(g);
    map4_setup(g);
    map5_setup(g);
    map6_setup(g);
    map7_setup(g);

    // Initialize shoots
    for (int i = 0; i < 2 - g->dificuldade; i++)
    {
        g->shoot[i].pos1.x = g->hero.pos.x;
        g->shoot[i].pos1.y = g->hero.pos.y + g->hero.pos.height / 2;
        g->shoot[i].pos1.width = 25;
        g->shoot[i].pos1.height = 25;
        g->shoot[i].speed.x = 0;
        g->shoot[i].speed.y = 0;
        g->shoot[i].active = false;
        g->shoot[i].color = RAYWHITE;
        g->shoot[i].direction = 2;
    }
    for (int i = 0; i < 1 + g->dificuldade; i++)
    {
        g->enemy[i].pos1.x = g->maps->enemies[i].pos.x;
        g->enemy[i].pos1.y = g->maps->enemies[i].pos.y + g->maps->enemies[i].pos.height / 2;
        g->enemy[i].pos1.width = 18;
        g->enemy[i].pos1.height = 20;
        g->enemy[i].speed.x = 0;
        g->enemy[i].speed.y = 0;
        g->enemy[i].active = false;
        g->enemy[i].color = YELLOW;
        g->enemy[i].direction = KEY_LEFT;
    }

    
    g->special[0].texture = LoadTexture("sprites/rede.png");
    g->special[1].texture = LoadTexture("sprites/wpp.png");
    g->som_tiro = LoadSound("sounds/fireball.mp3");
    g->maps->enemies[0].som_explosion = LoadSound("sounds/explosion.mp3");
    

    g->key.key.texture = LoadTexture("sprites/key.png");
    g->hero.hero_texture[1].texture = LoadTexture("sprites/felpsd.png");
    g->hero.hero_texture[0].texture = LoadTexture("sprites/felps.png");

    g->shoot[0].shoot_texture = LoadTexture("sprites/tiro.png");
    g->enemy[0].shoot_texture = LoadTexture("sprites/ovo.png");

    g->item = LoadSound("sounds/Item.mp3");
    
    g->victory = LoadTexture("Background/Victory1.png");

    g->creditos.x = GetScreenWidth() / 2 - 300;
    g->creditos.y = 560;
}

// Update game (one frame)
void UpdateGame(Game *g)
{
    update_hero_pos(g);
    update_tiro_pos(g);
    update_tiro_enemy_pos(g);

    Map *map = &g->maps[g->curr_map];
    for (int i = 0; i < map->num_enemies; i++)
    {
        if (g->curr_map == 7 && g->maps[7].enemies[0].draw_enemy == 0)
        {
            while (!WindowShouldClose())
            {
                BeginDrawing();
                
                
                DrawTexture(g->victory, 0, 0, RAYWHITE);
                DrawText("OBRIGADO POR JOGAR!", GetScreenWidth() / 2 - 220, 635 - g->creditos.y, 35, RED);
                DrawText("CREDITOS:", GetScreenWidth() / 2 - 120, 670 - g->creditos.y, 35, YELLOW);
                DrawText("DIRETORES/CRIADORES DA OBRA", GetScreenWidth() / 2 - 320, 705 - g->creditos.y, 35, YELLOW);
                DrawText("Eduardo Gomes Martins & Lucas Daniel Lana Maciel", GetScreenWidth() / 2 - 420, 740 - g->creditos.y, 35, YELLOW);

                EndDrawing();
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    g->gameover = 2;
                    break;
                }
            }
        }
        if (!map->enemies[i].draw_enemy)
            continue;
        update_enemy_pos(g, &map->enemies[i]);
        if (!CheckCollisionRecs(g->hero.pos, map->enemies[i].pos))
            continue;
        if (g->curr_map == 1 && g->hero.special == 1)
        {
            map->enemies[i].draw_enemy = 0;
            PlaySound(g->som_rede);
            SetSoundVolume(g->som_rede, 1);
            continue;
        }
        if (g->curr_map != 1)
        {
            reinicia_mapa(g);
            reseta_tiro(g);
        }
    }

    if (CheckCollisionRecs(g->hero.pos, map->special_item))
    {
        g->hero.special = 1;
        map->draw_special_item = 0;
        PlaySound(g->item);
        if (g->curr_map == 7)
            g->hero.color = GREEN;
    }

    if (CheckCollisionRecs(g->hero.pos, map->door) && map->door_locked == 0)
    {
        g->curr_map = map->next_map;
        g->hero.pos = (Rectangle){50, 200, g->hero.pos.width, g->hero.pos.height};
        g->hero.special = 0;
        reseta_tiro(g);
    }

    if (map->prev_map != -1 && CheckCollisionRecs(g->hero.pos, map->prev_door))
    {
        g->curr_map = map->prev_map;
        g->hero.pos = (Rectangle){g->screenWidth - 60, g->screenHeight / 3, g->hero.pos.width, g->hero.pos.height};
        g->hero.special = 0;
        reseta_tiro(g);
    }
}

// Draw game (one frame)
void DrawGame(Game *g)
{
    BeginDrawing();
    Map *map = &g->maps[g->curr_map];

    ClearBackground(RAYWHITE);
    DrawRectangle(0, 0, g->screenWidth, g->screenHeight, GRAY);

    draw_map(g);
    draw_borders(g);

    if (g->hero.direction == 0)
    {
        DrawTextureRec(g->hero.hero_texture[0].texture, (Rectangle){0, 0, g->hero.pos.width, g->hero.pos.height}, (Vector2){g->hero.pos.x, g->hero.pos.y}, g->hero.color);
    }
    if (g->hero.direction == 1)
    {
        DrawTextureRec(g->hero.hero_texture[1].texture, (Rectangle){0, 0, g->hero.pos.width, g->hero.pos.height}, (Vector2){g->hero.pos.x, g->hero.pos.y}, g->hero.color);
    }
    for (int i = 0; i < 2 - g->dificuldade; i++)
    {
        if (g->shoot[i].active)
        {
            DrawTextureRec(g->shoot[0].shoot_texture, (Rectangle){0, 0, g->shoot[i].pos1.width, g->shoot[i].pos1.height}, (Vector2){g->shoot[i].pos1.x, g->shoot[i].pos1.y}, g->shoot[i].color);
        }
    }
    for (int i = 0; i < 1 + g->dificuldade; i++)
    {
        if (g->enemy[i].active)
        {
            DrawTextureRec(g->enemy[0].shoot_texture, (Rectangle){0, 0, g->enemy[i].pos1.width, g->enemy[i].pos1.height}, (Vector2){g->enemy[i].pos1.x, g->enemy[i].pos1.y}, RAYWHITE);
        }
    }

    for (int i = 0; i < map->num_enemies; i++)
    {
        if (!map->enemies[i].draw_enemy)
        {
            if (map->enemies[i].has_key)
            {
                g->key.key_pos = map->enemies[i].pos;
                DrawTextureRec(g->key.key.texture, (Rectangle){0, 0, 23, 23}, (Vector2){map->enemies[i].pos.x, map->enemies[i].pos.y}, RAYWHITE);
            }
            if (CheckCollisionRecs(g->hero.pos, g->key.key_pos))
            {
                map->enemies[i].has_key = 0;
                map->door_locked = 0;
            }
            continue;
        }
        if (map->enemies[i].direction == KEY_LEFT)
            DrawTextureRec(map->enemies[i].enemy_texture[1].texture, (Rectangle){0, 0, map->enemies[i].pos.width, map->enemies[i].pos.height}, (Vector2){map->enemies[i].pos.x, map->enemies[i].pos.y}, map->enemies[i].color);
        if (map->enemies[i].direction == KEY_RIGHT)
            DrawTextureRec(map->enemies[i].enemy_texture[0].texture, (Rectangle){0, 0, map->enemies[i].pos.width, map->enemies[i].pos.height}, (Vector2){map->enemies[i].pos.x, map->enemies[i].pos.y}, map->enemies[i].color);
        if (map->enemies[i].direction == KEY_UP)
            DrawTextureRec(map->enemies[i].enemy_texture[1].texture, (Rectangle){0, 0, map->enemies[i].pos.width, map->enemies[i].pos.height}, (Vector2){map->enemies[i].pos.x, map->enemies[i].pos.y}, map->enemies[i].color);
        if (map->enemies[i].direction == KEY_DOWN)
            DrawTextureRec(map->enemies[i].enemy_texture[0].texture, (Rectangle){0, 0, map->enemies[i].pos.width, map->enemies[i].pos.height}, (Vector2){map->enemies[i].pos.x, map->enemies[i].pos.y}, map->enemies[i].color);
    }

    EndDrawing();
}

// Update and Draw (one frame)
void UpdateDrawFrame(Game *g)
{
    UpdateGame(g);
    DrawGame(g);
}

void draw_borders(Game *g)
{
    DrawRectangle(0, 0, SCREEN_BORDER, g->screenHeight, BLACK);
    DrawRectangle(0, 0, g->screenWidth, SCREEN_BORDER, BLACK);
    DrawRectangle(g->screenWidth - SCREEN_BORDER, 0, g->screenWidth, g->screenHeight, BLACK);
    DrawRectangle(0, g->screenHeight - SCREEN_BORDER, g->screenWidth, g->screenHeight, BLACK);
}

void draw_map(Game *g)
{
    Map *map = &g->maps[g->curr_map];
    DrawTexture(map->background.texture, 0, 0, (Color){255, 255, 255, 255});
    for (int i = 0; i < map->num_barriers; i++)
    {
        DrawRectangleRec(map->barriers[i], BLACK);
    }

    if (!map->door_locked)
        DrawRectangleRec(map->door, BLUE);

    if (map->prev_map != -1)
    {
        DrawRectangleRec(map->prev_door, ORANGE);
    }

    if (map->draw_special_item)
    {
        if (g->curr_map == 1)
            DrawTextureRec(g->special[0].texture, (Rectangle){0, 0, 60, 60}, (Vector2){g->maps[g->curr_map].special_item.x, g->maps[g->curr_map].special_item.y}, RAYWHITE);
        if (g->curr_map == 7)
            DrawTextureRec(g->special[1].texture, (Rectangle){0, 0, 60, 60}, (Vector2){g->maps[g->curr_map].special_item.x, g->maps[g->curr_map].special_item.y}, RAYWHITE);
    }
}
void update_hero_pos(Game *g)
{

    Hero *h = &g->hero;
    Map *m = &g->maps[g->curr_map];
    Shoot *s = &g->shoot[2 - g->dificuldade];

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        if (h->pos.x > SCREEN_BORDER)
            h->pos.x -= h->speed;
        if (barrier_collision(m, &h->pos))
            h->pos.x += h->speed;
        for (int i = 0; i < 2 - g->dificuldade; i++)
        {
            g->shoot[i].direction = 1;
        }
        g->hero.direction = 0;
    }
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        if (h->pos.x + h->pos.width < g->screenWidth - SCREEN_BORDER)
            h->pos.x += h->speed;
        if (barrier_collision(m, &h->pos))
            h->pos.x -= h->speed;
        for (int i = 0; i < 2 - g->dificuldade; i++)
        {
            g->shoot[i].direction = 2;
        }
        g->hero.direction = 1;
    }
    else if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        if (h->pos.y > SCREEN_BORDER)
            h->pos.y -= h->speed;
        if (barrier_collision(m, &h->pos))
            h->pos.y += h->speed;
        for (int i = 0; i < 2 - g->dificuldade; i++)
        {
            g->shoot[i].direction = 3;
        }
    }
    else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        if (h->pos.y + h->pos.height < g->screenHeight - SCREEN_BORDER)
            h->pos.y += h->speed;
        if (barrier_collision(m, &h->pos))
            h->pos.y -= h->speed;
        for (int i = 0; i < 2 - g->dificuldade; i++)
        {
            g->shoot[i].direction = 4;
        }
    }
    if (IsKeyDown(KEY_A) && IsKeyDown(KEY_W) || IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_UP))
    {
        if (h->pos.x > SCREEN_BORDER)
            h->pos.x -= h->speed / 2;

        if (h->pos.y > SCREEN_BORDER)
            h->pos.y -= h->speed / 2;

        if (barrier_collision(m, &h->pos))
        {
            h->pos.y += h->speed / 2;
            h->pos.x += h->speed / 2;
        }
        for (int i = 0; i < 2 - g->dificuldade; i++)
        {
            g->shoot[i].direction = 5;
        }
        g->hero.direction = 0;
    }
    if (IsKeyDown(KEY_D) && IsKeyDown(KEY_W) || IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_UP))
    {
        if (h->pos.x + h->pos.width < g->screenWidth - SCREEN_BORDER)
            h->pos.x += h->speed / 2;

        if (h->pos.y > SCREEN_BORDER)
            h->pos.y -= h->speed / 2;

        if (barrier_collision(m, &h->pos))
        {
            h->pos.y += h->speed / 2;
            h->pos.x -= h->speed / 2;
        }
        for (int i = 0; i < 2 - g->dificuldade; i++)
        {
            g->shoot[i].direction = 6;
        }
        g->hero.direction = 1;
    }
    if (IsKeyDown(KEY_A) && IsKeyDown(KEY_S) || IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_DOWN))
    {
        if (h->pos.x > SCREEN_BORDER)
            h->pos.x -= h->speed / 2;

        if (h->pos.y + h->pos.height < g->screenHeight - SCREEN_BORDER)
            h->pos.y += h->speed / 2;

        if (barrier_collision(m, &h->pos))
        {
            h->pos.y -= h->speed / 2;
            h->pos.x += h->speed / 2;
        }
        for (int i = 0; i < 2 - g->dificuldade; i++)
        {
            g->shoot[i].direction = 7;
        }
        g->hero.direction = 0;
    }
    if (IsKeyDown(KEY_D) && IsKeyDown(KEY_S) || IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_DOWN))
    {
        if (h->pos.x + h->pos.width < g->screenWidth - SCREEN_BORDER)
            h->pos.x += h->speed / 2;

        if (h->pos.y + h->pos.height < g->screenHeight - SCREEN_BORDER)
            h->pos.y += h->speed / 2;

        if (barrier_collision(m, &h->pos))
        {
            h->pos.y -= h->speed / 2;
            h->pos.x -= h->speed / 2;
        }
        for (int i = 0; i < 2 - g->dificuldade; i++)
        {
            g->shoot[i].direction = 8;
        }
        g->hero.direction = 1;
    }
}

void update_tiro_pos(Game *g)
{
    Map *map = &g->maps[g->curr_map];
    if (g->curr_map == 0 || g->curr_map > 1)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            // Encontra um tiro inativo e o ativa
            for (int i = 0; i < 2 - g->dificuldade; i++)
            {
                if (!g->shoot[i].active)
                {
                    PlaySound(g->som_tiro);
                    g->shoot[i].pos1.x = g->hero.pos.x + g->hero.pos.width / 2;
                    g->shoot[i].pos1.y = g->hero.pos.y + g->hero.pos.height / 2;
                    g->shoot[i].active = true;

                    if (g->shoot[i].direction == 1)
                    {
                        g->shoot[i].speed.x = -10;
                        g->shoot[i].speed.y = 0;
                    }
                    else if (g->shoot[i].direction == 2)
                    {
                        g->shoot[i].speed.x = +10;
                        g->shoot[i].speed.y = 0;
                    }
                    else if (g->shoot[i].direction == 3)
                    {
                        g->shoot[i].speed.x = 0;
                        g->shoot[i].speed.y = -10;
                    }
                    else if (g->shoot[i].direction == 4)
                    {
                        g->shoot[i].speed.x = 0;
                        g->shoot[i].speed.y = +10;
                    }
                    else if (g->shoot[i].direction == 5)
                    {
                        g->shoot[i].speed.x = -5;
                        g->shoot[i].speed.y = -5;
                    }
                    else if (g->shoot[i].direction == 6)
                    {
                        g->shoot[i].speed.x = +5;
                        g->shoot[i].speed.y = -5;
                    }
                    else if (g->shoot[i].direction == 7)
                    {
                        g->shoot[i].speed.x = -5;
                        g->shoot[i].speed.y = 5;
                    }
                    else if (g->shoot[i].direction == 8)
                    {
                        g->shoot[i].speed.x = 5;
                        g->shoot[i].speed.y = 5;
                    }
                    break;
                }
            }
        }
    }
    for (int i = 0; i < 2 - g->dificuldade; i++)
    {
        if (g->shoot[i].active)
        {
            g->shoot[i].pos1.x += g->shoot[i].speed.x;
            g->shoot[i].pos1.y += g->shoot[i].speed.y;
            for (int j = 0; j < map->num_enemies; j++)
            {
                if (map->enemies[j].draw_enemy)
                {
                    if (CheckCollisionRecs(g->shoot[i].pos1, map->enemies[j].pos))
                    {
                        if (g->curr_map != 7)
                        {
                            g->shoot[i].active = false;
                            map->enemies[j].draw_enemy = 0;
                            map->dead_enemies++;
                            PlaySound(g->maps->enemies[0].som_explosion);
                            SetSoundVolume(g->maps->enemies[0].som_explosion, 1);
                        }
                        else if (g->hero.special == 1)
                        {
                            g->shoot[i].active = false;
                            map->enemies[j].draw_enemy = 0;
                            map->dead_enemies++;
                            PlaySound(g->maps->enemies[0].som_explosion);
                            SetSoundVolume(g->maps->enemies[0].som_explosion, 1);
                        }
                        else
                        {
                            g->shoot[i].active = false;
                        }
                    }
                }
                if (g->shoot[i].active)
                {

                    // Verificar colisão do tiro com as barreiras
                    if (barrier_collision(map, &g->shoot[i].pos1))
                    {
                        g->shoot[i].active = false;
                    }

                    // Verificar se o tiro saiu da tela
                    if (g->shoot[i].pos1.x + g->shoot[i].pos1.width < 0 || g->shoot[i].pos1.x > g->screenWidth)
                    {
                        g->shoot[i].active = false;
                    }

                    if (g->shoot[i].pos1.y + g->shoot[i].pos1.height < 0 || g->shoot[i].pos1.y > g->screenHeight)
                    {
                        g->shoot[i].active = false;
                    }
                }
            }
        }
    }
}

void update_tiro_enemy_pos(Game *g)
{
    Map *map = &g->maps[g->curr_map];
    if (g->curr_map != 4 && g->curr_map != 1 && g->curr_map != 6 && g->curr_map != 7)
    {
        for (int k = 0; k < map->num_enemies; k++)
        {
            for (int i = 0; i < 1 + g->dificuldade; i++)
            {
                if (g->hero.pos.x - g->hero.pos.width <= map->enemies[k].pos.x - map->enemies[k].pos.width / 2 && g->hero.pos.x >= map->enemies[k].pos.x - map->enemies[k].pos.width / 2)
                {
                    if (!g->enemy[i].active && map->enemies[k].draw_enemy)
                    {
                        g->enemy[i].pos1.x = map->enemies[k].pos.x + map->enemies[k].pos.width / 2;
                        g->enemy[i].pos1.y = map->enemies[k].pos.y + map->enemies[k].pos.height / 2;

                        if (map->enemies[k].direction == KEY_UP && g->hero.pos.y < map->enemies[k].pos.y)
                        {
                            g->enemy[i].active = true;
                            g->enemy[i].speed.x = 0;
                            g->enemy[i].speed.y = -10;
                        }
                        else if (map->enemies[k].direction == KEY_DOWN && g->hero.pos.y > map->enemies[k].pos.y)
                        {
                            g->enemy[i].active = true;
                            g->enemy[i].speed.x = 0;
                            g->enemy[i].speed.y = +10;
                        }
                    }
                }
                if (g->hero.pos.y + g->hero.pos.height >= map->enemies[k].pos.y + map->enemies[k].pos.height / 2 && g->hero.pos.y <= map->enemies[k].pos.y + map->enemies[k].pos.height / 2)
                {
                    if (!g->enemy[i].active && map->enemies[k].draw_enemy)
                    {
                        g->enemy[i].pos1.x = map->enemies[k].pos.x + map->enemies[k].pos.width / 2;
                        g->enemy[i].pos1.y = map->enemies[k].pos.y + map->enemies[k].pos.height / 2;

                        if (map->enemies[k].direction == KEY_LEFT && g->hero.pos.x < map->enemies[k].pos.x)
                        {
                            g->enemy[i].active = true;
                            g->enemy[i].speed.x = -10;
                            g->enemy[i].speed.y = 0;
                        }
                        else if (map->enemies[k].direction == KEY_RIGHT && g->hero.pos.x > map->enemies[k].pos.x)
                        {
                            g->enemy[i].active = true;
                            g->enemy[i].speed.x = +10;
                            g->enemy[i].speed.y = 0;
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < 1 + g->dificuldade; i++)
    {
        if (g->enemy[i].active)
        {
            g->enemy[i].pos1.x += g->enemy[i].speed.x;
            g->enemy[i].pos1.y += g->enemy[i].speed.y;
            if (CheckCollisionRecs(g->enemy[i].pos1, g->hero.pos))
            {
                g->enemy[i].active = false;
                reinicia_mapa(g);
                reseta_tiro(g);
            }
            if (g->enemy[i].active)
            {

                // Verificar colisão do tiro com as barreiras
                if (barrier_collision(map, &g->enemy[i].pos1) && g->curr_map != 3 && g->curr_map != 5)
                {
                    g->enemy[i].active = false;
                }

                // Verificar se o tiro saiu da tela
                if (g->enemy[i].pos1.x + g->enemy[i].pos1.width < 0 || g->enemy[i].pos1.x > g->screenWidth)
                {
                    g->enemy[i].active = false;
                }

                if (g->enemy[i].pos1.y + g->enemy[i].pos1.height < 0 || g->enemy[i].pos1.y > g->screenHeight)
                {
                    g->enemy[i].active = false;
                }
            }
        }
    }
}

void update_enemy_pos(Game *g, Enemy *e)
{
    Map *m = &g->maps[g->curr_map];

    if (e->direction == KEY_LEFT)
    {
        if (e->pos.x > SCREEN_BORDER)
            e->pos.x -= e->speed;
        else
        {
            e->direction = KEY_RIGHT + (rand() % 4);
        }
        if (barrier_collision(m, &e->pos))
        {
            e->pos.x += e->speed;
            e->direction = KEY_RIGHT + (rand() % 4);
        }
    }
    else if (e->direction == KEY_RIGHT)
    {
        if (e->pos.x + e->pos.width < g->screenWidth - SCREEN_BORDER)
            e->pos.x += e->speed;
        else
            e->direction = KEY_RIGHT + (rand() % 4);

        if (barrier_collision(m, &e->pos))
        {
            e->pos.x -= e->speed;
            e->direction = KEY_RIGHT + (rand() % 4);
        }
    }
    else if (e->direction == KEY_UP)
    {
        if (e->pos.y > SCREEN_BORDER)
            e->pos.y -= e->speed;
        else
            e->direction = KEY_RIGHT + (rand() % 4);

        if (barrier_collision(m, &e->pos) && g->curr_map != 6)
        {
            e->pos.y += e->speed;
            e->direction = KEY_RIGHT + (rand() % 4);
        }
    }
    else if (e->direction == KEY_DOWN)
    {
        if (e->pos.y + e->pos.height < g->screenHeight - SCREEN_BORDER)
            e->pos.y += e->speed;
        else
            e->direction = KEY_RIGHT + (rand() % 4);

        if (barrier_collision(m, &e->pos) && g->curr_map != 6)
        {
            e->pos.y -= e->speed;
            e->direction = KEY_RIGHT + (rand() % 4);
        }
    }
    if (g->curr_map <= 1)
    {
        if (rand() % 100 == 1) // 1% de chance do inimigo mudar de direcao
            e->direction = KEY_RIGHT + (rand() % 4);
    }
    if (g->curr_map > 1 && g->curr_map < 7)
    {
        if (rand() % 50 == 1) // 2% de chance do inimigo mudar de direcao
            e->direction = KEY_RIGHT + (rand() % 4);
    }
    if (g->curr_map == 7)
    {
        if (rand() % 25 == 1) // 4% de chance do inimigo mudar de direcao
            e->direction = KEY_RIGHT + (rand() % 4);
    }
}

int barrier_collision(Map *map, Rectangle *target)
{
    for (int i = 0; i < map->num_barriers; i++)
    {
        if (CheckCollisionRecs(*target, map->barriers[i]))
        {
            return 1;
        }
    }
    return 0;
}

// Maps Setup
void map0_setup(Game *g)
{
    Map *map = &g->maps[0];
    map->color = GRAY;
    map->background.texture = LoadTexture("sprites/background.png");
    // Definindo as barreiras do mapa
    map->num_barriers = 5;
    map->barriers[0] = (Rectangle){g->screenWidth / 4, 0, 12, 0.8 * g->screenHeight};
    map->barriers[1] = (Rectangle){g->screenWidth / 2 + 110, g->screenHeight / 2 - 10, 50, 100};
    map->barriers[2] = (Rectangle){g->screenWidth * 0.84 - 110, g->screenHeight / 2 - 10, 50, 100};
    map->barriers[3] = (Rectangle){-43, g->screenHeight / 3, 150, 5};
    map->barriers[4] = (Rectangle){g->screenWidth * 0.64, g->screenHeight * 0.70, 0.1 * g->screenWidth, 50};

    // Definindo a porta de saída do mapa
    map->door = (Rectangle){g->screenWidth - (SCREEN_BORDER + 5), g->screenHeight / 3, SCREEN_BORDER, 100};
    map->door_locked = 1;

    // Definindo os inimigos do mapa
    map->num_enemies = 3;

    for (int i = 0; i < map->num_enemies; i++)
    {
        map->enemies[i].enemy_texture[0].texture = LoadTexture("sprites/badd.png");
        map->enemies[i].enemy_texture[1].texture = LoadTexture("sprites/bade.png");
        map->enemies[i].pos = (Rectangle){2 * g->screenWidth / 3, 2 * g->screenHeight / 4, 43, 60};
        map->enemies[i].color = RAYWHITE;
        map->enemies[i].speed = 2 + g->dificuldade*2;
        map->enemies[i].direction = KEY_RIGHT + (rand() % 4);
        map->enemies[i].draw_enemy = 1;
        map->enemies[i].has_key = 0;
    }

    // Definindo que o primeiro inimigo tem uma chave
    map->enemies[0].has_key = 1;

    // Definindo um item especial no mapa
    map->special_item = (Rectangle){2 * g->screenWidth / 3, 20, 15, 15};
    map->draw_special_item = 0;

    // Definindo o mapa anterior e próximo
    map->prev_map = -1;
    map->next_map = 1;
}

void map1_setup(Game *g)
{
    Map *map = &g->maps[1];
    map->background.texture = LoadTexture("sprites/background.png");
    map->num_barriers = 2;
    map->barriers[0] = (Rectangle){3 * g->screenWidth / 4, 0, 12, 0.6 * g->screenHeight};
    map->barriers[1] = (Rectangle){g->screenWidth / 4, 0.4 * g->screenHeight, 12, g->screenHeight};
    map->color = GRAY;
    map->door = (Rectangle){g->screenWidth - (SCREEN_BORDER + 5), g->screenHeight / 3, SCREEN_BORDER, 100};
    map->prev_door = (Rectangle){SCREEN_BORDER, g->screenHeight / 3, 5, 100};
    map->num_enemies = 10;
    map->door_locked = 1;

    for (int i = 0; i < map->num_enemies; i++)
    {
        map->enemies[i].enemy_texture[0].texture = LoadTexture("sprites/blued.png");
        map->enemies[i].enemy_texture[1].texture = LoadTexture("sprites/bluee.png");
        map->enemies[i].pos = (Rectangle){2 * g->screenWidth / 3, 2 * g->screenHeight / 3, 32, 22};
        map->enemies[i].color = RAYWHITE;
        map->enemies[i].speed = 8 + g->dificuldade;
        map->enemies[i].direction = KEY_RIGHT + (rand() % 4);
        map->enemies[i].draw_enemy = 1;
        map->enemies[i].has_key = 0;
    }

    map->enemies[0].has_key = 1;
    map->special_item = (Rectangle){2 * g->screenWidth / 3, 20, 15, 15};
    map->draw_special_item = 1;
    map->prev_map = 0;
    map->next_map = 2;
}

void map2_setup(Game *g)
{
    Map *map = &g->maps[2];
    map->background.texture = LoadTexture("sprites/background.png");
    map->num_barriers = 3;
    map->barriers[0] = (Rectangle){g->screenWidth / 4, 0, 12, 0.6 * g->screenHeight};
    map->barriers[1] = (Rectangle){2 * g->screenWidth / 4, g->screenHeight / 2, 12, 0.6 * g->screenHeight};
    map->barriers[2] = (Rectangle){3 * g->screenWidth / 4, 0, 12, 0.6 * g->screenHeight};
    map->color = GRAY;
    map->door = (Rectangle){g->screenWidth - (SCREEN_BORDER + 5), g->screenHeight / 3, SCREEN_BORDER, 100};
    map->prev_door = (Rectangle){SCREEN_BORDER, g->screenHeight / 3, 5, 100};
    map->num_enemies = 4;
    map->door_locked = 1;

    for (int i = 0; i < map->num_enemies; i++)
    {
        map->enemies[i].enemy_texture[0].texture = LoadTexture("sprites/badd.png");
        map->enemies[i].enemy_texture[1].texture = LoadTexture("sprites/bade.png");
        map->enemies[i].pos = (Rectangle){2 * g->screenWidth / 3, 2 * g->screenHeight / 3, 43, 60};
        map->enemies[i].color = RED;
        map->enemies[i].speed = 4 + g->dificuldade*2;
        map->enemies[i].direction = KEY_RIGHT + (rand() % 4);
        map->enemies[i].draw_enemy = 1;
        map->enemies[i].has_key = 0;
    }

    map->enemies[0].has_key = 1;
    map->special_item = (Rectangle){4 * g->screenWidth / 5, 20, 15, 15};
    map->draw_special_item = 0;
    map->prev_map = 1;
    map->next_map = 3;
}

void map3_setup(Game *g)
{
    Map *map = &g->maps[3];
    map->background.texture = LoadTexture("sprites/background.png");
    map->num_barriers = 3;
    map->barriers[0] = (Rectangle){g->screenWidth / 4, g->screenHeight / 2, 12, 500};
    map->barriers[1] = (Rectangle){2 * g->screenWidth / 4, 0, 12, 500};
    map->barriers[2] = (Rectangle){3 * g->screenWidth / 4, g->screenHeight / 2, 12, 500};
    map->color = GRAY;
    map->door = (Rectangle){g->screenWidth - (SCREEN_BORDER + 5), g->screenHeight / 3, SCREEN_BORDER, 100};
    map->prev_door = (Rectangle){SCREEN_BORDER, g->screenHeight / 3, 5, 100};
    map->num_enemies = 3;
    map->door_locked = 1;

    for (int i = 0; i < map->num_enemies; i++)
    {
        map->enemies[i].enemy_texture[0].texture = LoadTexture("sprites/cellsd.png");
        map->enemies[i].enemy_texture[1].texture = LoadTexture("sprites/cellse.png");
        map->enemies[i].pos = (Rectangle){2 * g->screenWidth / 3, 2 * g->screenHeight / 3, 70, 90};
        map->enemies[i].color = RAYWHITE;
        map->enemies[i].speed = 4 + g->dificuldade*2;
        map->enemies[i].direction = KEY_RIGHT + (rand() % 4);
        map->enemies[i].draw_enemy = 1;
        map->enemies[i].has_key = 0;
    }

    map->enemies[0].has_key = 1;
    map->special_item = (Rectangle){4 * g->screenWidth / 5, 20, 15, 15};
    map->draw_special_item = 0;
    map->prev_map = 2;
    map->next_map = 4;
}

void map4_setup(Game *g)
{
    Map *map = &g->maps[4];
    map->background.texture = LoadTexture("sprites/background.png");
    map->num_barriers = 1;
    map->barriers[0] = (Rectangle){g->screenWidth / 4, 0.5 * g->screenHeight, 0.6 * g->screenWidth, 12};

    map->color = GRAY;
    map->door = (Rectangle){g->screenWidth - (SCREEN_BORDER + 5), g->screenHeight / 3, SCREEN_BORDER, 100};
    map->prev_door = (Rectangle){SCREEN_BORDER, g->screenHeight / 3, 5, 100};
    map->num_enemies = 4;
    map->door_locked = 1;

    for (int i = 0; i < map->num_enemies; i++)
    {
        map->enemies[i].enemy_texture[0].texture = LoadTexture("sprites/dogd.png");
        map->enemies[i].enemy_texture[1].texture = LoadTexture("sprites/doge.png");
        map->enemies[i].pos = (Rectangle){2 * g->screenWidth / 3, 2 * g->screenHeight / 3, 100, 83};
        map->enemies[i].color = RAYWHITE;
        map->enemies[i].speed = 6 + g->dificuldade*2;
        map->enemies[i].direction = KEY_RIGHT + (rand() % 4);
        map->enemies[i].draw_enemy = 1;
        map->enemies[i].has_key = 0;
    }

    map->enemies[0].has_key = 1;
    map->special_item = (Rectangle){4 * g->screenWidth / 5, 20, 15, 15};
    map->draw_special_item = 0;
    map->prev_map = 3;
    map->next_map = 5;
}

void map5_setup(Game *g)
{
    Map *map = &g->maps[5];
    map->background.texture = LoadTexture("sprites/background.png");
    map->num_barriers = 2;
    map->barriers[0] = (Rectangle){g->screenWidth / 2 - 400, g->screenHeight / 2 - 225, 12, 425};
    map->barriers[1] = (Rectangle){g->screenWidth / 2 + 400, g->screenHeight / 2 - 225, 12, 425};

    map->color = GRAY;
    map->door = (Rectangle){g->screenWidth - (SCREEN_BORDER + 5), g->screenHeight / 3, SCREEN_BORDER, 100};
    map->prev_door = (Rectangle){SCREEN_BORDER, g->screenHeight / 3, 5, 100};
    map->num_enemies = 4;
    map->door_locked = 1;

    for (int i = 0; i < map->num_enemies; i++)
    {
        map->enemies[i].enemy_texture[0].texture = LoadTexture("sprites/cellsd.png");
        map->enemies[i].enemy_texture[1].texture = LoadTexture("sprites/cellse.png");
        map->enemies[i].pos = (Rectangle){2 * g->screenWidth / 3, 2 * g->screenHeight / 3, 70, 90};
        map->enemies[i].color = RED;
        map->enemies[i].speed = 6 + g->dificuldade*2;
        map->enemies[i].direction = KEY_RIGHT + (rand() % 4);
        map->enemies[i].draw_enemy = 1;
        map->enemies[i].has_key = 0;
    }

    map->enemies[0].has_key = 1;
    map->special_item = (Rectangle){4 * g->screenWidth / 5, 20, 15, 15};
    map->draw_special_item = 0;
    map->prev_map = 4;
    map->next_map = 6;
}

void map6_setup(Game *g)
{
    Map *map = &g->maps[6];
    map->background.texture = LoadTexture("sprites/background.png");
    map->num_barriers = 2;
    map->barriers[0] = (Rectangle){g->screenWidth / 2 - 700, g->screenHeight / 2, 500, 12};
    map->barriers[1] = (Rectangle){g->screenWidth / 2 + 200, g->screenHeight / 2, 500, 12};

    map->color = GRAY;
    map->door = (Rectangle){g->screenWidth - (SCREEN_BORDER + 5), g->screenHeight / 3, SCREEN_BORDER, 100};
    map->prev_door = (Rectangle){SCREEN_BORDER, g->screenHeight / 3, 5, 100};
    map->num_enemies = 5;
    map->door_locked = 1;

    for (int i = 0; i < map->num_enemies; i++)
    {
        map->enemies[i].enemy_texture[0].texture = LoadTexture("sprites/dogd.png");
        map->enemies[i].enemy_texture[1].texture = LoadTexture("sprites/doge.png");
        map->enemies[i].pos = (Rectangle){2 * g->screenWidth / 3, 2 * g->screenHeight / 3, 100, 83};
        map->enemies[i].color = BLUE;
        map->enemies[i].speed = 6 + g->dificuldade*2;
        map->enemies[i].direction = KEY_RIGHT + (rand() % 4);
        map->enemies[i].draw_enemy = 1;
        map->enemies[i].has_key = 0;
    }

    map->enemies[0].has_key = 1;
    map->special_item = (Rectangle){4 * g->screenWidth / 5, 20, 15, 15};
    map->draw_special_item = 0;
    map->prev_map = 5;
    map->next_map = 7;
}

void map7_setup(Game *g)
{
    Map *map = &g->maps[7];
    
    map->background.texture = LoadTexture("sprites/back2.png");

    map->num_barriers = 0;

    map->color = GRAY;
    map->num_enemies = 1;

    map->enemies[0].enemy_texture[0].texture = LoadTexture("sprites/bossd.png");
    map->enemies[0].enemy_texture[1].texture = LoadTexture("sprites/bosse.png");
    map->enemies[0].pos = (Rectangle){g->screenWidth - 300, g->screenHeight / 2 - 200, 354, 500};
    map->enemies[0].color = RAYWHITE;
    map->enemies[0].speed = 3 + g->dificuldade;
    map->enemies[0].direction = KEY_RIGHT + (rand() % 4);
    map->enemies[0].draw_enemy = 1;
    map->enemies[0].has_key = 0;

    map->special_item = (Rectangle){g->screenWidth - 100, g->screenHeight / 2, 15, 15};
    map->draw_special_item = 1;
}

void reinicia_mapa(Game *g)
{
    switch (g->curr_map)
    {
    case 0:
        map0_setup(g);
        g->hero.pos = (Rectangle){15, 15, 49, 111};
        break;
    case 1:
        map1_setup(g);
        g->hero.pos = (Rectangle){15, 15, 49, 111};
        break;
    case 2:
        map2_setup(g);
        g->hero.pos = (Rectangle){15, 15, 49, 111};
        break;
    case 3:
        map3_setup(g);
        g->hero.pos = (Rectangle){15, 15, 49, 111};
        break;
    case 4:
        map4_setup(g);
        g->hero.pos = (Rectangle){15, 15, 49, 111};
        break;
    case 5:
        map5_setup(g);
        g->hero.pos = (Rectangle){15, 15, 49, 111};
        break;
    case 6:
        map6_setup(g);
        g->hero.pos = (Rectangle){15, 15, 49, 111};
        break;
    case 7:
        g->gameover = 1;
        break;
    }
}
void reseta_tiro(Game *g)
{
    for (int i = 0; i < 2 - g->dificuldade; i++)
    {
        g->shoot[i].active = false;
    }
    for (int i = 0; i < 1 + g->dificuldade; i++)
    {
        g->enemy[i].active = false;
    }
}

void carregaPosicoes(Jogador jogadores[])
{
    FILE *placar = fopen("pontuacoes.txt", "r");
    int linhaDesejada = 1;
    char linha[MAX_CHARS];
    int numeroLinha = 1;
    int i = 0;

    while (fscanf(placar, "%[^\n] ", linha) != EOF)
    {
        if (numeroLinha % 2 == 1)
        {
            strcpy(jogadores[i].nome, linha);
        }
        if (numeroLinha % 2 == 0)
        {
            jogadores[i].pontuacao = atoi(linha);
            i++;
        }
        printf("> %s\n", linha);
        numeroLinha++;
    }
    fclose(placar);
}

void salvarPosicoes(Jogador jogadores[])
{
    FILE *placar = fopen("pontuacoes.txt", "w");

    // Comparação entre as pontuações dos jogadores
    if (jogadores[3].pontuacao < jogadores[2].pontuacao)
    {
        // Troca das pontuações
        int tempPontuacao = jogadores[2].pontuacao;
        jogadores[2].pontuacao = jogadores[3].pontuacao;
        jogadores[3].pontuacao = tempPontuacao;

        // Troca dos nomes correspondentes
        char tempNome[11];
        strcpy(tempNome, jogadores[2].nome);
        strcpy(jogadores[2].nome, jogadores[3].nome);
        strcpy(jogadores[3].nome, tempNome);
    }

    if (jogadores[2].pontuacao < jogadores[1].pontuacao)
    {
        int tempPontuacao = jogadores[1].pontuacao;
        jogadores[1].pontuacao = jogadores[2].pontuacao;
        jogadores[2].pontuacao = tempPontuacao;

        char tempNome[11];
        strcpy(tempNome, jogadores[1].nome);
        strcpy(jogadores[1].nome, jogadores[2].nome);
        strcpy(jogadores[2].nome, tempNome);
    }

    if (jogadores[1].pontuacao < jogadores[0].pontuacao)
    {
        int tempPontuacao = jogadores[0].pontuacao;
        jogadores[0].pontuacao = jogadores[1].pontuacao;
        jogadores[1].pontuacao = tempPontuacao;

        char tempNome[11];
        strcpy(tempNome, jogadores[0].nome);
        strcpy(jogadores[0].nome, jogadores[1].nome);
        strcpy(jogadores[1].nome, tempNome);
    }
    fprintf(placar, "%s\n%d\n", jogadores[0].nome, jogadores[0].pontuacao);
    fprintf(placar, "%s\n%d\n", jogadores[1].nome, jogadores[1].pontuacao);
    fprintf(placar, "%s\n%d\n", jogadores[2].nome, jogadores[2].pontuacao);

    fclose(placar);
}
