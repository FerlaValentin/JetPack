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
#include "jugador.cc"

// Windows dimensions with default values
const int KWindow_Width = 800;
const int KWindow_Height = 800;

// FPS
double delta_time;
unsigned char fps = 25;
double current_time;
double last_time;

enum Direction{
  UP,
  DOWN,
  NONE
};

struct Sprite{
  esat::SpriteHandle sprite;
};

struct Nave{
    float vel;
    esat::Vec2 pos;
    int direccion;
    bool activada;
};

// void ReservaMemoria(){
//     spritesNave = (nave*) malloc(sizeof(nave) * 9);
// }

// void SpritesNave(){
//     (spritesNave) = 
// }

Sprite *InstanciarSpritesNave(int numSprite){
  return (Sprite*)malloc(sizeof(Sprite)*numSprite);
}

void InitSpriteNave(Sprite *punteroSprites)
{
  punteroSprites[0].sprite = esat::SpriteFromFile("./SPRITES/nave_cabeza_1_2x.png");
  punteroSprites[1].sprite = esat::SpriteFromFile("./SPRITES/nave_cabeza_1_2x.png");
  punteroSprites[2].sprite = esat::SpriteFromFile("./SPRITES/nave_cabeza_1_2x.png");
  punteroSprites[3].sprite = esat::SpriteFromFile("./SPRITES/nave_cabeza_1_2x.png");
  punteroSprites[4].sprite = esat::SpriteFromFile("./SPRITES/nave_cabeza_1_2x.png");
  punteroSprites[5].sprite = esat::SpriteFromFile("./SPRITES/nave_cabeza_1_2x.png");
  punteroSprites[6].sprite = esat::SpriteFromFile("./SPRITES/nave_cabeza_1_2x.png");
  punteroSprites[7].sprite = esat::SpriteFromFile("./SPRITES/nave_cabeza_1_2x.png");
  punteroSprites[8].sprite = esat::SpriteFromFile("./SPRITES/nave_cabeza_1_2x.png");
  punteroSprites[9].sprite = esat::SpriteFromFile("./SPRITES/rosita.png");
  punteroSprites[10].sprite = esat::SpriteFromFile("./SPRITES/blanquito.png");
}

void InstanciarNave(Nave *nave)
{
  int height = 32;

  nave->activada = false;
  nave->direccion = Direction::NONE;
  nave->pos.x = KWindow_Width/2;
  nave->pos.y = KWindow_Height-height;
}

// void DibujarNave()
//{
  // esat::DrawSprite(punteroSprites[0], pos x, pos y);
//}
// mover nave (trigger nave)



int esat::main(int argc, char **argv) {

	esat::WindowInit(KWindow_Width, KWindow_Height);
	esat::WindowSetMouseVisibility(true);

	srand((unsigned)time(nullptr));
	last_time = esat::Time();

  Sprite *punteroSprites = InstanciarSpritesNave(11);
  InitSpriteNave(punteroSprites);

  Nave nave;
  InstanciarNave(&nave);



  // Main game loop
  while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
    
    // Calculate time elapsed since the last frame
    current_time = esat::Time();
    delta_time = (current_time - last_time) / 1000.0;
    // Limit delta_time to avoid large jumps
    if (delta_time > 0.1) delta_time = 0.1;
    
    last_time = current_time;

    esat::DrawBegin();
    esat::DrawClear(0,0,0);



    // Finish drawing
    esat::DrawEnd();
    // Control frame speed (fps)
    do{
      current_time = esat::Time();
    }while((current_time - last_time) <= 1000.0/fps);
    esat::WindowFrame();
  }

  // Destroy window
  esat::WindowDestroy();
  return 0;
}
