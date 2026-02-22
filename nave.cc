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
//#include "colisiones.h"
//#include "jugador.cc"

// Windows dimensions with default values
const int KWindow_Width = 800;
const int KWindow_Height = 800;

// FPS
double delta_time;
unsigned char fps = 25;
double current_time;
double last_time;

enum Direction
{
  UP,
  DOWN,
  STATIC
};

struct Sprites{
    esat::SpriteHandle sprite;
};

struct Nave
{
  float vel;
  esat::Vec2 pos;
  Direction direccion;
  int height = 32, fuelAmount;
  //COL::object nave_config;
};

Sprites *InstanciarSpritesNave(int numSprite)
{
  return (Sprites *)malloc(sizeof(Sprites) * numSprite);
}

void InitSpriteNave(Sprites *punteroSprites)
{
  punteroSprites[0].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_1_2x.png");
  punteroSprites[1].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_1_2x.png");
  punteroSprites[2].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_1_2x.png");
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
  punteroSprites[13].sprite = esat::SpriteFromFile("./SPRITES/NAVE/fondo_rosa.png");
  punteroSprites[14].sprite = esat::SpriteFromFile("./SPRITES/NAVE/explosion_1_2x.png");
  punteroSprites[15].sprite = esat::SpriteFromFile("./SPRITES/NAVE/explosion_2_2x.png");
}

void InstanciarNave(Nave *nave)
{
  const int terrain_height = 16;
  nave->vel = 100;
  nave->pos.x = KWindow_Width / 2;
  nave->pos.y = KWindow_Height - (nave->height * 3) - terrain_height;
  nave->fuelAmount = 0;
  nave->direccion = Direction::STATIC;
}

void MoverNave(Nave *nave)
{
    const int terrain_height = 16;
    if(nave->fuelAmount == 6) nave->direccion = Direction::UP;
    switch (nave->direccion){
        case Direction::UP:
            nave->pos.y -= nave->vel * delta_time;
            if(nave->pos.y + nave->height * 2 <= nave->height){
                nave->direccion = Direction::DOWN; 
                nave->fuelAmount = 0;
            }
            break;
        
        case Direction::DOWN:
            nave->pos.y += nave->vel * delta_time;
            if(nave->pos.y + nave->height * 3 >= KWindow_Height - terrain_height){
                nave->pos.y = KWindow_Height - terrain_height - nave->height * 3;
                nave->direccion = Direction::STATIC;
            }
            break;
    }
    
}

void RellenarFuelNave(Nave *nave)
{
    // Detecta colision y player tiene fuel
    if(esat::IsKeyDown('B') || esat::IsKeyDown('b'))
        if(nave->fuelAmount <= 6) nave->fuelAmount++;
}

void DibujarNave(Nave *nave, Sprites *punteroSprites)
{
    const int terrain_height = 16, pink_sprite_height = 16, max_sprites = 6;
    
    //Blanco
    for(int i = 0; i < 3; i++)
        esat::DrawSprite(punteroSprites[12].sprite, nave->pos.x, nave->pos.y + nave->height * i);

    //Rosa
    for(int i = 0; i < nave->fuelAmount; i++){
        int initFuelY = nave->pos.y + nave->height * 3 - pink_sprite_height;
        esat::DrawSprite(punteroSprites[13].sprite, nave->pos.x, initFuelY - (i * pink_sprite_height));
    }

    //Nave
    for(int i = 0; i < 3; i++)
        esat::DrawSprite(punteroSprites[i].sprite, nave->pos.x, nave->pos.y + nave->height * i);
}

int esat::main(int argc, char **argv){

  esat::WindowInit(KWindow_Width, KWindow_Height);
  esat::WindowSetMouseVisibility(true);

  srand((unsigned)time(nullptr));
  last_time = esat::Time();

  Sprites *punteroSprites = InstanciarSpritesNave(16);
  InitSpriteNave(punteroSprites);

  Nave nave;
  InstanciarNave(&nave);

  // Main game loop
  while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
  {

    // Calculate time elapsed since the last frame
    current_time = esat::Time();
    delta_time = (current_time - last_time) / 1000.0;
    // Limit delta_time to avoid large jumps
    if (delta_time > 0.1)
      delta_time = 0.1;

    last_time = current_time;

    esat::DrawBegin();
    esat::DrawClear(0, 0, 0);

    // InitFuel(&nave, punteroSprites);
    RellenarFuelNave(&nave);
    MoverNave(&nave);
    DibujarNave(&nave, punteroSprites);

    // Finish drawing
    esat::DrawEnd();
    // Control frame speed (fps)
    do
    {
      current_time = esat::Time();
    } while ((current_time - last_time) <= 1000.0 / fps);
    esat::WindowFrame();
  }

  // Destroy window
  esat::WindowDestroy();
  return 0;
}
