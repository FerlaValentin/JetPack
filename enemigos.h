#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include "colisiones.h"

namespace ENE
{

    enum EnemyType
    {
        KMeteorites,
        KFurballs,
        KBubbles,
        KDarts,
        KJets,
        KUfo,
        KFlower,
        KAlien,
        KTypeCount
    };

    enum ColorType
    {
        yellow,
        red,
        blue,
        green,
        pink,
    };

    struct EnemyTemplate
    {
        esat::SpriteHandle *sprite = nullptr;
        int width, height;
        int num_frames;
        COL::vec2 speed;
    };

    struct Enemy
    {
        COL::vec2 position;
        COL::vec2 speed;
        EnemyType type;
        ColorType Color;
        int count;
        bool active;
        bool iscolliding;
        COL::colision col;
    };

    struct EnemyManager
    {
        Enemy *pool = nullptr;
        int pool_size;
        EnemyTemplate *templates = nullptr;
    };

    struct VisualEffect
    {
        float x, y;
        float startTime;
        ColorType color;
        bool active;
    };

    void InitVFXSystem(VisualEffect **g_fx_pool_pointer, esat::SpriteHandle **g_fx_sprites_pointer);
    void InitManager(EnemyManager **mgr, int pool_capacity);
    void ExplodeAt(float x, float y, ColorType color, VisualEffect *g_fx_pool_pointer, esat::SpriteHandle *g_fx_sprites_pointer);
    void SpeedEnemies(Enemy *e);
    void SpawnEnemy(EnemyManager *mgr, EnemyType type, float x, float y);
    void EnemiesAI(Enemy *e, COL::colision ecol, EnemyManager *mgr, VisualEffect *g_fx_pool_pointer, esat::SpriteHandle *g_fx_sprites_pointer);
    void BGcolor(COL::colision col, ColorType type);
    void UpdateEnemies(EnemyManager *mgr, VisualEffect *g_fx_pool_pointer, esat::SpriteHandle *g_fx_sprites_pointer);
    void DrawEnemies(EnemyManager mgr);
    void DrawActiveVFX(VisualEffect *g_fx_pool, esat::SpriteHandle *g_fx_sprites);
    void FreeManager(EnemyManager **mgr);
    void FreeVFX(VisualEffect **g_fx_pool_pointer, esat::SpriteHandle **g_fx_sprites_pointer);
    void HowManyEnemies(EnemyManager *mgr);
    void ResetEnemies(EnemyManager *mgr);

}

#endif
