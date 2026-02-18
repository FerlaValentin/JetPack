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
};

struct Sprite{
  esat::SpriteHandle sprite;
};

struct Nave{
    float vel;
    esat::Vec2 pos;
    Direction direccion;
    bool activada;
    int height = 32;
};

Sprite *InstanciarSpritesNave(int numSprite){
  return (Sprite*)malloc(sizeof(Sprite)*numSprite);
}

void InitSpriteNave(Sprite *punteroSprites)
{
  punteroSprites[0].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_1_2x.png");
  punteroSprites[1].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_1_2x.png");
  punteroSprites[2].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_1_2x.png");
  punteroSprites[3].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_2_2x.png");
  punteroSprites[4].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_2_2x.png");
  punteroSprites[5].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_2_2x.png");
  punteroSprites[6].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_3_2x.png");
  punteroSprites[7].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_3_2x.png");
  punteroSprites[8].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_3_2x.png");
  punteroSprites[9].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_4_2x.png");
  punteroSprites[10].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_4_2x.png");
  punteroSprites[11].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_4_2x.png");
  punteroSprites[12].sprite = esat::SpriteFromFile("./SPRITES/NAVE/fondo_blanco.png");
  punteroSprites[13].sprite = esat::SpriteFromFile("./SPRITES/NAVE/explosion_1_2x.png");
  punteroSprites[14].sprite = esat::SpriteFromFile("./SPRITES/NAVE/explosion_2_2x.png");
  //punteroSprites[14].sprite = esat::SpriteFromFile("./SPRITES/rosita.png");
}

void InstanciarNave(Nave *nave)
{
  nave->activada = false;
  nave->vel = 10;
  nave->pos.x = KWindow_Width/2;
  nave->pos.y = KWindow_Height - nave->height;
}

void DibujarNave(Nave *nave, Sprite *punteroSprites){
 esat::DrawSprite(punteroSprites[12].sprite, nave->pos.x-100, nave->pos.y-(nave->height));
  esat::DrawSprite(punteroSprites[14].sprite, nave->pos.x-100, nave->pos.y + (nave->height));

  for(int i=0; i<3; i++){
      esat::DrawSprite(punteroSprites[12].sprite, nave->pos.x, nave->pos.y-(nave->height*i));
      esat::DrawSprite(punteroSprites[i].sprite, nave->pos.x, nave->pos.y-(nave->height*i));
  }
 
}
void TriggerNave(Nave *nave){
if(esat::IsSpecialKeyDown(esat::SpecialKey::kSpecialKey_Up)){
  nave->activada = true;
}


}
void MoverNave(Nave *nave, Direction d){
  if(nave->pos.y >= 100){
    nave->direccion = d;
    nave->pos.y -= nave->vel;
  }

  
}

int esat::main(int argc, char **argv) {

	esat::WindowInit(KWindow_Width, KWindow_Height);
	esat::WindowSetMouseVisibility(true);

	srand((unsigned)time(nullptr));
	last_time = esat::Time();

  Sprite *punteroSprites = InstanciarSpritesNave(15);
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

    DibujarNave(&nave, punteroSprites);
    TriggerNave(&nave);
    if(nave.activada){
      MoverNave(&nave, DOWN);
    }

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
