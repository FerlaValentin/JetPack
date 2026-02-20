// ESAT Libraries
#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>

// Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Group Libraries
#include "colisiones.h"

namespace ENE{

    enum EnemyType{
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

    enum ColorType{
        red,
        gree,
        yellow,
        blue
    };

    struct EnemyTemplate{
        esat::SpriteHandle sprite;
        int width, height;
        COL::vec2 speed;
    };

    struct Enemy{
        COL::vec2 position;
        COL::vec2 speed;
        EnemyType type;
        ColorType Color;
        bool active;
    };

    struct EnemyManager{
        Enemy* pool=nullptr;
        int pool_size;
        EnemyTemplate templates[KTypeCount];
    };

    struct VisualEffect{
        float x,y;
        float startTime;
        bool active;
    };
    
    VisualEffect *g_fx_pool = (VisualEffect*)malloc(20*sizeof(VisualEffect));
    esat::SpriteHandle *g_fx_sprites = (esat::SpriteHandle*)malloc(3*sizeof(esat::SpriteHandle));
    ENE::EnemyManager  *enemies = (ENE::EnemyManager*)malloc(sizeof(ENE::EnemyManager));

    void InitVFXSystem(){
        *(g_fx_sprites+0) = esat::SpriteFromFile("SPRITES/VFX_1_2x.png");
        *(g_fx_sprites+1) = esat::SpriteFromFile("SPRITES/VFX_2_2x.png");
        *(g_fx_sprites+2) = esat::SpriteFromFile("SPRITES/VFX_3_2x.png");
        for(int i=0;i<20;i++) (g_fx_pool+i)->active = false;
    }

    void InitManager(EnemyManager *mgr, int pool_capacity){

        char* paths[] = {
        "SPRITES/ENEMIGOS/enemigo1_1_2x.png", "SPRITES/ENEMIGOS/enemigo2_1_2x.png",
        "SPRITES/ENEMIGOS/enemigo3_1_2x.png", "SPRITES/ENEMIGOS/enemigo4_2x.png",
        "SPRITES/ENEMIGOS/enemigo5_2x.png",   "SPRITES/ENEMIGOS/enemigo6_2x.png",
        "SPRITES/ENEMIGOS/enemigo7_2x.png",   "SPRITES/ENEMIGOS/enemigo8_2x.png"
        };

        for(int i=0;i<KTypeCount;i++){
            (mgr->templates+i)->sprite = esat::SpriteFromFile(paths[i]);
            (mgr->templates+i)->width = esat::SpriteWidth((mgr->templates+i)->sprite);
            (mgr->templates+i)->height = esat::SpriteHeight((mgr->templates+i)->sprite);
        }

        mgr->pool_size = pool_capacity;
        mgr->pool = (Enemy*)malloc(pool_capacity*sizeof(Enemy));

        for(int i=0;i<pool_capacity;i++){
            (mgr->pool+i)->active = false;
        }
    }

    void ExplodeAt(float x, float y){
        bool found = false;
        for(int i=0;i<20 && !found;i++){
            if(!(g_fx_pool+i)->active){
                (g_fx_pool+i)->x = x;
                (g_fx_pool+i)->y = y;
                (g_fx_pool+i)->startTime = esat::Time();
                (g_fx_pool+i)->active = true;
                found = true;
            }
        }
    }

    void SpeedEnemies(Enemy *e){
        float Value = (float)(rand() % 1 + 1);
        switch(e->type){
            case KMeteorites:
                if(rand()%2 == 1){
                    e->speed={Value,0.3}; 
                }else{
                    e->speed={Value,0};   
                }
                break;

            case KFurballs:
                break;

            case KBubbles:
                break;

            case KDarts:
                break;

            case KJets:
                break;

            case KUfo:
                break;

            case KFlower:
                break;

            case KAlien:
                break;
        }
    }

    void SpawnEnemy(EnemyManager *mgr,EnemyType type, float x, float y){
        bool found = false;
        for(int i=0;i<mgr->pool_size && !found;i++){
            if(!(mgr->pool+i)->active){
                (mgr->pool+i)->active = true;
                (mgr->pool+i)->type = type;
                (mgr->pool+i)->position = {x,y};
                SpeedEnemies((mgr->pool+i));

                found = true;
            }
        }
    }
    
