#ifndef JUGADOR_H
#define JUGADOR_H

#include <esat/sprite.h>
#include <esat/draw.h>
#include "colisiones.h"
#include "nave.h"
#include "enemigos.h"
#include "interface.h"

struct Bala;
struct ItemDrop;
struct TPlatform;
struct TGame;

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
    int level;
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

// --- Jugador ---
void InstanciarPlayer(Jugador *player, int id = 1);
void InstanciarSpritesPlayer(Sprites *punteroSprites);
void MoverJugador(Jugador *jugador, bool moverLeft, bool moverRight);
void LoopMoverJugador(bool moverLeft, bool moverRight, Jugador *player);
void Ascender_Gravedad(Jugador *jugador, bool ascendiendo);
void ColisionJugador(Jugador *player);
bool SwitchPlayer(Jugador *player);
void ControlarLimitesPantalla(Jugador *player, Bala *bala);
int ActualizarAnimacionJugador(Jugador jugador);
void ResetPlayer_OnDead(Jugador *player, bool *ascender, bool *izquierda, bool *derecha, bool* vida_perdida);
void ColisionPlayerPlatforma(Jugador &player, TPlatform *g_platforms);

// enemigos
void EnemiesCollision(ENE::EnemyManager *mgr, Jugador *player, ItemDrop *gasofa, int frame, TGame *game, ENE::VisualEffect *g_fx_pool_pointer, esat::SpriteHandle *g_fx_sprites_pointer);

// --- Balas ---
void InstanciarBalas(Bala *bala);
void CrearDisparos(Bala *bala, Jugador player);
void ActualizarDisparos(Bala *bala, Jugador player);
void DibujarDisparos(Bala *bala);
void ColisionDisparos(Bala *bala, ENE::EnemyManager *punteroEnemy, ENE::VisualEffect *g_fx_pool_pointer, esat::SpriteHandle *g_fx_sprites_pointer);

// --- Items ---
void InstanciarItems(ItemDrop *item, Sprites *punteroSprites);
void InstanciarSpritesItems(Sprites *punteroSprites);
void InstaciarGasofa_Nave(ItemDrop *gasofa, Sprites punteroSprites);
void InstanciarSpritesColores(Sprites *punteroSprites);
void LoopGasofa(Jugador &player, ItemDrop &gasofa, Nave *nave, TPlatform *g_platforms);
void LoopPickItems(Jugador &player, ItemDrop *item, Sprites *sprites, Nave nave, TPlatform *g_platforms);
void ActualizarColisionesItems(ItemDrop &gasofa, ItemDrop &item, Nave *nave);
void AnimationDust(Jugador *player, bool isOnPlatform, ENE::VisualEffect *g_fx_pool_pointer, esat::SpriteHandle *g_fx_sprites_pointer);
// Dibujar
void DibujarColoresJugador(Sprites *punteroSprites, Jugador jugador, int frame);
void DibujarJugador(Sprites *punteroSprites, Jugador jugador, int frame);
void DibujarItems(ItemDrop item, Sprites *punteroSprites);
void DibujarGasofa(ItemDrop gasofa, Sprites *punteroSprites, Nave nave);

#endif