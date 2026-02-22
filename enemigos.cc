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
        esat::SpriteHandle* sprite = nullptr;
        int width, height;
        int num_frames;
        COL::vec2 speed;
    };

    struct Enemy{
        COL::vec2 position;
        COL::vec2 speed;
        EnemyType type;
        ColorType Color;
        int count;
        bool active;
        COL::colision col;
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

        for(int i=0; i<KTypeCount; i++){
            if(i == 0 || i == 1 || i == 2){
                (mgr->templates+i)->num_frames = 2;
            }else{
                (mgr->templates+i)->num_frames = 1;
            }

            (mgr->templates+i)->sprite = (esat::SpriteHandle*)malloc((mgr->templates+i)->num_frames*sizeof(esat::SpriteHandle));
        }


        *((mgr->templates+0)->sprite+0) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo1_1_2x.png");
        *((mgr->templates+0)->sprite+1) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo1_2_2x.png");

        *((mgr->templates+1)->sprite+0) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo2_1_2x.png");
        *((mgr->templates+1)->sprite+1) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo2_2_2x.png");

        *((mgr->templates+2)->sprite+0) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo3_1_2x.png");
        *((mgr->templates+2)->sprite+1) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo3_2_2x.png");

        *((mgr->templates+3)->sprite+0) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo7_2x.png");
        *((mgr->templates+4)->sprite+0) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo4_2x.png");
        *((mgr->templates+5)->sprite+0) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo5_2x.png");
        *((mgr->templates+6)->sprite+0) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo6_2x.png");
        *((mgr->templates+7)->sprite+0) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo8_2x.png");



        for(int i=0;i<KTypeCount;i++){
            (mgr->templates+i)->width = esat::SpriteWidth(*((mgr->templates+i)->sprite));
            (mgr->templates+i)->height = esat::SpriteHeight(*((mgr->templates+i)->sprite));
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

        float speedX = ((float)rand() / (float)RAND_MAX) + 0.5f;
        float speedY = ((float)rand() / (float)RAND_MAX) + 0.5f;

        if(rand() % 2 == 0) speedX *= -1;
        if(rand() % 2 == 0) speedY *= -1;

        switch(e->type){
            case KMeteorites:
                if(speedX < 0){speedX*=-1;}
                if(rand()%2 == 1){
                    e->speed={speedX,0.3}; 
                }else{
                    e->speed={speedX,0};   
                }
                break;

            case KFurballs:
                e->speed={speedX,speedY};
                break;

            case KBubbles:
                e->speed={speedX,speedY};
                break;

            case KDarts:
                if(rand()%2 == 1){
                    e->speed={speedX,0.3}; 
                }else{
                    e->speed={speedX,0};   
                }
                break;

            case KJets:
                if(speedX < 0){
                    e->speed={0,speedY};
                }else{
                    e->speed={0,speedY};
                }
                break;

            case KUfo:
                e->speed={speedX,speedY};
                break;

            case KFlower:
                e->speed={speedX,speedY};
                break;

            case KAlien:
                if(speedY < 0){
                    e->speed={speedX,speedY};
                }else{
                    e->speed={speedX,-speedY};
                }
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
                (mgr->pool+i)->count = 0;
                (mgr->pool+i)->Color = static_cast<ENE::ColorType>(rand()%4);
                found = true;
            }
        }
    }
    
    void EnemiesAI(Enemy *e, COL::colision ecol){
        if(e->type == KMeteorites || e->type == KDarts){
            if(e->type == KMeteorites){

            }
            if (COL::WindowsColision(ecol,COL::down,0)){
                e->active=false;
                ExplodeAt(e->position.x,e->position.y, e->Color);
                if(e->type == KMeteorites){
                    SpawnEnemy(enemies,KMeteorites,-32,rand()%360);
                }else{
                    SpawnEnemy(enemies,KDarts,-32,rand()%360);
                }

            }     
            if (COL::WindowsColision(ecol,COL::right,100)){e->position.x=-32;}
        }
        else if(e->type == KFurballs || e->type == KBubbles || e->type == KFlower){
            if (COL::WindowsColision(ecol,COL::right,100)){e->position.x=-32;}
            if (COL::WindowsColision(ecol,COL::left,100)){e->position.x=(256*2)+32;}
            if (COL::WindowsColision(ecol,COL::top,0) || COL::WindowsColision(ecol,COL::down,0)){e->speed.y *= -1;}
        }
        else if(e->type == KAlien){
            if (COL::WindowsColision(ecol,COL::right,100)){e->position.x=-32;}
            if (COL::WindowsColision(ecol,COL::left,100)){e->position.x=(256*2)+32;}
            if (e->position.y < rand()%(192*2)-80 && e->speed.y < 0 || COL::WindowsColision(ecol,COL::down,0)){e->speed.y *= -1;}
        }
        else if(e->type == KUfo) {
            if (e->speed.y < 0) {
                if (COL::WindowsColision(ecol, COL::top, 0)) {
                    e->speed.y = 0.5f;
                }
            } 
            if (e->speed.y > 0) {

                e->speed.x = sinf(esat::Time() * 0.005f) * 1.5f; 
                
                if (COL::WindowsColision(ecol, COL::down, 0)) {
                    e->speed.y = -1.0f;
                }
            }
            if (COL::WindowsColision(ecol,COL::right,100)){e->position.x=-32;}
            if (COL::WindowsColision(ecol,COL::left,100)){e->position.x=(256*2)+32;}
        }
        else if(e->type == KJets) {
            e->count++;
            if(e->count < 300) {
                e->speed.x=0;
                e->speed.y = sinf(esat::Time() * 0.005f) * 1.0f; 
            } 
            else {
                e->speed.x = 1.0f;
            }
            if (COL::WindowsColision(ecol, COL::down, 0) || COL::WindowsColision(ecol, COL::top, 0) || COL::WindowsColision(ecol,COL::right,0)) {
                e->active = false;
                ExplodeAt(e->position.x, e->position.y, e->Color);
                SpawnEnemy(enemies, KJets, 0, rand() % 360);
                e->count = 0;
            }
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
        *(P+0)=col.p1.x+1; *(P+1)=col.p1.y+1;
        *(P+2)=col.p2.x-1; *(P+3)=col.p1.y+1;
        *(P+4)=col.p2.x-1; *(P+5)=col.p2.y-1;
        *(P+6)=col.p1.x+1; *(P+7)=col.p2.y-1;
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

                e->col = COL::CreateColision(tempobj);

                BGcolor(e->col, e->Color);

                ENE::EnemyTemplate *t = &mgr->templates[e->type];
                int frame = (int)(esat::Time() / 200) % t->num_frames;

                esat::DrawSprite(*(t->sprite + frame), e->position.x, e->position.y);

                EnemiesAI(e,e->col);
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
            EnemyTemplate *t = (mgr->templates + i);
            if (t->sprite != nullptr) {
                for(int j=0; j < t->num_frames; j++){
                    esat::SpriteRelease(*(t->sprite + j));
                }
                free(t->sprite); 
            }
        }
        if (mgr->pool != nullptr) free(mgr->pool);
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



// int level = 1;
// bool toogle = false; 

// int esat::main(int argc, char **argv)
// {

//     esat::WindowInit(256*2,192*2);
//     WindowSetMouseVisibility(true);

//     //////////////////////////////
//     srand(time(NULL));
//     ENE::InitManager(ENE::enemies,10);
//     ENE::InitVFXSystem();
//     /////////////////////////////

//     while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
//     {
//         esat::DrawBegin();
//         esat::DrawClear(0, 0, 0);

//         ///////////////////////////////

//         if(level == 1 && !toogle){
//             ENE::ResetEnemies(ENE::enemies);
//             for(int i=0;i<3;i++){
//                 ENE::SpawnEnemy(ENE::enemies,ENE::KMeteorites,0,rand()%350);
//             }
//             for(int i=0;i<3;i++){
//                 ENE::SpawnEnemy(ENE::enemies,ENE::KFurballs,-32,rand()%350);
//             }
//             toogle = true;
//         }
//         if(level == 2 && toogle){
//             ENE::ResetEnemies(ENE::enemies);
//             for(int i=0;i<3;i++){
//                 ENE::SpawnEnemy(ENE::enemies,ENE::KUfo,-32,rand()%360);
//             }
//             toogle = false;
//         }

//         if(esat::MouseButtonDown(0)){
//             level++;
//         }

//         ENE::UpdateAndDraw(ENE::enemies);
//         ENE::DrawActiveVFX();

//         ////////////////////////////////

//         esat::DrawEnd();
//         esat::WindowFrame();
//     }

//     esat::WindowDestroy();

//     /////////////////////////
//     ENE::FreeManager(ENE::enemies);
//     ENE::FreeVFX();
//     /////////////////////////

//     return 0;
// }