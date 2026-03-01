#ifndef MERGE_H
#define MERGE_H
// ESAT Libraries
#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// FPS
const int kScreenWidth = 512;
const int kScreenHeight = 384;
extern double delta_time;

// #include "colisiones.h"
// #include "nave.cc"
// #include "enemigos.h"
// #include "jugador.cc"
#include "interface.h"
#include "colisiones.h"
#include "nave.h"
#include "enemigos.h"
#include "jugador.h"
#include "audio.h"
#include "data.h"

void InitiateFrame();

void InitiateAll(Sprites **spritesColores, Sprites **spritesPersonaje, Bala **punteroBalas, Sprites **spritesItems, Jugador *player,
                 ItemDrop *gasofa, ItemDrop *itemdrop, esat::SpriteHandle **platform_sprite, TPlatform **g_platforms,
                 esat::SpriteHandle **loading_sprite, TGame *game, esat::SpriteHandle *sprite_lives, Sprites **spritesNave, Nave *nave,
                 ENE::EnemyManager **mgr, ENE::VisualEffect **g_fx_pool, esat::SpriteHandle **g_fx_sprites, ParteNave **punteroParteNave);

void GetInput(bool *moverLeft, bool *moverRight, bool *ascender, Bala *punteroBalas, Jugador *player,
              TGame *game, int *menu_selection_player, int *menu_selection_control, Nave *nave, ItemDrop *itemdrop, ItemDrop *gasofa, Sprites *spritesItems,
              ENE::EnemyManager *mgr, ParteNave *punteroParteNave);

// Solo para testear luego se borra
void TestValues(Jugador *player);

void Update(Jugador *player, bool *ascender, Bala *punteroBalas, bool *moverLeft, bool *moverRight, int *frame,
            ItemDrop &gasofa, ItemDrop *itemdrop, Sprites *spritesItems, TPlatform *g_platforms,
            TGame *game, float *timer, float *menu_blink_timer, bool *menu_highlight_white, Nave *nave,
            ENE::EnemyManager *mgr, int level, ParteNave *punteroParteNave, ENE::VisualEffect *g_fx_pool, esat::SpriteHandle *g_fx_sprites);

void DrawAll(Sprites *spritesColores, Sprites *spritesPersonaje, Bala *punteroBalas, Jugador player, int frame,
             ItemDrop gasofa, Sprites *spritesItems, ItemDrop itemdrop, TPlatform *g_platforms, esat::SpriteHandle *platform_sprite,
             TGame game, esat::SpriteHandle *loading_sprite, int menu_selection_player, int menu_selection_control, int menu_highlight_white, esat::SpriteHandle sprite_vidas,
             Nave *nave, Sprites *spritesNave, ENE::EnemyManager mgr, ENE::VisualEffect *g_fx_pool, esat::SpriteHandle *g_fx_sprites, ParteNave *punteroNave);

void FinishFrame();

void FreeAll(Sprites **spritesColores, Sprites **spritesPersonaje, Sprites **spritesItem, Bala **punteroBalas,
             ItemDrop *gasofa, ItemDrop *itemdrop, ENE::EnemyManager **mgr, ENE::VisualEffect **g_fx_pool_pointer, esat::SpriteHandle **g_fx_sprites_pointer);
#endif