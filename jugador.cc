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
#include "interface.cc"
#include "audio.cc"
#include <esat_extra/soloud/soloud.h>

/*struct Sprites
{
  esat::SpriteHandle sprite;
}*/

struct Jugador
{
  esat::Vec2 pos;
  esat::Vec2 dir;
  float speed;
  bool isMoving;
  bool isShooting;
  bool mirandoDerecha;
  bool volando;
  bool tiene_gasofa;
  COL::object config_colision;
  int vidas;
  int puntos;
  int player_id;
  bool isActive;
  bool muerto;
  float tiempo_aparicion;
  float tiempo_invulnerable;
  int spriteWidth;
  int spriteHeight;
  bool colisiona;
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

struct ItemDrop
{
  COL::object item_config;
  int tipo;
  float cooldown;
  bool recogido;
};

#include "data.cc"

//! LAS FUNCIONES DE MALLOC FUERA
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

void InstanciarSpritesItems(Sprites *punteroSprites)
{
  punteroSprites[1].sprite = esat::SpriteFromFile("SPRITES/ITEMS/chafada.png");
  punteroSprites[2].sprite = esat::SpriteFromFile("SPRITES/ITEMS/diamante.png");
  punteroSprites[3].sprite = esat::SpriteFromFile("SPRITES/ITEMS/oro.png");
  punteroSprites[4].sprite = esat::SpriteFromFile("SPRITES/ITEMS/radioa.png");
  punteroSprites[0].sprite = esat::SpriteFromFile("SPRITES/ITEMS/triangulo.png");
  punteroSprites[5].sprite = esat::SpriteFromFile("SPRITES/ITEMS/fuel.png");
}

void InstanciarPlayer(Jugador *player)
{
  const int terrain_height = 16;
  player->spriteWidth = 34;
  player->spriteHeight = 50;
  player->pos.x = kScreenWidth / 2;
  player->pos.y = kScreenHeight - player->spriteHeight - terrain_height;
  player->dir = {0, 0};
  player->colisiona = true;
  player->isMoving = false;
  player->isShooting = false;
  player->volando = false;
  player->speed = 80.0f;
  player->mirandoDerecha = true;
  player->tiene_gasofa = false;
  player->muerto = false;
  player->tiempo_aparicion = 5.0f;
  player->tiempo_invulnerable = 3.0f;

  player->config_colision.width = player->spriteWidth;
  player->config_colision.height = player->spriteHeight;

  player->puntos = 0;
  player->vidas = 3;
  player->player_id = 1;
  player->isActive = true;
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

void InstaciarGasofa_Nave(ItemDrop *gasofa, Sprites punteroSprites)
{
  int spritewidth = 34;
  int spriteheight = 50;

  gasofa->item_config.sprite = punteroSprites.sprite;
  gasofa->item_config.width = esat::SpriteWidth(punteroSprites.sprite);
  gasofa->item_config.height = esat::SpriteHeight(punteroSprites.sprite);

  gasofa->item_config.position.x = kScreenWidth - spritewidth;
  gasofa->item_config.position.y = kScreenHeight - spriteheight;
  gasofa->cooldown = 5.0f;
  gasofa->recogido = false;
}
void InstanciarItems(ItemDrop *item, Sprites *punteroSprites)
{
  item->tipo = rand() % 5; // 0 1 2 3 4
  item->cooldown = 5.0f;
  item->recogido = false;
  item->item_config.position.y = kScreenHeight / 2;
  item->item_config.position.x = 0;
  item->item_config.sprite = punteroSprites[item->tipo].sprite;
  item->item_config.width = esat::SpriteWidth(punteroSprites[0].sprite);
  item->item_config.height = esat::SpriteHeight(punteroSprites[0].sprite);
}

// ________________________________
// BALAS
// ________________________________

void CrearDisparos(Bala *bala, Jugador player)
{
  if (esat::IsSpecialKeyDown(esat::kSpecialKey_Space))
  {
    bool inactive_bullet_found = false;
    PlayAudio(shoot);
    for (int i = 0; i < 20 && !inactive_bullet_found; i++)
    {
      if (!bala[i].activa)
      {
        inactive_bullet_found = true;
        bala[i].activa = true;
        bala[i].pos.x = player.pos.x + (player.spriteWidth / 2);
        bala[i].pos.y = player.pos.y + (player.spriteHeight / 2);
        bala[i].speed = 400.0f;
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
        }

        // animacion bala
        bala[i].longitud_actual = 0.0f;
        bala[i].longitud_maxima = 200.0f;
        bala[i].velocidad_crecimiento = 350.0f;

        if (player.mirandoDerecha)
        {
          bala[i].dir = {1, 0};
          bala[i].pos.x = player.pos.x + player.spriteWidth;
        }
        else
        {
          bala[i].dir = {-1, 0};
          bala[i].pos.x = player.pos.x;
        }
      }
    }
  }
}

void ActualizarDisparos(Bala *bala, Jugador player)
{
  float duracion_bala = 1.5f;

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
      // dejar rastro chulo
      if (bala[i].longitud_actual < bala[i].longitud_maxima)
      {
        bala[i].longitud_actual += bala[i].velocidad_crecimiento * delta_time;
      }
    }
  }
}

