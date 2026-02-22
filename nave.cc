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
};

struct Sprite
{
  esat::SpriteHandle sprite;
};

struct Nave
{
  float vel;
  esat::Vec2 pos;
  Direction direccion;
  bool activada;
  int height = 32, fuelAmount = 0;
};

int alTFuel = 16, contFuel = 0;

Sprite *InstanciarSpritesNave(int numSprite)
{
  return (Sprite *)malloc(sizeof(Sprite) * numSprite);
}

Sprite *InstanciarFuelSprite(int totalFuel)
{
  return (Sprite *)malloc(sizeof(Sprite) * totalFuel);
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
  punteroSprites[13].sprite = esat::SpriteFromFile("./SPRITES/NAVE/fondo_rosa.png");
  punteroSprites[14].sprite = esat::SpriteFromFile("./SPRITES/NAVE/explosion_1_2x.png");
  punteroSprites[15].sprite = esat::SpriteFromFile("./SPRITES/NAVE/explosion_2_2x.png");
}

void InstanciarNave(Nave *nave)
{
  nave->activada = false;
  nave->vel = 10;
  nave->pos.x = KWindow_Width / 2;
  nave->pos.y = KWindow_Height - nave->height;
  nave->fuelAmount = 0;
}

void DrawFuel(Nave *nave, Sprite *punteroSprites, int j)
{
  int posY = KWindow_Height;

  for (int i = j * 2 + 1; i <= contFuel && contFuel <= 6; i++)
  {
    esat::DrawSprite(punteroSprites[13].sprite, nave->pos.x, posY - (i * alTFuel));
  }
}

void DibujarNave(Nave *nave, Sprite *punteroSprites)
{
  //  esat::DrawSprite(punteroSprites[12].sprite, nave->pos.x-100, nave->pos.y-(nave->height));
  //   esat::DrawSprite(punteroSprites[14].sprite, nave->pos.x-100, nave->pos.y + (nave->height));

  for (int i = 0; i < 3; i++)
  {
    // for(int j=0; j<2; j++){
    //   int altFuel = 16;

    //   esat::DrawSprite(punteroSprites[13].sprite, nave->pos.x, KWindow_Height - (altFuel*j));
    // }

    DrawFuel(nave, punteroSprites, i);
    esat::DrawSprite(punteroSprites[i].sprite, nave->pos.x, nave->pos.y - (nave->height * i));
  }
}

void InitFuel(Nave *nave, Sprite *spriteFuel)
{
  // bool encendido = false;

  if (esat::IsSpecialKeyDown(esat::SpecialKey::kSpecialKey_F1))
  {
    contFuel++;
  }
}

void TriggerNave(Nave *nave, Sprite *punteroSprites)
{
  if (esat::IsSpecialKeyDown(esat::SpecialKey::kSpecialKey_Up))
  {
    nave->activada = true;
    nave->direccion = Direction::UP;
  }
}

void MoverNave(Nave *nave)
{
  switch (nave->direccion)
  {
  case Direction::UP:
    nave->pos.y -= nave->vel;
    break;

  case Direction::DOWN:
    nave->pos.y += nave->vel;
    break;
  }

  if (nave->pos.y <= 100)
  {
    nave->direccion = Direction::DOWN;
  }

  if (nave->pos.y + nave->height >= KWindow_Height)
  {
    nave->pos.y = KWindow_Height - nave->height;
  }
}

void RellenarNave(Nave *nave, Sprite *punteroSprites)
{
  int max_sprites = 6;
  int alturaTotal = max_sprites * 16;

  // esat::DrawSprite(punteroSprites[13].sprite, posicion_rosa.x, posicion_rosa.y);

  if (esat::IsKeyDown('B') || esat::IsKeyDown('b'))
  {
    if (nave->fuelAmount < max_sprites)
      nave->fuelAmount++;
  }
  printf(" %d ", nave->fuelAmount);

  for (int i = 0; i <= max_sprites; i++)
  {
    // the white sprite is twice as tall so we need to draw it first to prevent it of overlapping with the pink

    if(i < 3){
      esat::DrawSprite(punteroSprites[12].sprite, nave->pos.x, (nave->pos.y - (i * 32)));
    }
    //pink
    if (i < nave->fuelAmount)
    {
      esat::DrawSprite(punteroSprites[13].sprite, nave->pos.x, (nave->pos.y - (i * 16) + 16));
    }
  }
}

int esat::main(int argc, char **argv)
{

  esat::WindowInit(KWindow_Width, KWindow_Height);
  esat::WindowSetMouseVisibility(true);

  srand((unsigned)time(nullptr));
  last_time = esat::Time();

  Sprite *punteroSprites = InstanciarSpritesNave(16);
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

    InitFuel(&nave, punteroSprites);
    TriggerNave(&nave, punteroSprites);
    MoverNave(&nave);
    RellenarNave(&nave, punteroSprites);
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
