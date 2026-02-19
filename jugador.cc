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
#include "globalfunctions.h"
#define spritewidth 34
#define spriteheight 50

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
};

/*Asigna memoria a las instancias de Sprites
  param cantidad: el numero de elementos que tiene ese bloque
  devuelve el puntero*/
Sprites *AsignarMemoriaSprites(int cantidad)
{
  return (Sprites *)malloc(cantidad * sizeof(Sprites));
}

/*Asigna memoria a las instancias de Bala
  param cantidad: el numero de elementos que tiene ese bloque
  devuelve el puntero*/
Bala *AsignarMemoriaBalas(int cantidad)
{
  return (Bala *)malloc(cantidad * sizeof(Bala));
}
// ________________________________
// INSTANCIAR
// ________________________________

/*Carga sprites en el puntero de los sprites colores
  param *punteroSprites El puntero donde se cargan los sprites colores
*/
void InstanciarSpritesColores(Sprites *punteroSprites)
{
  punteroSprites[0].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/amarillo1.png");
  punteroSprites[1].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/rojo1.png");
  punteroSprites[2].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/azul1.png");
  punteroSprites[3].sprite = esat::SpriteFromFile("SPRITES/JUGADOR/verde1.png");
}

/*Carga sprites en el puntero de los sprites del jugador
  param *punteroSprites El puntero donde se cargan los sprites del jugador
*/
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

/*Instancia el Player
  param *player puntero a la instancia Player
*/
void InstanciarPlayer(Jugador *player)
{
  player->pos.x = GLO::kScreenWidth / 2;
  player->pos.y = GLO::kScreenHeight - spriteheight;
  player->dir = {0, 0};
  player->isMoving = false;
  player->isShooting = false;
  player->volando = false;
  player->speed = 5.0f;
  player->mirandoDerecha = true;

  player->config_colision.width = spritewidth;
  player->config_colision.height = spriteheight;
}

/*Instancia las balas del puntero de las balas
  param *bala Puntero de las balas
*/
void InstanciarBalas(Bala *bala)
{
  for (int i = 0; i < 20; i++)
  {
    bala[i].activa = false;
    bala[i].config_bala.width = 10;
    bala[i].config_bala.height = 10;
  }
}

/*Instancia el cubo para hacer pruebas
  param *cubo_prueba puntero al cubo para hacer pruebas
  param 
*/
void InstanciarCubo(COL::object *cubo_prueba, Sprites punteroSprites)
{
  cubo_prueba->sprite = punteroSprites.sprite;
  cubo_prueba->width = esat::SpriteWidth(punteroSprites.sprite);
  cubo_prueba->height = esat::SpriteHeight(punteroSprites.sprite);

  cubo_prueba->position.x = GLO::kScreenWidth - spritewidth;
  cubo_prueba->position.y = GLO::kScreenHeight - spriteheight;
}

// ________________________________
// BALAS
// ________________________________

void CrearDisparos(Bala *bala, Jugador player)
{
  if (esat::IsSpecialKeyDown(esat::kSpecialKey_Space))
  {
    for (int i = 0; i < 20; i++)
    {
      if (!bala[i].activa)
      {
        bala[i].activa = true;
        bala[i].pos.x = player.pos.x + (spritewidth / 2);
        bala[i].pos.y = player.pos.y + (spriteheight / 2);
        bala[i].speed = 16.0f;
        bala[i].tiempo_vida = 0.0f;
        
        if (player.mirandoDerecha)
          bala[i].dir = {1, 0};
        else
        {
          bala[i].dir = {-1, 0};
          bala[i].pos.x = player.pos.x - (spritewidth / 2);
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
      bala[i].pos.x += bala[i].dir.x * bala[i].speed * delta_time;
      //! si sale de pantalla (funcion carlos)
      bala[i].config_bala.position.x = bala[i].pos.x;
      bala[i].config_bala.position.y = bala[i].pos.y;
      bala[i].config_bala.colision = CreateColision(bala[i].config_bala);
      
      bala[i].tiempo_vida += delta_time;
      if (bala[i].tiempo_vida >= duracion_bala)
      {
        bala[i].activa = false;
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
    if (bala[i].activa)
    {
      float balas[8] = {bala[i].pos.x, bala[i].pos.y, bala[i].pos.x + 50, bala[i].pos.y, bala[i].pos.x + 50, bala[i].pos.y + 2, bala[i].pos.x, bala[i].pos.y + 2};
      esat::DrawSetFillColor(255, 255, 255);
      esat::DrawSolidPath(balas, 4);
    }
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
  int base = jugador.volando
    ? (jugador.mirandoDerecha ? 8 : 12)
    : (jugador.mirandoDerecha ? 0 : 4);

  esat::DrawSprite(punteroSprites[base + frame].sprite, jugador.pos.x, jugador.pos.y);
}


//_____________________________
// LIMITES PANTALLA
//______________________________
void ControlarLimitesPantalla(Jugador *player, Bala *bala)
{
  const int terrain_height = 16;
  //! (funcion carlos)
  if (player->pos.x > GLO::kScreenWidth)
    player->pos.x = -spritewidth;
  if (player->pos.x < -spritewidth)
    player->pos.x = GLO::kScreenWidth;
  if (player->pos.y >= GLO::kScreenHeight - terrain_height)
    player->pos.y = GLO::kScreenHeight - (terrain_height + spriteheight);
  if (player->pos.y <= 0)
    player->pos.y = GLO::kScreenHeight - spriteheight;

  for (int i = 0; i < 20; i++)
  {
    if (bala[i].pos.x > GLO::kScreenWidth)
      bala[i].pos.x = -50;
    if (bala[i].pos.x < -50) // lo que mida la bala, en este caso 50
      bala[i].pos.x = GLO::kScreenWidth;
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
    jugador->pos.x -= jugador->speed * delta_time;
  }
  if (moverRight)
  {
    jugador->isMoving = true;
    jugador->pos.x += jugador->speed * delta_time;
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
  const int terrain_height = 16;
  const float suelo = GLO::kScreenHeight - terrain_height;
  if (ascendiendo)
    jugador->pos.y -= jugador->speed * delta_time;
  else
    jugador->pos.y += jugador->speed * delta_time;

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
  esat::WindowInit(GLO::kScreenWidth, GLO::kScreenHeight);
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
    DibujarColoresJugador(spritesColores, player, frame);
    DibujarJugador(spritesPersonaje, player, frame);
    DibujarDisparos(punteroBalas);

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

  // Destroy window
  esat::WindowDestroy();
  return 0;
}