// void ColisionDisparos(Bala *bala, COL::object objeto)
// {
//   for (int i = 0; i < 20; i++)
//   {
//     if (bala[i].activa)
//     {
//       if (CheckColision(bala[i].config_bala.colision, objeto.colision))
//       {
//         bala[i].activa = false;
//       }
//     }
//   }
// }

void ColisionDisparos(Bala *bala, ENE::EnemyManager *punteroEnemy) // disparos enemigos
{
  for (int i = 0; i < 20; i++)
  {
    for (int j = 0; j < punteroEnemy->pool_size; j++)
    {
      if (bala[i].activa && (*punteroEnemy).pool[j].active)
      {
        if (CheckColision(bala[i].config_bala.colision, (*punteroEnemy).pool[j].col))
        {
          PlayAudio(enemyDies);
          bala[i].activa = false;
          // Sprite explosion, animacion, etc
          printf("colision ENEMIGO !! \n");
        }
      }
    }
  }
}

void EnemiesCollision(ENE::EnemyManager* mgr, Jugador *player, int frame){
  for(int i = 0; i < mgr->pool_size; ++i){
    ENE::Enemy *e = mgr->pool+i;
    if(e->active){
      bool collision_now = COL::CheckColision(e->col,player->config_colision.colision);
      if(collision_now && !e->iscolliding){
        player->vidas --;
        ENE::ExplodeAt(e->position.x, e->position.y, e->Color);
        ENE::ExplodeAt(player->pos.x, player->pos.y, static_cast<ENE::ColorType>(frame));
        player->muerto = true;
      }
      e->iscolliding = collision_now;
    }
  }
}

