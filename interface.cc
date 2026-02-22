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

#include "colisiones.h"

// TODO(@jhony): Optimize this scripts
extern const int kScreenWidth;
extern const int kScreenHeight; 

enum TScreen {
  IMAGE,
  MAIN_MENU,
  GAME_SCREEN
};

struct TPlatform {
  // esat::Vec2 pos;
  COL::object collision_platform;
  int size;
};

// (@jhony) check this
struct TGame {
  TScreen current_screen;
  unsigned char current_lives;
  int hi_socore;
  int score_p1; // actual hi-score for player1
  int score_p2; // actual hi-score for player2
};

// test
const unsigned char kplatform_numbers = 3;

// esat::SpriteHandle* platform_sprite = nullptr;
// esat::SpriteHandle* loading_sprite = nullptr;
// TPlatform* g_platforms = nullptr;

TGame game_data;

// TODO: move top
float timer = 0;

int menu_selection_player = 0;   /* 0 = 1 player, 1 = 2 player */
int menu_selection_control = 0;   /* 0 = keyboard, 1 = kempston */
float menu_blink_timer = 0.0f;
bool menu_highlight_white = true;

esat::SpriteHandle sprite_lives;

void InitLivesSprite(){
  printf("[DEBUG] call InitLivesSprite\n");
  sprite_lives = esat::SpriteFromFile("SPRITES/JUGADOR/vidas_2x.png");
  printf("[DEBUG] end InitLivesSprite\n");
}

void InitPlatforms(TPlatform** g_platforms, esat::SpriteHandle* platform_sprite){
  printf("[DEBUG] call InitPlatforms\n");
  if (*g_platforms != nullptr) return;
  int w = esat::SpriteWidth(*(platform_sprite + 0));
  int h = esat::SpriteHeight(*(platform_sprite + 0));
  *g_platforms = (TPlatform*)malloc(kplatform_numbers * sizeof(TPlatform));

  // (@jhony) TODO: fix this
  (*g_platforms)[0] = { nullptr, w, h, 70.0f, 150.0f, 70.0f, 150.0f, 70.0f + w*6, 150.0f + h, 6 };
  (*g_platforms)[1] = { nullptr, w, h, 240.0f, 180.0f, 240.0f, 180.0f, 240.0f + w*4, 180.0f + h, 4 };
  (*g_platforms)[2] = { nullptr, w, h, 390.0f, 100.0f, 390.0f, 100.0f, 390.0f + w*6, 100.0f + h, 6 };
  
  printf("[DEBUG] end InitPlatforms\n");
}

void ReserveMemory(esat::SpriteHandle** platform_sprite){
  printf("[DEBUG] sizeof(esat::SpriteHandle) %d\n", sizeof(esat::SpriteHandle));
  *platform_sprite = (esat::SpriteHandle*)malloc(4 * sizeof(esat::SpriteHandle));
  if (*platform_sprite == nullptr){
    printf("Error: reserve memory\n");
  }else{
    printf("All ok\n");
  }
}

void InitGameVariables(){
  game_data = {IMAGE,0,0,0,0};
}

void InitLoadingSprites(esat::SpriteHandle** loading_sprite){
  *loading_sprite = (esat::SpriteHandle*)malloc(2 * sizeof(esat::SpriteHandle));
  *(*loading_sprite + 0) = esat::SpriteFromFile("assets/sprites/loader/jetpac_frame1.png");
  *(*loading_sprite + 1) = esat::SpriteFromFile("assets/sprites/loader/jetpac_frame2.png");
}

void InitPlatformSprites(esat::SpriteHandle** platform_sprite, TPlatform** g_platforms){
  ReserveMemory(platform_sprite);

  *(*platform_sprite + 0) = esat::SpriteFromFile("./SPRITES/TERRENO/terreno_1_2x.png"); // First platform_sprite
  *(*platform_sprite + 1) = esat::SpriteFromFile("./SPRITES/TERRENO/terreno_2_2x.png"); // Middle platform_sprite
  *(*platform_sprite + 2) = esat::SpriteFromFile("./SPRITES/TERRENO/terreno_3_2x.png"); // Last platform_sprite

  for (int i = 0; i < 3;  ++i){
    if (*(*platform_sprite + i) == nullptr){
      printf("Error: on sprite %d\n", i);
    }
  }
  InitPlatforms(g_platforms, *platform_sprite);
}

void LoadFonts(){
  // printf("[DEBUG] call LoadFonts\n");
  esat::DrawSetTextFont("assets/fonts/zx_spectrum-7.ttf");
}

void UpdateInterface(int *score, int *lives, int *player_id){
  game_data.current_lives = *lives;
  if (*player_id == 1){
    game_data.score_p1 = *score;
  }else{
    game_data.score_p2 = *score;
  }
  if (game_data.score_p1 > game_data.hi_socore){
    game_data.hi_socore = game_data.score_p1;
  }
  if (game_data.score_p2 > game_data.hi_socore){
    game_data.hi_socore = game_data.score_p2;
  }
}

