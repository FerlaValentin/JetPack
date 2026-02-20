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
        green,
        pink,
        blue,
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
        ColorType color;
        bool active;
    };
    
    VisualEffect *g_fx_pool = nullptr;
    esat::SpriteHandle *g_fx_sprites = nullptr;
    ENE::EnemyManager  *enemies = (ENE::EnemyManager*)malloc(sizeof(ENE::EnemyManager));

    void InitVFXSystem(){
        g_fx_pool = (VisualEffect*)malloc(20*sizeof(VisualEffect));
        g_fx_sprites = (esat::SpriteHandle*)malloc(3*sizeof(esat::SpriteHandle));

        *(g_fx_sprites+0) = esat::SpriteFromFile("SPRITES/NAVE/nube_polvo_1_2x.png");
        *(g_fx_sprites+1) = esat::SpriteFromFile("SPRITES/NAVE/nube_polvo_2_2x.png");
        *(g_fx_sprites+2) = esat::SpriteFromFile("SPRITES/NAVE/nube_polvo_3_2x.png");
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

    void ExplodeAt(float x, float y, ColorType color){
        bool found = false;
        for(int i=0;i<20 && !found;i++){
            if(!(g_fx_pool+i)->active){
                (g_fx_pool+i)->x = x;
                (g_fx_pool+i)->y = y;
                (g_fx_pool+i)->startTime = esat::Time();
                (g_fx_pool+i)->color = color;
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
                (mgr->pool+i)->Color = static_cast<ENE::ColorType>(rand()%4);
                found = true;
            }
        }
    }
    
    void EnemiesAI(Enemy *e, COL::colision ecol){
        if (COL::WindowsColision(ecol,COL::right,100)){e->position.x=-32;}
        if (COL::WindowsColision(ecol,COL::left,100) && !e->type == KMeteorites){e->position.x=799;}
        if (COL::WindowsColision(ecol,COL::down,0) && e->type == KMeteorites){
            e->active=false;
            ExplodeAt(e->position.x,e->position.y, e->Color);
            SpawnEnemy(enemies,KMeteorites,-32,rand()%360);
        }       
    }

    void BGcolor(COL::colision col, ColorType type){
        float *P = (float*)malloc(10*sizeof(float));

        switch(type){
            case red:
            esat::DrawSetFillColor(255,0,0,255);
                break;

            case green:
            esat::DrawSetFillColor(0,255,0,255);
                break;

            case pink:
            esat::DrawSetFillColor(228,0,124,255);
                break;

            case blue:
            esat::DrawSetFillColor(52,198,233,255);
                break;
        }

        *(P+0)=col.p1.x; *(P+1)=col.p1.y+3;
        *(P+2)=col.p2.x; *(P+3)=col.p1.y+3;
        *(P+4)=col.p2.x; *(P+5)=col.p2.y-3;
        *(P+6)=col.p1.x; *(P+7)=col.p2.y-3;
        *(P+8)=*(P+0); *(P+9)=*(P+1);

        esat::DrawSolidPath(P,5, false);
        free(P);
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

                BGcolor(enemycol, e->Color);
                esat::DrawSprite(myTemplate->sprite, (mgr->pool+i)->position.x, (mgr->pool+i)->position.y);

                EnemiesAI(e,enemycol);
            }
        }
    }

    void DrawActiveVFX(){
        for(int i=0;i<20;i++){
            VisualEffect *fx = (g_fx_pool + i);
            COL:: colision col = {fx->x,fx->y,fx->x+48,fx->y+32};

            if(fx->active){
                int frame = (int)((esat::Time() - fx->startTime)/100);

                if(frame < 3){
                    BGcolor(col,fx->color);
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

    esat::WindowInit(256*2,192*2);
    WindowSetMouseVisibility(true);

    //////////////////////////////
    srand(time(NULL));
    ENE::InitManager(ENE::enemies,10);
    ENE::InitVFXSystem();
    /////////////////////////////

    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
    {
        esat::DrawBegin();
        esat::DrawClear(0, 0, 0);

        ///////////////////////////////

        if(level == 1 && !toogle){
            ENE::ResetEnemies(ENE::enemies);
            for(int i=0;i<3;i++){
                ENE::SpawnEnemy(ENE::enemies,ENE::KMeteorites,-32,rand()%360);
            }
            toogle = true;
        }
        if(level == 2 && toogle){
            ENE::ResetEnemies(ENE::enemies);
            for(int i=0;i<3;i++){
                ENE::SpawnEnemy(ENE::enemies,ENE::KMeteorites,-32,rand()%360);
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