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
#include "audio.cc"
#include <esat_extra/soloud/soloud.h>
#define spritewidth 34
#define spriteheight 50
#define windowX 800
#define windowY 800

// FPS
double delta_time;
unsigned char fps = 25;
double current_time;
double last_time;
extern SoLoud::Soloud Soloud;

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
  bool volando;
  COL::object config_colision;
};
struct Bala
{
  esat::Vec2 pos;
  esat::Vec2 dir;
  float speed;
  bool activa;
  float tiempo_vida;
  COL::object config_bala;
  // animacion bala
  float longitud_actual;
  float longitud_maxima;
  float velocidad_crecimiento;
  unsigned char r, g, b;
};

Sprites *AsignarMemoriaSprites(int cantidad)
{
  return (Sprites *)malloc(cantidad * sizeof(Sprites));
}
Bala *AsignarMemoriaBalas(int cantidad)
{
  return (Bala *)malloc(cantidad * sizeof(Bala));
}
// ________________________________
// INSTANCIAR
// ________________________________

void InstanciarSpritesColores(Sprites *punteroSprites)
{
  punteroSprites[0].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/amarillo1.png");
  punteroSprites[1].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/rojo1.png");
  punteroSprites[2].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/azul1.png");
  punteroSprites[3].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/verde1.png");
}
void InstanciarSpritesPlayer(Sprites *punteroSprites)
{
  // suelo
  punteroSprites[0].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player1right.png");
  punteroSprites[1].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player2right.png");
  punteroSprites[2].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player3right.png");
  punteroSprites[3].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player4right.png");
  punteroSprites[4].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player1.png");
  punteroSprites[5].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player2.png");
  punteroSprites[6].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player3.png");
  punteroSprites[7].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/player4.png");
  // aire
  punteroSprites[8].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/right1.png");
  punteroSprites[9].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/right2.png");
  punteroSprites[10].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/right3.png");
  punteroSprites[11].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/right4.png");
  punteroSprites[12].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/left1.png");
  punteroSprites[13].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/left2.png");
  punteroSprites[14].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/left3.png");
  punteroSprites[15].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/left4.png");
}

void InstanciarPlayer(Jugador *player)
{
  player->pos.x = windowX / 2;
  player->pos.y = windowY - spriteheight;
  player->dir = {0, 0};
  player->isMoving = false;
  player->isShooting = false;
  player->volando = false;
  player->speed = 5.0f;
  player->mirandoDerecha = true;

  player->config_colision.width = spritewidth;
  player->config_colision.height = spriteheight;
}

void InstanciarBalas(Bala *bala)
{
  for (int i = 0; i < 20; i++)
  {
    bala[i].activa = false;
    bala[i].config_bala.width = 10;
    bala[i].config_bala.height = 10;
  }
}

void InstanciarCubo(COL::object *cubo_prueba, Sprites punteroSprites)
{
  cubo_prueba->sprite = punteroSprites.sprite;
  cubo_prueba->width = esat::SpriteWidth(punteroSprites.sprite);
  cubo_prueba->height = esat::SpriteHeight(punteroSprites.sprite);

  cubo_prueba->position.x = windowX - spritewidth;
  cubo_prueba->position.y = windowY - spriteheight;
}

// ________________________________
// BALAS
// ________________________________

void CrearDisparos(Bala *bala, Jugador player)
{
  if (esat::IsSpecialKeyDown(esat::kSpecialKey_Space))
  {
    PlayAudio(shoot);
    for (int i = 0; i < 20; i++)
    {
      if (!bala[i].activa)
      {
        bala[i].activa = true;
        bala[i].pos.x = player.pos.x + (spritewidth / 2);
        bala[i].pos.y = player.pos.y + (spriteheight / 2);
        bala[i].speed = 16.0f;
        bala[i].tiempo_vida = 0.0f;

        int tipo = rand() % 4;
        switch (tipo)
        {
        case 0: // CiAN
          bala[i].r = 0;
          bala[i].g = 255;
          bala[i].b = 255;
          break;

        case 1: // MAGENTA
          bala[i].r = 255;
          bala[i].g = 0;
          bala[i].b = 255;
          break;

        case 2: // AMARILLO
          bala[i].r = 255;
          bala[i].g = 255;
          bala[i].b = 0;
          break;

        case 3: // verd
          bala[i].r = 0;
          bala[i].g = 255;
          bala[i].b = 0;
          break;
        }

        // animacion bala
        bala[i].longitud_actual = 0.0f;
        bala[i].longitud_maxima = 200.0f;
        bala[i].velocidad_crecimiento = 350.0f;

        if (player.mirandoDerecha)
        {
          bala[i].dir = {1, 0};
          bala[i].pos.x = player.pos.x + spritewidth;
        }
        else
        {
          bala[i].dir = {-1, 0};
          bala[i].pos.x = player.pos.x;
        }
        break;
      }
    }
  }
}