void DrawHeader(int lives = 0){
  char* score_1up = (char*)malloc(7 * sizeof(char));
  char* hi_score = (char*)malloc(7 * sizeof(char));
  char* score_2up = (char*)malloc(7 * sizeof(char));
  char* lives_text = (char*)malloc(2 * sizeof(char));
  itoa(game_data.score_p1 + 1000000, score_1up, 10);
  itoa(game_data.hi_socore + 1000000, hi_score, 10);
  itoa(game_data.score_p2 + 1000000, score_2up, 10);
  itoa(lives, lives_text, 10);

  esat::DrawSetTextSize(18);
  
  esat::DrawSetFillColor(255, 255, 255, 255);
  esat::DrawText(30, 16, "1UP");
  esat::DrawText(438, 16, "2UP");
  
  esat::DrawSetFillColor(0, 255, 255, 255);
  esat::DrawText(240, 16, "HI"); 
  
  esat::DrawSetFillColor(208, 208, 0, 255);
  esat::DrawText(10, 32, score_1up + 1); // score 1up placeholder
  esat::DrawText(210, 32, hi_score + 1); // Hi-Score placeholder
  esat::DrawText(413, 32, score_2up + 1); // score 2up placeholder

  // TODO @jhony: fix this
  if (lives > 0){
    esat::DrawSetFillColor(255, 255, 255, 255);
    esat::DrawText(115, 16, lives_text);
    // Draw bg white and sprite to the right of the number (aligned at top)
    float sprite_x = 120 + esat::SpriteWidth(sprite_lives);
    float sprite_y = 2;
    esat::DrawSetFillColor(255, 255, 255, 255);
    float* pts = (float*)malloc(8 * sizeof(float));
    *(pts + 0) = sprite_x;      *(pts + 1) = sprite_y;
    *(pts + 2) = sprite_x + esat::SpriteWidth(sprite_lives); *(pts + 3) = sprite_y;
    *(pts + 4) = sprite_x + esat::SpriteWidth(sprite_lives); *(pts + 5) = sprite_y + esat::SpriteHeight(sprite_lives);
    *(pts + 6) = sprite_x;      *(pts + 7) = sprite_y + esat::SpriteHeight(sprite_lives);
    esat::DrawSolidPath(pts, 4);
    free(pts);
    esat::DrawSprite(sprite_lives, sprite_x, sprite_y);
  }

  free(score_1up);
  free(hi_score);
  free(score_2up);
}

void DrawHighlightRect(float x, float y, float w, float h, bool white) {
  float* pts = (float*)malloc(8 * sizeof(float));
  *(pts + 0) = x;     *(pts + 1) = y;
  *(pts + 2) = x + w; *(pts + 3) = y;
  *(pts + 4) = x + w; *(pts + 5) = y + h;
  *(pts + 6) = x;     *(pts + 7) = y + h;
  esat::DrawSetStrokeColor(0, 0, 0, 0);
  esat::DrawSetFillColor(white ? 255 : 0, white ? 255 : 0, white ? 255 : 0, 255);
  esat::DrawSolidPath(pts, 4);
  free(pts);
}

void MainMenu(int selected_player, int selected_control, bool highlight_white) {
  DrawHeader();

  esat::DrawSetTextSize(20);
  esat::DrawSetFillColor(255, 255, 255);
  esat::DrawText(60, 70, "JETPAC GAME SELECTION");

  const int num_rows = 5;
  float* y_pos = (float*)malloc(num_rows * sizeof(float));
  *(y_pos + 0) = 110.0f;
  *(y_pos + 1) = 140.0f;
  *(y_pos + 2) = 170.0f;
  *(y_pos + 3) = 200.0f;
  *(y_pos + 4) = 250.0f;

  const char** rows = (const char**)malloc(num_rows * sizeof(const char*));
  *(rows + 0) = "1  1 PLAYER GAME";
  *(rows + 1) = "2  2 PLAYER GAME";
  *(rows + 2) = "3  KEYBOARD";
  *(rows + 3) = "4  KEMPSTON JOYSTICK";
  *(rows + 4) = "5  START GAME";

  esat::DrawSetTextSize(20);
  for (int i = 0; i < num_rows; ++i) {
    float y = *(y_pos + i);
    int is_highlighted = (i <= 1 && i == selected_player) || (i >= 2 && i <= 3 && (i - 2) == selected_control);
    if (is_highlighted) {
      DrawHighlightRect(70.0f - 4, y - 18, 320.0f, 22.0f, highlight_white);
      esat::DrawSetFillColor(highlight_white ? 0 : 255, highlight_white ? 0 : 255, highlight_white ? 0 : 255, 255);
    } else {
      esat::DrawSetFillColor(255, 255, 255, 255);
    }
    esat::DrawText(70.0f, y, *(rows + i));
  }
  free(y_pos);
  free(rows);

  esat::DrawSetFillColor(255, 255, 255, 255);
  esat::DrawSetTextSize(18);
  esat::DrawText(10, 360, "@1983 A.C.G. ALL RIGHTS RESERVED");
}

