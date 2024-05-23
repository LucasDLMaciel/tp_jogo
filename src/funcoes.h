#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define STD_SIZE_X 66
#define STD_SIZE_Y 52
#define SCREEN_BORDER 5
#define MAX_TIROS 2
#define MAX_CHARS 11

typedef struct Sprite
{
    Texture2D texture;
} Sprite;

typedef struct Jogador
{
    char nome[MAX_CHARS + 1];
    int pontuacao;
} Jogador;

typedef struct Hero
{
    Rectangle pos;
    Color color;
    int speed;
    int special;
    int direction;
    Sprite hero_texture[2];
    Texture2D hero_texture_right;
    Texture2D hero_texture_left;
    char nome[MAX_CHARS + 1];
} Hero;

typedef struct Enemy
{
    Rectangle pos;
    Color color;
    int speed;
    int direction;
    int draw_enemy;
    int has_key;
    Sprite enemy_texture[2];
    Texture2D enemy_texture_right;
    Texture2D enemy_texture_left;
    Sound som_explosion;
} Enemy;

typedef struct Map
{
    Sprite background;
    Rectangle barriers[10];
    int num_barriers;
    Rectangle door;
    Rectangle prev_door;
    Color color;
    Enemy enemies[10];
    int num_enemies;
    Rectangle special_item;
    int draw_special_item;
    int door_locked;
    int next_map;
    int prev_map;
    int dead_enemies;
} Map;

typedef struct Shoot
{
    Rectangle pos1;
    Vector2 speed;
    Color color;
    bool active;
    float radius;
    int direction;
    Texture2D shoot_texture;
} Shoot;

typedef struct Key
{
    Sprite key;
    Rectangle key_pos;
} Key;

typedef struct Game
{
    Map maps[10];
    int num_maps;
    int curr_map;
    Hero hero;
    int screenWidth;
    int screenHeight;
    int gameover;
    Shoot shoot[MAX_TIROS];
    Shoot enemy[MAX_TIROS];
    Key key;
    int dificuldade;
    Sprite special[2];
    Texture2D victory;
    Vector2 creditos;
    Jogador info[5];
    Sound som_tiro;
    Sound som_rede;
    Sound item;
} Game;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
void InitGame(Game *g);        // Initialize game
void UpdateGame(Game *g);      // Update game (one frame)
void DrawGame(Game *g);        // Draw game (one frame)
void UpdateDrawFrame(Game *g); // Update and Draw (one frame)
void salvarPosicoes(Jogador jogadores[]);
void carregaPosicoes(Jogador jogadores[]);

//------------------------------------------------------------------------------------
// Auxiliar Functions Declaration
//------------------------------------------------------------------------------------
void draw_borders(Game *g);
void draw_map(Game *g);
void update_enemy_pos(Game *g, Enemy *e);
void update_hero_pos(Game *g);
void update_tiro_pos(Game *g);
void update_tiro_enemy_pos(Game *g);

int barrier_collision(Map *m, Rectangle *t);
void map0_setup(Game *g);
void map1_setup(Game *g);
void map2_setup(Game *g);
void map3_setup(Game *g);
void map4_setup(Game *g);
void map5_setup(Game *g);
void map6_setup(Game *g);
void map7_setup(Game *g);
void reinicia_mapa(Game *g);
void reseta_tiro(Game *g);