void ActualizarDisparos(Bala *bala, Jugador player)
{
  float duracion_bala = 3.0f;

  for (int i = 0; i < 20; i++)
  {
    if (bala[i].activa)
    {
      bala[i].pos.x += bala[i].dir.x * bala[i].speed;
      //! si sale de pantalla (funcion carlos)
      bala[i].config_bala.position.x = bala[i].pos.x;
      bala[i].config_bala.position.y = bala[i].pos.y;
      bala[i].config_bala.colision = CreateColision(bala[i].config_bala);

      bala[i].tiempo_vida += delta_time;
      if (bala[i].tiempo_vida >= duracion_bala)
      {
        bala[i].activa = false;
      }
      // dejar rastro chulo
      if (bala[i].longitud_actual < bala[i].longitud_maxima)
      {
        bala[i].longitud_actual += bala[i].velocidad_crecimiento * delta_time;
      }
    }
  }
}

void ColisionDisparos(Bala *bala, COL::object objeto)
{
  for (int i = 0; i < 20; i++)
  {
    if (bala[i].activa)
    {
      if (CheckColision(bala[i].config_bala.colision, objeto.colision))
      {
        printf("Colision balas!!! ");
        bala[i].activa = false;
      }
    }
  }
}

void DibujarDisparos(Bala *bala)
{
  for (int i = 0; i < 20; i++)
  {
    if (!bala[i].activa)
      continue;

    float punta_x = bala[i].pos.x;
    float punta_y = bala[i].pos.y;
    float dir = bala[i].dir.x;

    float alto = 3.0f;
    float espacio = 20.0f;
    float largo_segmento = 18.0f;

    float distancia = bala[i].longitud_actual;

    if (distancia <= 0.0f)
      continue;

    // Dibujar desde la punta hacia atras
    for (float d = 0; d < distancia; d += espacio)
    {
      float inicio = punta_x - dir * d;

      // gradiente
      float factor = 1.0f - (d / distancia);

      if (factor < 0.0f)
        factor = 0.0f;

      // brillo cerca de la punta
      float brillo = powf(factor, 0.5f);

      unsigned char r = (unsigned char)(bala[i].r * brillo);
      unsigned char g = (unsigned char)(bala[i].g * brillo);
      unsigned char b = (unsigned char)(bala[i].b * brillo);

      esat::DrawSetFillColor(r, g, b);

      float segmento[8] = {
          inicio, punta_y,
          inicio - dir * largo_segmento, punta_y,
          inicio - dir * largo_segmento, punta_y + alto,
          inicio, punta_y + alto};

      esat::DrawSolidPath(segmento, 4);
    }

    // punta
    esat::DrawSetFillColor(255, 255, 255);

    float punta[8] = {
        punta_x, punta_y,
        punta_x - dir * 25.0f, punta_y,
        punta_x - dir * 25.0f, punta_y + alto,
        punta_x, punta_y + alto};

    esat::DrawSolidPath(punta, 4);
  }
}

// ________________________________
// DIBUJADO
// ________________________________

int ActualizarAnimacionJugador(Jugador jugador)
{
  static int frame = 0;
  static float timer = 0.0f;
  float frame_time = 0.15f;

  timer += delta_time;

  if (timer >= frame_time && (jugador.isMoving || jugador.volando))
  {
    timer = 0.0f;
    frame = (frame + 1) % 4;
  }
  else if (!jugador.isMoving && !jugador.volando)
  {
    frame = 0;
  }

  return frame;
}

void DibujarColoresJugador(Sprites *punteroSprites, Jugador jugador, int frame)
{
  esat::DrawSprite(punteroSprites[frame].sprite, jugador.pos.x, jugador.pos.y);
}

void DibujarJugador(Sprites *punteroSprites, Jugador jugador, int frame)
{
  int base = jugador.volando ? (jugador.mirandoDerecha ? 8 : 12) : (jugador.mirandoDerecha ? 0 : 4);

  esat::DrawSprite(punteroSprites[base + frame].sprite, jugador.pos.x, jugador.pos.y);
}