    void EnemiesAI(Enemy *e, COL::colision ecol){
        if (COL::WindowsColision(ecol,COL::right,100)){e->position.x=-32;}
        if (COL::WindowsColision(ecol,COL::left,100) && !e->type == KMeteorites){e->position.x=799;}
        if (COL::WindowsColision(ecol,COL::down,0) && e->type == KMeteorites){
            e->active=false;
            ExplodeAt(e->position.x,e->position.y);
            SpawnEnemy(enemies,KMeteorites,-32,rand()%151+500);
        }       
    }

    void UpdateAndDraw(EnemyManager *mgr){
        for(int i=0;i<mgr->pool_size;i++){
            Enemy *e = &(*(mgr->pool+i));

            if(e->active){

                e->position.y += e->speed.y;
                e->position.x += e->speed.x;

                ENE::EnemyTemplate *myTemplate = &mgr->templates[e->type];

                COL::object tempobj;
                tempobj.position = e->position;
                tempobj.width = myTemplate->width;
                tempobj.height = myTemplate->height;

                COL::colision enemycol = COL::CreateColision(tempobj);

                COL::ShowColision(enemycol);
                esat::DrawSprite(myTemplate->sprite, (mgr->pool+i)->position.x, (mgr->pool+i)->position.y);

                EnemiesAI(e,enemycol);
            }
        }
    }

    void DrawActiveVFX(){
        for(int i=0;i<20;i++){
            VisualEffect *fx = (g_fx_pool + i);

            if(fx->active){
                int frame = (int)((esat::Time() - fx->startTime)/100);

                if(frame < 3){
                    esat::DrawSprite(*(g_fx_sprites+frame),fx->x,fx->y);
                }else{
                    fx->active = false;
                }
            }
        }
    }

    void FreeManager(EnemyManager *mgr) {
        if (mgr == nullptr) return;

        for (int i = 0; i < KTypeCount; i++) {
            if ((mgr->templates+i)->sprite != nullptr) {
                esat::SpriteRelease((mgr->templates+i)->sprite);
            }
        }

        if (mgr->pool != nullptr) {
            free(mgr->pool);
            mgr->pool = nullptr;
        }

        free(mgr);
    }

    void FreeVFX(){
        for(int i=0;i<3;i++){
            if(*(g_fx_sprites+i) != nullptr){
                esat::SpriteRelease(g_fx_sprites+i);
            }
        }

        if (g_fx_sprites != nullptr) {
            free(g_fx_sprites);
            g_fx_sprites = nullptr;
        }

        if (g_fx_pool != nullptr) {
            free(g_fx_pool);
            g_fx_pool = nullptr;
        }
    }

    void HowManyEnemies(EnemyManager *mgr){
        int num=0;
        for(int i=0;i<mgr->pool_size;i++){
            if((mgr->pool+i)->active) num++;
        }
        printf("Enemigos en pantalla %d\n", num);
    }

    void ResetEnemies(EnemyManager *mgr){
        for(int i=0;i<mgr->pool_size;i++){
            (mgr->pool+i)->active = false;
        }
    }
}

int level = 1;
bool toogle = false; 


int esat::main(int argc, char **argv)
{

    esat::WindowInit(800, 800);
    WindowSetMouseVisibility(true);

    //////////////////////////////
    srand(time(NULL));
    ENE::InitManager(ENE::enemies,10);
    ENE::InitVFXSystem();
    /////////////////////////////

    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
    {

        esat::DrawBegin();
        esat::DrawClear(0, 0, 255);

        ///////////////////////////////

        if(level == 1 && !toogle){
            ENE::ResetEnemies(ENE::enemies);
            for(int i=0;i<3;i++){
                ENE::SpawnEnemy(ENE::enemies,ENE::KMeteorites,-32,rand()%151+500);
            }
            toogle = true;
        }
        if(level == 2 && toogle){
            ENE::ResetEnemies(ENE::enemies);
            for(int i=0;i<3;i++){
                ENE::SpawnEnemy(ENE::enemies,ENE::KMeteorites,-32,rand()%151+500);
            }
            toogle = false;
        }

        if(esat::MouseButtonDown(0)){
            level++;
        }

        ENE::UpdateAndDraw(ENE::enemies);
        ENE::DrawActiveVFX();
        ////////////////////////////////

        esat::DrawEnd();
        esat::WindowFrame();
    }

    esat::WindowDestroy();

    /////////////////////////
    ENE::FreeManager(ENE::enemies);
    ENE::FreeVFX();
    /////////////////////////

    return 0;
}