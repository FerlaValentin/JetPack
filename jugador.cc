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
#define spritewidth 34
#define spriteheight 50
#define windowX 800
#define windowY 800

// FPS
double delta_time;
unsigned char fps = 25;
double current_time;
double last_time;
struct Sprites
{
  esat::SpriteHandle sprite;
};

struct Jugador
{
  esat::Vec2 pos;
  esat::Vec2 dir;
  float speed;
  bool isMoving;
  bool isShooting;
  bool mirandoDerecha;
};

Sprites *AsignarMemoriaSprites(int cantidad)
{
  return (Sprites *)malloc(cantidad * sizeof(Sprites));
}

void InstanciarSpritesColores(Sprites *punteroSprites)
{
  punteroSprites[0].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/amarillo1.png");
  punteroSprites[1].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/rojo1.png");
  punteroSprites[2].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/azul1.png");
  punteroSprites[3].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/verde1.png");
}
void InstanciarSpritesPlayer(Sprites *punteroSprites)
{
  punteroSprites[0].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player1.png");
  punteroSprites[1].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player2.png");
  punteroSprites[2].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player3.png");
  punteroSprites[3].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player4.png");
}

void InstanciarPlayer(Jugador *player)
{
  player->pos.x = windowX / 2;
  player->pos.y = windowY - spriteheight;
  player->dir = {0, 0};
  player->isMoving = false;
  player->isShooting = false;
  player->speed = 5.0f;
  player->mirandoDerecha = true;
}

void DibujarColoresJugador(Sprites *punteroSprites, Jugador jugador)
{
  static int frame_actual = 0;
  static float timer = 0.0f;
  float frame_time = 0.15f;

  timer += delta_time;
  //! Cambiar posteriormente !!!!!!!!
  if (timer >= frame_time)
  {
    timer = 0.0f;
    frame_actual = (frame_actual + 1) % 4;
  }
  esat::DrawSprite(punteroSprites[frame_actual].sprite, jugador.pos.x, jugador.pos.y);
}

void ControlarLimitesPantalla(Jugador *player)
{
  if (player->pos.x >= windowX - spritewidth)
    player->pos.x = windowX - spritewidth;
  if (player->pos.x <= 0)
    player->pos.x = 0;
  if (player->pos.y >= windowY - spriteheight)
    player->pos.y = windowY - spriteheight;
  if (player->pos.y <= 0)
    player->pos.y = 0;
}

void MoverJugador(Jugador *jugador, bool moverLeft, bool moverRight)
{
  if (!jugador->isMoving)
    return;
  if (moverLeft)
  {
    jugador->isMoving = true;
    jugador->pos.x -= jugador->speed;
  }
  if (moverRight)
  {
    jugador->isMoving = true;
    jugador->pos.x += jugador->speed;
  }
}
void Ascender_Gravedad(Jugador *jugador, bool ascendiendo)
{
  if (ascendiendo)
  {
    jugador->pos.y -= jugador->speed;
  }
  else
  {
    jugador->pos.y += jugador->speed;
  }
}

void DibujarJugador(Sprites *punteroSprites, Jugador jugador)
{
  static int frame_actual = 0;
  static float timer = 0.0f;
  float frame_time = 0.15f;

  timer += delta_time;
  // if (!jugador.mirandoDerecha)

  // else

  //! Cambiar posteriormente !!!!!!!!
  if (timer >= frame_time && jugador.isMoving)
  {
    timer = 0.0f;
    frame_actual = (frame_actual + 1) % 4;
  }
  else if (!jugador.isMoving)
    frame_actual = 0;

  esat::DrawSprite(punteroSprites[frame_actual].sprite, jugador.pos.x, jugador.pos.y);
}

int esat::main(int argc, char **argv)
{
  esat::WindowInit(windowX, windowY);
  esat::WindowSetMouseVisibility(true);

  srand((unsigned)time(nullptr));
  last_time = esat::Time();

  // puntero a sprites
  Sprites *spritesColores = AsignarMemoriaSprites(4);
  Sprites *spritesPersonaje = AsignarMemoriaSprites(4);
  InstanciarSpritesColores(spritesColores);
  InstanciarSpritesPlayer(spritesPersonaje);

  Jugador player;
  InstanciarPlayer(&player);

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

    bool moverLeft = (esat::IsKeyPressed('A') || esat::IsKeyPressed('a'));
    bool moverRight = (esat::IsKeyPressed('D') || esat::IsKeyPressed('d'));
    bool ascender = (esat::IsKeyPressed('W') || esat::IsKeyPressed('w'));
    Ascender_Gravedad(&player, ascender);
    ControlarLimitesPantalla(&player);
    if (moverLeft || moverRight)
    {
      player.isMoving = true;
      MoverJugador(&player, moverLeft, moverRight);
      if (moverLeft)
        player.mirandoDerecha = false;
      else
        player.mirandoDerecha = true;
    }
    else
      player.isMoving = false;

    DibujarColoresJugador(spritesColores, player);
    DibujarJugador(spritesPersonaje, player);

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