void DibujarDisparos(Bala *bala)
{
  for (int i = 0; i < 20; i++)
  {
    if (bala[i].activa)
    {
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

void DibujarGasofa(ItemDrop gasofa, Sprites *punteroSprites, Nave nave)
{
  if (nave.direccion == Direction::STATIC)
  {
    float puntos[8] = {gasofa.item_config.position.x, gasofa.item_config.position.y, gasofa.item_config.position.x + 32, gasofa.item_config.position.y, gasofa.item_config.position.x + 32, gasofa.item_config.position.y + 32, gasofa.item_config.position.x, gasofa.item_config.position.y + 32};

    esat::DrawSetFillColor(255, 0, 255);
    esat::DrawSolidPath(puntos, 4);
    esat::DrawSprite(punteroSprites[5].sprite, gasofa.item_config.position.x, gasofa.item_config.position.y);
  }
}
// ! convertir gasofa a ItemDrop IMPORTANTE !!!!!!! y ahorrarse una funcion aqui
void DibujarItems(ItemDrop item, Sprites *punteroSprites)
{
  if (item.recogido)
    return;
  float puntos[8] = {item.item_config.position.x, item.item_config.position.y, item.item_config.position.x + 32, item.item_config.position.y, item.item_config.position.x + 32, item.item_config.position.y + 32, item.item_config.position.x, item.item_config.position.y + 32};

  esat::DrawSetFillColor(rand() % 255, rand() % 255, rand() % 255);
  esat::DrawSolidPath(puntos, 4);
  esat::DrawSprite(punteroSprites[item.tipo].sprite, item.item_config.position.x, item.item_config.position.y);
}

//_____________________________
// LIMITES PANTALLA
//______________________________
void ControlarLimitesPantalla(Jugador *player, Bala *bala)
{
  const int hud_height = 50;
  //! (funcion carlos)
  if (player->pos.x > kScreenWidth)
    player->pos.x = -player->spriteWidth;
  if (player->pos.x < -player->spriteWidth)
    player->pos.x = kScreenWidth;
  if (player->pos.y >= kScreenHeight - player->spriteHeight - 16)
    player->pos.y = kScreenHeight - player->spriteHeight - 16;
  if (player->pos.y <= hud_height)
    player->pos.y = hud_height;

  for (int i = 0; i < 20; i++)
  {
    if (bala[i].pos.x > kScreenWidth)
      bala[i].pos.x = -50;
    if (bala[i].pos.x < -50) // lo que mida la bala, en este caso 50
      bala[i].pos.x = kScreenWidth;
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
  float suelo = kScreenHeight - jugador->spriteHeight - 16;
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
// _____________________________________
//              ITEMS
// _______________________________________

void SpawnItem(COL::object &item, Nave nave)
{
  const int hud_height = 50;
  //Que solo spawnee en el espacio que hay a la derecha o a la izquierda de la nave, pero no encima
  float x = rand() % 2 ? rand() % (int)(kScreenWidth - item.width - (nave.pos.x + nave.nave_config.width)) + (nave.pos.x + nave.nave_config.width) : rand() % (int)(nave.pos.x - item.width);
  item.position.x = x;
  item.position.y = hud_height;
}

void GravedadItem(COL::object &item)
{
  if(item.position.y < kScreenHeight){
    const int terrain_height = 16, item_height = 32;
    float speed = 5.0f;
    if(item.position.y + speed >= kScreenHeight - terrain_height - item_height)
      item.position.y = kScreenHeight - terrain_height - item_height;
    else
      item.position.y += speed;
  }
}

void CambiarTipoItem(ItemDrop *item, Sprites *sprites)
{
  item->tipo = rand() % 5;
  item->item_config.sprite = sprites[item->tipo].sprite;
  item->item_config.width = esat::SpriteWidth(item->item_config.sprite);
  item->item_config.height = esat::SpriteHeight(item->item_config.sprite);
}

void UpdateGasofaPosition(Jugador player, ItemDrop &gasofa)
{
  gasofa.item_config.position.x = player.pos.x;
  gasofa.item_config.position.y = player.pos.y + player.spriteHeight / 2;
}

void SpawnGasofaConTimer(ItemDrop &gasofa, Nave nave)
{
  const float final_timer = 0.0f;
  gasofa.cooldown -= delta_time;
  if (gasofa.cooldown <= final_timer)
  {
    gasofa.cooldown = 5.0f;
    SpawnItem(gasofa.item_config, nave);
  }
}

/*Actualiza la posicion de la gasofa
  param player El objeto del jugador
  param gasofa El objeto de la gasolina
*/
void MoveGasofa(Jugador &player, ItemDrop &gasofa, TPlatform* g_platforms){
  if(player.tiene_gasofa)
    UpdateGasofaPosition(player, gasofa);
  else{
    bool colisionWithPlatform = false;
    int nPlatform;for(int i = 0; i < 3; ++i)
    {
      TPlatform *p = g_platforms + i;
      if(CheckColision(gasofa.item_config.colision, p->collision_platform.colision)){
        nPlatform = i;
        colisionWithPlatform = true;
      }
    }
    if(colisionWithPlatform){
      gasofa.item_config.colision.p2.y = g_platforms[nPlatform].collision_platform.colision.p1.y  + 1;
      gasofa.item_config.colision.p1.y = gasofa.item_config.colision.p2.y - gasofa.item_config.height;
    }
    else
      GravedadItem(gasofa.item_config);
  }
}

/*Comprueba si el sprite de la gasolina está a la altura de la nave donde se debe sumar a su contador de gasolina
  param y_fuel La altura actual del sprite de la gasolina
  Devuelve True si ya está a la altura en la que debe sumarse*/
bool IsGasofaOnAddingPosition(float y_fuel){
  const int sprites_height = 16;
  const float gravity_velocity = 5.0f;
  //Uso gravity_velocity como un offset por seguridad por si la Y nunca vale (kScreenHeight - sprites_height * 3)
  return y_fuel >= kScreenHeight - sprites_height * 3 &&
       y_fuel <= kScreenHeight - sprites_height * 3 + gravity_velocity;
}

/*Lógica que comprueba si la gasolina pasa por el punto de la nave donde la absorba, agrega gasolina al contador de gasolina del cohete
  y mueve el sprite fuera de la pantalla mientras pasa el tiempo de cooldown
  param gasofa El objeto de la gasolina
  param nave El objeto de la nave*/
void AddFuelToRocket(ItemDrop &gasofa, Nave *nave){
  if(IsGasofaOnAddingPosition(gasofa.item_config.position.y)){ 
    nave->fuelAmount++;
    gasofa.item_config.position.y = kScreenHeight;
    gasofa.recogido = false;
  }
}

void LoopGasofa(Jugador &player, ItemDrop &gasofa, Nave *nave, TPlatform* g_platforms)
{
  if (nave->direccion == Direction::STATIC)
  {
    MoveGasofa(player, gasofa, g_platforms);
    if (COL::CheckColision(player.config_colision.colision, gasofa.item_config.colision) && gasofa.recogido == false)
    {
      gasofa.recogido = true;
      player.tiene_gasofa = true;
    }
    if (player.tiene_gasofa)
      if (COL::CheckColision(player.config_colision.colision, nave->nave_config.colision)){
        player.tiene_gasofa = false;
        gasofa.item_config.position.x = nave->nave_config.colision.p1.x;
      }
    if(gasofa.recogido == true && player.tiene_gasofa == false) AddFuelToRocket(gasofa, nave);
    if(gasofa.item_config.position.y >= kScreenHeight) SpawnGasofaConTimer(gasofa, *nave);
  }
}

void ActualizarColisionesItems(ItemDrop &gasofa, ItemDrop &item, Nave *nave)
{
  gasofa.item_config.colision = COL::CreateColision(gasofa.item_config);
  nave->nave_config.colision = COL::CreateColision(nave->nave_config);
  item.item_config.colision = COL::CreateColision(item.item_config);
}

void LoopPickItems(Jugador player, ItemDrop *item, Sprites *sprites, Nave nave, TPlatform* g_platforms)
{
  static float timer = 0.0f;
  if (!item->recogido)
  {
    bool colisionWithPlatform = false;
    int nPlatform;
    // detecta colision con plataformas y guarda en caso de ser Cierto
    for(int i = 0; i < 3; ++i)
    {
      TPlatform *p = g_platforms + i;
      if(CheckColision(item->item_config.colision, p->collision_platform.colision)){
        nPlatform = i;
        colisionWithPlatform = true;
      }
    }
    if(colisionWithPlatform){
      item->item_config.colision.p2.y = g_platforms[nPlatform].collision_platform.colision.p1.y+1;
      item->item_config.position.y = item->item_config.colision.p2.y - item->item_config.height;
    }
    else
      GravedadItem(item->item_config);
  }

  if (COL::CheckColision(player.config_colision.colision, item->item_config.colision))
  {
    item->recogido = true;
  }
  if (item->recogido)
  {
    timer += delta_time;
    if (timer >= item->cooldown)
    {
      timer = 0.0f;
      item->recogido = false;
      CambiarTipoItem(item, sprites);
      SpawnItem(item->item_config, nave);
    }
  }
}

void ColisionJugador(Jugador *player)
{
  player->config_colision.position.x = player->pos.x;
  player->config_colision.position.y = player->pos.y;
  // Crear la colisión
  player->config_colision.colision = COL::CreateColision(player->config_colision);
}

void AnimationDust(Jugador *player, bool isOnPlatform)
{
  if (isOnPlatform && player->volando) {
    float explode_x = player->pos.x + (player->spriteWidth * 0.5f) - 24.0f;
    float explode_y = player->pos.y + player->spriteHeight - 32.0f;
    ENE::ColorType color = (ENE::ColorType)(rand() % 4);
    
    ENE::ExplodeAt(explode_x, explode_y, color);
  }
}

void ColisionPlayerPlatforma(Jugador &player, TPlatform *g_platforms)
{
  const unsigned char kplatform_numbers = 3;
  for (int i = 0; i < kplatform_numbers; ++i)
  {
    TPlatform *p = g_platforms + i;

    if (CheckColision(player.config_colision.colision, p->collision_platform.colision))
    {
      if (player.config_colision.colision.p2.y >= p->collision_platform.colision.p1.y &&
          player.config_colision.colision.p2.y <= p->collision_platform.colision.p2.y)
      {

        player.pos.y = p->collision_platform.position.y - (float)player.spriteHeight;
        player.volando = false;
      }
      else if (player.config_colision.colision.p1.y <= p->collision_platform.colision.p2.y &&
               player.config_colision.colision.p1.y >= p->collision_platform.colision.p1.y)
      {
        player.pos.y = p->collision_platform.position.y + p->collision_platform.height;
      }
      else if (player.config_colision.colision.p2.x <= p->collision_platform.colision.p2.x &&
               player.config_colision.colision.p2.x >= p->collision_platform.colision.p1.x)
      {

        player.pos.x = p->collision_platform.colision.p1.x - player.config_colision.width;
      }
      else if (player.config_colision.colision.p1.x >= p->collision_platform.colision.p1.x &&
               player.config_colision.colision.p1.x <= p->collision_platform.colision.p2.x)
      {

        player.pos.x = p->collision_platform.colision.p2.x;
      }
    }
  }
}
// if(muerto == true || colisiona == false)
void ResetPlayer_OnDead(Jugador *player)
{
  static float timer = 0.0f;
  static float timer_invulnerable = 0.0f;

  if (player->muerto && timer > player->tiempo_aparicion)
    timer = 0.0f;

  if (timer <= player->tiempo_aparicion)
  {
    if (!timer)
    {
      player->pos.x = kScreenWidth / 2;
      player->pos.y = kScreenHeight - player->spriteHeight - 16;
      player->muerto = false;
    }

    timer += delta_time;
  }
  else
  {
    player->colisiona = false;
    timer_invulnerable += delta_time; // 0 1 2 3 4 5
    // no detectar colisiones con enemigos
    if (timer_invulnerable >= player->tiempo_invulnerable)
    {
      timer = 0.0f;
      timer_invulnerable = 0.0f;
      player->colisiona = true;
      // empezar a detectar colisiones con enemigos
    }
  }
}

void SwitchPlayer(Jugador *player){
  Jugador tmp;
  tmp = *player;
  LoadPlayerDataFromFile(player, player->player_id == 1 ? 2 : 1);
  SavePlayerDataToFile(&tmp, player);
}

void ColisionPlayerEnemigos(Jugador *player, ENE::EnemyManager *mgr, TGame *game){
  if (player->colisiona){
    for(int i = 0; i < mgr->pool_size; i++){
      ENE::Enemy *e = (mgr->pool + i);
      if (e->active){
        if (COL::CheckColision(player->config_colision.colision, e->col) && !player->muerto){
          player->vidas--;
          player->muerto = true;
          player->colisiona = false;
          e->active = false;

          SwitchPlayer(player);
          if(player->vidas <= 0){
            DeletePlayerDataFiles();
            game->current_screen = TScreen::MAIN_MENU;
          }
        }
      }
    }
  }
}

// void DebuggingCubo(object cubo, Sprites punteroSprites)
// {
//   esat::DrawSprite(punteroSprites.sprite, cubo.position.x, cubo.position.y);
// }  COL::SHOWCOLISION

/*int esat::main(int argc, char **argv)
{
  //AudioInit();
  esat::WindowInit(kScreenWidth, kScreenHeight);
  esat::WindowSetMouseVisibility(true);

  srand((unsigned)time(nullptr));
  last_time = esat::Time();

  // puntero a sprites
  Sprites *spritesColores = AsignarMemoriaSprites(4);
  Sprites *spritesPersonaje = AsignarMemoriaSprites(16);
  Sprites *spritesItems = AsignarMemoriaSprites(6);
  Bala *punteroBalas = AsignarMemoriaBalas(20);
  int contador_gasofa = 0;
  int numero_max_gasofa = 3;
  InstanciarSpritesColores(spritesColores);
  InstanciarSpritesPlayer(spritesPersonaje);
  InstanciarSpritesItems(spritesItems);
  InstanciarBalas(punteroBalas);

  Jugador player;
  InstanciarPlayer(&player);

  COL::object gasofa;
  COL::object prueba_nave;
  ItemDrop itemdrop;

  InstaciarGasofa_Nave(&gasofa, &prueba_nave, spritesItems[0]);
  InstanciarItems(&itemdrop, spritesItems);

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

    // ! Colisiones
    ColisionJugador(&player); // Actualizar colider a player
    ActualizarColisionesItems(&player, gasofa, prueba_nave, itemdrop);
    LoopGasofa(player, gasofa, prueba_nave, contador_gasofa, numero_max_gasofa);
    LoopPickItems(player, &itemdrop, spritesItems);
    //GameScreen();
    //ColisionPlayerPlatforma(player);
    // ColisionDisparos();
    EnemiesCollision();

    // Animaciones Dibujado
    int frame = ActualizarAnimacionJugador(player);
    DibujarDisparos(punteroBalas);
    DibujarColoresJugador(spritesColores, player, frame);
    DibujarJugador(spritesPersonaje, player, frame);
    DibujarGasofa(gasofa, spritesItems);
    DibujarItems(itemdrop, spritesItems);

    COL::ShowColision(player.config_colision.colision);
    COL::ShowColision(gasofa.colision);
    COL::ShowColision(punteroBalas->config_bala.colision);
    COL::ShowColision(prueba_nave.colision);
    // Finish drawing
    esat::DrawEnd();
    // Control frame speed (fps)
    do
    {
      current_time = esat::Time();
    } while ((current_time - last_time) <= 1000.0 / fps);
    esat::WindowFrame();
  }
  //FreeAudio();
  // Destroy window
  esat::WindowDestroy();
  return 0;
}*/



















////////////////////////// COLISION ITEMS PLATAFORMA //////////////////////////
/*

entonces en el merge tenemos que poner gplatforme en la llamada a funcion
  LoopGasofa(*player, gasofa, nave, g_platforms);
  LoopPickItems(*player, itemdrop, spritesItems, g_platforms);

void LoopGasofa(Jugador &player, COL::object &gasofa, Nave *nave, TPlatform* g_platforms)
{
  
  if(nave->direccion == Direction::STATIC){
    bool colisionWithPlatform = false;
    int nPlatform;
    // detecta colision con plataformas y guarda en caso de ser sierto
    for(int i = 0; i < 3; ++i)
    {
      TPlatform *p = g_platforms + i;
      if(CheckColision(gasofa.colision, p->collision_platform.colision)){
        nPlatform = i;
        colisionWithPlatform = true;
      }
    }

    if(colisionWithPlatform){
      gasofa.colision.p2.y = g_platforms[nPlatform].collision_platform.colision.p1.y+1;
      gasofa.position.y = gasofa.colision.p2.y - gasofa.height;
    }
    else{
      GravedadItem(gasofa);
    }

    if (COL::CheckColision(player.config_colision.colision, gasofa.colision))
    {
      if(!player.tiene_gasofa){player.puntos += 100;}
      player.tiene_gasofa = true;
      UpdateGasofaPosition(player, gasofa);
    }
    if (player.tiene_gasofa)
    {
      if (COL::CheckColision(player.config_colision.colision, nave->nave_config.colision))
      {
        player.tiene_gasofa = false;
        nave->fuelAmount++;
        SpawnItem(gasofa);
      }
    }
  }
}

void LoopPickItems(Jugador &player, ItemDrop *item, Sprites *sprites, TPlatform* g_platforms)
{
  static float timer = 0.0f;
  if (!item->recogido)
  {
    bool colisionWithPlatform = false;
    int nPlatform;
    // detecta colision con plataformas y guarda en caso de ser sierto
    for(int i = 0; i < 3; ++i)
    {
      TPlatform *p = g_platforms + i;
      if(CheckColision(item->item_config.colision, p->collision_platform.colision)){
        nPlatform = i;
        colisionWithPlatform = true;
      }
    }

    if(colisionWithPlatform){
      item->item_config.colision.p2.y = g_platforms[nPlatform].collision_platform.colision.p1.y+1;
      item->item_config.position.y = item->item_config.colision.p2.y - item->item_config.height;
    }
    else{
      GravedadItem(item->item_config);
    }
  }
    */