void GeneratePlatform(TPlatform* g_platforms, esat::SpriteHandle* platform_sprite){ 
  if (g_platforms == nullptr) return;
  int w = esat::SpriteWidth(*(platform_sprite + 0));
  int h = esat::SpriteHeight(*(platform_sprite + 0));

  float* bg = (float*)malloc(8 * sizeof(float));

  for (int i = 0; i < kplatform_numbers; ++i){
    TPlatform* p = (g_platforms + i);

    *(bg + 0) = p->collision_platform.position.x;
    *(bg + 1) = p->collision_platform.position.y;
    *(bg + 2) = p->collision_platform.position.x + (w * p->size);
    *(bg + 3) = p->collision_platform.position.y;
    *(bg + 4) = p->collision_platform.position.x + (w * p->size);
    *(bg + 5) = p->collision_platform.position.y + h;
    *(bg + 6) = p->collision_platform.position.x;
    *(bg + 7) = p->collision_platform.position.y + h;

    esat::DrawSetStrokeColor(0, 0, 0, 0); // hide borders
    esat::DrawSetFillColor(0, 255, 0, 255);
    esat::DrawSolidPath(bg, 4);
    // esat::DrawSolidPath(p->collision_platform.colision, 4);

    // Draw platforms
    for (int j = 0; j < p->size; ++j){
      if (j == 0){
        esat::DrawSprite(*(platform_sprite + 0), p->collision_platform.position.x, p->collision_platform.position.y);
      }else if (j == p->size - 1){
        esat::DrawSprite(*(platform_sprite + 2), p->collision_platform.position.x + (j * w), p->collision_platform.position.y);
      }else{
        esat::DrawSprite(*(platform_sprite + 1), p->collision_platform.position.x + (j * w), p->collision_platform.position.y);
      }
    }
  }

  free(bg);
}

void GenerateFloor(esat::SpriteHandle* platform_sprite){
  if (platform_sprite == nullptr) return;
  float* floor = (float*)malloc(8 * sizeof(float));
  float h = esat::SpriteHeight(*(platform_sprite + 0));

  *(floor + 0) = 0.0f;
  *(floor + 1) = (float)(kScreenHeight - (int)h);
  *(floor + 2) = (float)kScreenWidth;
  *(floor + 3) = (float)(kScreenHeight - (int)h);
  *(floor + 4) = (float)kScreenWidth;
  *(floor + 5) = (float)kScreenHeight;
  *(floor + 6) = 0.0f;
  *(floor + 7) = (float)kScreenHeight;

  esat::DrawSetStrokeColor(0, 0, 0, 0); // hide borders
  esat::DrawSetFillColor(208, 208, 0, 255);
  esat::DrawSolidPath(floor, 4);

  for (int j = 0; j < kScreenWidth/esat::SpriteWidth(*(platform_sprite + 0)); ++j){
    esat::DrawSprite(*(platform_sprite + 0), j * esat::SpriteWidth(*(platform_sprite + 0)), kScreenHeight - h);
  }

  free(floor);
}

// Basic game screen
void GameScreen(TPlatform* g_platforms, esat::SpriteHandle* platform_sprite, int lives = 0){
  DrawHeader(lives);
  GeneratePlatform(g_platforms, platform_sprite);
  GenerateFloor(platform_sprite);
}

// Update here

void InitialImage(esat::SpriteHandle* loading_sprite){
  esat::DrawSprite(*(loading_sprite + 0), 0, 0);
}

// function to select the screen to show
void ScreenSelector(float dt, TPlatform* g_platforms, esat::SpriteHandle* platform_sprite, esat::SpriteHandle* loading_sprite) {
  switch (game_data.current_screen) {
    case IMAGE:
      timer += dt;
      if (timer >= 5.0f) game_data.current_screen = MAIN_MENU;
      InitialImage(loading_sprite);
      break;
    case MAIN_MENU: {
      menu_blink_timer += dt;
      if (menu_blink_timer >= 0.5f) {
        menu_blink_timer = 0.0f;
        menu_highlight_white = !menu_highlight_white;
      }
      if (esat::IsKeyPressed('1')) menu_selection_player = 0;
      if (esat::IsKeyPressed('2')) menu_selection_player = 1;
      if (esat::IsKeyPressed('3')) menu_selection_control = 0;
      if (esat::IsKeyPressed('4')) menu_selection_control = 1;
      if (esat::IsKeyPressed('5')) game_data.current_screen = GAME_SCREEN;
      MainMenu(menu_selection_player, menu_selection_control, menu_highlight_white);
      break;
    }
    case GAME_SCREEN:
      GameScreen(g_platforms, platform_sprite);
      break;
    default:
      break;
  }
}

void TestMousePosition(){
  if(esat::MouseButtonDown(0))
    printf("[DEBUG] Mouse position: %f, %f\n", (float) esat::MousePositionX(), (float) esat::MousePositionY());


  
}

void Testers(){

}