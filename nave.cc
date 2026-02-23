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
  STATIC
};

struct Nave
{
  float vel;
  esat::Vec2 pos;
  Direction direccion;
  int height = 32, fuelAmount;
  COL::object nave_config;
};

struct Sprites
{
  esat::SpriteHandle sprite;
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
  punteroSprites[3].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_2_2x.png");
  punteroSprites[4].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_2_2x.png");
  punteroSprites[5].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_2_2x.png");
  punteroSprites[6].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_3_2x.png");
  punteroSprites[7].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_3_2x.png");
  punteroSprites[8].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_3_2x.png");
  punteroSprites[9].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_4_2x.png");
  punteroSprites[10].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_4_2x.png");
  punteroSprites[11].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_4_2x.png");
  punteroSprites[12].sprite = esat::SpriteFromFile("./SPRITES/NAVE/fondo_blanco.png");
  punteroSprites[13].sprite = esat::SpriteFromFile("./SPRITES/NAVE/fondo_rosa.png");
  punteroSprites[14].sprite = esat::SpriteFromFile("./SPRITES/NAVE/explosion_1_2x.png");
  punteroSprites[15].sprite = esat::SpriteFromFile("./SPRITES/NAVE/explosion_2_2x.png");
}

void InstanciarNave(Nave *nave)
{
  const int terrain_height = 16;
  nave->vel = 100;
  nave->pos.x = KWindow_Width - 100;
  nave->pos.y = KWindow_Height - (nave->height * 3) - terrain_height;
  nave->fuelAmount = 0;
  nave->direccion = Direction::STATIC;
  nave->nave_config.position.x = nave->pos.x;
  nave->nave_config.position.y = 0;
  nave->nave_config.width = 32;
  nave->nave_config.height = KWindow_Height - terrain_height;
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

/*Dibuja las capas del cohete (fondo blanco, fondo rosa y el sprite transparente)
    param max_sprites El máximo de sprites que debe dibujar en cada capa
    param *punteroSprites Donde se guardan los sprites de la nave
    param sprite_ptr_position La posicion dentro del puntero del sprite a dibujar o -1 si tiene que iterar con el indice del for
    param pos_x La posicion en X del sprite a dibujar
    param pos_y La posicion en Y del sprite a dibujar
    param height La altura a la que se debe dibujar cada sprite respecto a pos_y
*/
void DrawRocketLayer(int max_sprites, const Sprites *punteroSprites, int sprite_ptr_position, float pos_x, float pos_y, int height){
    for(int i = 0; i < max_sprites; ++i){
        if(sprite_ptr_position >= 0) esat::DrawSprite(punteroSprites[sprite_ptr_position].sprite, pos_x, pos_y + height * i);
        if(sprite_ptr_position == -1) esat::DrawSprite(punteroSprites[i].sprite, pos_x, pos_y + height * i);
    }
}

/*Rellena el puntero de Vec2 con los puntos del fondo rojo de la explosion
    param explosion_path El puntero de Vec2 para rellenar
    param nave El objeto de la nave para coger
    param explosion_size Los pixeles de ancho/alto del sprite de la explosion 
*/
void FillVec2PtrPoints(esat::Vec2* explosion_path, Nave nave, int explosion_size){
    explosion_path[0] = {nave.pos.x, nave.pos.y + nave.height * 3};
    explosion_path[1] = {nave.pos.x + explosion_size, nave.pos.y + nave.height * 3};
    explosion_path[2] = {nave.pos.x + explosion_size, nave.pos.y + nave.height * 3 + explosion_size};
    explosion_path[3] = {nave.pos.x, nave.pos.y + nave.height * 3 + explosion_size};
}

/*Dibuja el fondo rojo de la explosion y el sprite transparente de la explosion
    param explosion_path El puntero con las posiciones Vec2 de los vertices del fondo
    param punteroSprites El puntero con los sprites de las explosiones
*/
void DrawRocketExplosion(esat::Vec2* explosion_path, const Sprites* punteroSprites){
    static float explosion_flicker = 0.0f;
    esat::DrawSetFillColor(255,0,0);
    esat::DrawSolidPath(&explosion_path->x, 4);
    if((int)(explosion_flicker / 0.075) % 2)
        esat::DrawSprite(punteroSprites[14].sprite, explosion_path->x, explosion_path->y);
    else
        esat::DrawSprite(punteroSprites[15].sprite, explosion_path->x, explosion_path->y);
    explosion_flicker += delta_time;
}

/*Prepara el puntero de las posiciones de los vertices del fondo y llama a la funcion de dibujar
    param nave El objeto de la nave para sacar las posiciones del sprite y del fondo
    param explosion_size El ancho/alto del sprite de la explosion
    param punteroSprites El puntero con los sprites de la explosion
*/
void SetRocketExplosion(Nave nave, int explosion_size, const Sprites* punteroSprites){
    esat::Vec2* explosion_path = (esat::Vec2*)malloc(4 * sizeof(esat::Vec2));
    FillVec2PtrPoints(explosion_path, nave, explosion_size);
    DrawRocketExplosion(explosion_path, punteroSprites);
}

void DibujarNave(Nave *nave, Sprites *punteroSprites)
{
    const int terrain_height = 16, pink_sprite_height = 16, max_sprites = 6, explosion_size = 32, rocket_total_height = nave->height * 3,
                initFuelY = nave->pos.y + rocket_total_height - pink_sprite_height;
    
    //Blanco
    DrawRocketLayer(3, punteroSprites, 12, nave->pos.x, nave->pos.y, nave->height);
    //Rosa
    DrawRocketLayer(nave->fuelAmount, punteroSprites, 13, nave->pos.x, initFuelY, -pink_sprite_height);
    //Nave
    DrawRocketLayer(3, punteroSprites, -1, nave->pos.x, nave->pos.y, nave->height);

    //Calcula si el espacio entre el suelo y la base del cohete es como mínimo igual a explosion_size
    if(KWindow_Height - terrain_height - (nave->pos.y + rocket_total_height) >= explosion_size) SetRocketExplosion(*nave, explosion_size, punteroSprites);
}

/*int esat::main(int argc, char **argv){

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
}*/

