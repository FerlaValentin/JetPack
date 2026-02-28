#ifndef INTERFACE_H
#define INTERFACE_H

#include <esat/sprite.h>
#include "colisiones.h"

struct TPlatform
{
    // esat::Vec2 pos;
    COL::object collision_platform;
    int size;
};

enum TScreen
{
    IMAGE,
    MAIN_MENU,
    GAME_SCREEN,
    GAME_OVER
};

// (@jhony) check this
struct TGame
{
    TScreen current_screen;
    unsigned char current_lives;
    unsigned char current_player_id;
    int hi_socore;
    int score_p1; // actual hi-score for player1
    int score_p2; // actual hi-score for player2
    float label_timer_blink;
};

void InitialImage(esat::SpriteHandle *loading_sprite);
void InitLivesSprite(esat::SpriteHandle *sprite_lives);
void InitPlatforms(TPlatform **g_platforms, esat::SpriteHandle *platform_sprite);
void InitGameVariables(TGame *game_data);
void InitLoadingSprites(esat::SpriteHandle **loading_sprite);
void InitPlatformSprites(esat::SpriteHandle **platform_sprite, TPlatform **g_platforms);
void LoadFonts();
void ScreenSelector(TGame *game, float *timer, float *menu_blink_timer, bool *menu_highlight_white);
void UpdateInterface(int *score, int *lives, int *player_id, TGame *game_data);
void MainMenu(int selected_player, int selected_control, bool highlight_white, TGame game_data, esat::SpriteHandle sprite_lives);
void GameScreen(TPlatform *g_platforms, esat::SpriteHandle *platform_sprite, TGame game_data, esat::SpriteHandle sprite_lives, int lives);
void GameOverScreen(TGame* game_data, double dt);

#endif