//_____________________________
// LIMITES PANTALLA
//______________________________
void ControlarLimitesPantalla(Jugador *player, Bala *bala)
{
  //! (funcion carlos)
  if (player->pos.x > windowX)
    player->pos.x = -spritewidth;
  if (player->pos.x < -spritewidth)
    player->pos.x = windowX;
  if (player->pos.y >= windowY - spriteheight)
    player->pos.y = 0;
  if (player->pos.y <= 0)
    player->pos.y = windowY - spriteheight;

  for (int i = 0; i < 20; i++)
  {
    if (bala[i].pos.x > windowX)
      bala[i].pos.x = -50;
    if (bala[i].pos.x < -50) // lo que mida la bala, en este caso 50
      bala[i].pos.x = windowX;
  }
}
//_____________________________
// JUGADOR
//______________________________

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

void LoopMoverJugador(bool moverLeft, bool moverRight, Jugador *player)
{
  if (moverLeft || moverRight)
  {
    player->isMoving = true;
    MoverJugador(player, moverLeft, moverRight);
    if (moverLeft)
      player->mirandoDerecha = false;
    else
      player->mirandoDerecha = true;
  }
  else
    player->isMoving = false;
}

void Ascender_Gravedad(Jugador *jugador, bool ascendiendo)
{
  float suelo = windowY - spriteheight;
  if (ascendiendo)
    jugador->pos.y -= jugador->speed;
  else
    jugador->pos.y += jugador->speed;

  if (jugador->pos.y >= suelo)
  {
    jugador->pos.y = suelo;
    jugador->volando = false;
  }
  else
  {
    jugador->volando = true;
  }
}

void ActualizarColisiones(Jugador *player, COL::object &cubo_prueba)
{
  player->config_colision.position.x = player->pos.x;
  player->config_colision.position.y = player->pos.y;
  player->config_colision.colision = COL::CreateColision(player->config_colision);
  cubo_prueba.colision = COL::CreateColision(cubo_prueba);
}

void ColisionPlayer(Jugador &player, COL::object &objeto)
{
  if (CheckColision(player.config_colision.colision, objeto.colision))
  {
    printf("colision player \n");
  }
}

// void DebuggingCubo(object cubo, Sprites punteroSprites)
// {
//   esat::DrawSprite(punteroSprites.sprite, cubo.position.x, cubo.position.y);
// }  COL::SHOWCOLISION

int esat::main(int argc, char **argv)
{
  AudioInit();
  esat::WindowInit(windowX, windowY);
  esat::WindowSetMouseVisibility(true);

  srand((unsigned)time(nullptr));
  last_time = esat::Time();

  // puntero a sprites
  Sprites *spritesColores = AsignarMemoriaSprites(4);
  Sprites *spritesPersonaje = AsignarMemoriaSprites(16);
  Bala *punteroBalas = AsignarMemoriaBalas(20);
  InstanciarSpritesColores(spritesColores);
  InstanciarSpritesPlayer(spritesPersonaje);
  InstanciarBalas(punteroBalas);

  Jugador player;
  InstanciarPlayer(&player);

  COL::object cubo_prueba;
  InstanciarCubo(&cubo_prueba, *spritesPersonaje);

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
    CrearDisparos(punteroBalas, player);
    ActualizarDisparos(punteroBalas, player);
    LoopMoverJugador(moverLeft, moverRight, &player);
    ControlarLimitesPantalla(&player, punteroBalas);
    ActualizarColisiones(&player, cubo_prueba);

    ColisionPlayer(player, cubo_prueba);
    ColisionDisparos(punteroBalas, cubo_prueba);

    int frame = ActualizarAnimacionJugador(player);
    DibujarDisparos(punteroBalas);
    DibujarColoresJugador(spritesColores, player, frame);
    DibujarJugador(spritesPersonaje, player, frame);

    // DebuggingCubo(cubo_prueba, *spritesColores);
    // DebuggingCubo(player.config_colision, *spritesColores);
    // SHOWcOLISION()

    // Finish drawing
    esat::DrawEnd();
    // Control frame speed (fps)
    do
    {
      current_time = esat::Time();
    } while ((current_time - last_time) <= 1000.0 / fps);
    esat::WindowFrame();
  }
  FreeAudio();
  // Destroy window
  esat::WindowDestroy();
  return 0;
}
