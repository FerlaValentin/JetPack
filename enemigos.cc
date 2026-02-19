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

    enum Type{
        Meteorites,
        furballs,
        bubbles,
        darts,
        jets,
        ufo,
        flower,
        alien
    };

    struct Enemy{
        COL::object data;
        COL::vec2 speed;
        Type type;
        float counter;
        bool active;

        unsigned char r,g,b;

        int first_frame;
        int current_frame;
        int total_frames;
        float anim_timer;
        float anime_speed;
    };

    struct EnemyManager{
        Enemy* pool;
        int n_enemies;
        esat::SpriteHandle* sprites;
    };

    void Init(EnemyManager* mgr, int max_enemies){
        mgr->n_enemies = max_enemies;

        mgr->pool = (Enemy*)malloc(mgr->n_enemies*sizeof(Enemy));
        mgr->sprites = (esat::SpriteHandle*)malloc(11*sizeof(esat::SpriteHandle));

        for(int i=0;i<mgr->n_enemies;i++){
            (mgr->pool+i)->active = false;
            (mgr->pool+i)->data.sprite = nullptr;
        }

        *(mgr->sprites + 0) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo1_1_2x.png");
        *(mgr->sprites + 1) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo1_2_2x.png");

        *(mgr->sprites + 2) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo2_1_2x.png");
        *(mgr->sprites + 3) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo2_2_2x.png");

        *(mgr->sprites + 4) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo3_1_2x.png");
        *(mgr->sprites + 5) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo3_2_2x.png");

        *(mgr->sprites + 6) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo4_2x.png");
        *(mgr->sprites + 7) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo5_2x.png");
        *(mgr->sprites + 8) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo6_2x.png");
        *(mgr->sprites + 9) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo7_2x.png");
        *(mgr->sprites + 10) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo8_2x.png");
    }

    void SpawnEnemy(EnemyManager* mgr, Type type, float x, float y){
        for(int i=0;i<mgr->n_enemies;i++){
            Enemy* e = (mgr->pool + i);
            if(!e->active){
                e->active = true;
                e->type = type;
                e->data.position = {x,y};

                switch(type){
                    case Meteorites:
                    e->first_frame = 0;
                    e->total_frames = 2;
                    e->speed = {0.0f,1.5f};
                        break;
                    case furballs:
                    e->first_frame = 2;
                    e->total_frames = 2;
                    e->speed = {0.0f,1.5f};
                        break;
                    case bubbles:
                    e->first_frame = 4;
                    e->total_frames = 2;
                    e->speed = {0.0f,1.5f};
                        break;
                    case darts:
                    e->first_frame = 6;
                    e->total_frames = 1;
                    e->speed = {0.0f,1.5f};
                        break;
                    case jets:
                    e->first_frame = 7;
                    e->total_frames = 1;
                    e->speed = {0.0f,1.5f};
                        break;
                    case ufo:
                    e->first_frame = 8;
                    e->total_frames = 1;
                    e->speed = {0.0f,1.5f};
                        break;
                    case flower:
                    e->first_frame = 9;
                    e->total_frames = 1;
                    e->speed = {0.0f,1.5f};
                        break;
                    case alien:
                    e->first_frame = 10;
                    e->total_frames = 1;
                    e->speed = {0.0f,1.5f};
                        break;
                }

                e->current_frame = 0;
                e->anim_timer = 0.0f;
                e->anime_speed = 150.0f;

                e->data.sprite = *(mgr->sprites + e->first_frame);
                e->data.width = esat::SpriteWidth(e->data.sprite);
                e->data.height = esat::SpriteHeight(e->data.sprite);

                e->r = rand() % 256;
                e->g = rand() % 256;
                e->b = rand() % 256;

                e->data.colision = COL::CreateColision(e->data);
                return;
            }
        }
    }
    
    void Update(EnemyManager* mgr, float dt){
        for(int i=0;i<mgr->n_enemies;i++){
            Enemy* e = *(&mgr->pool + i);
            if(e->active){
                e->data.position.x += e->speed.x;
                e->data.position.y += e->speed.y;

                e->data.colision = COL::CreateColision(e->data);

                e->anim_timer += dt;
                if(e->anim_timer >= e->anime_speed){
                    e->current_frame++;
                    if(e->current_frame >= e->total_frames){
                        e->current_frame = 0;
                    }

                    e->data.sprite = mgr->sprites[e->first_frame + e->current_frame];
                    e->anim_timer = 0;
                }
            }
        }
    }

    void Draw(EnemyManager* mgr, bool debug){
        for(int i=0;i<mgr->n_enemies;i++){
            Enemy* e = *(&mgr->pool + i);
            if(e->active){

                float points[] = {
                    e->data.position.x, e->data.position.y,                         
                    e->data.position.x + e->data.width, e->data.position.y,         
                    e->data.position.x + e->data.width, e->data.position.y + e->data.height, 
                    e->data.position.x, e->data.position.y + e->data.height         
                };

                esat::DrawSetFillColor(e->r, e->g, e->b, 255); 
                esat::DrawSolidPath(points, 4, false);                

                esat::DrawSprite(e->data.sprite, e->data.position.x, e->data.position.y);
            }

            if(debug){
                COL::ShowColision(e->data.colision);
            }
        }
    }

    void Free(EnemyManager* mgr){
        if(mgr->pool != nullptr){
            free(mgr->pool);
            mgr->pool = nullptr;
        }
        if(mgr->sprites != nullptr){
            free(mgr->sprites);
            mgr->sprites = nullptr;
        }
    }

}


int esat::main(int argc, char **argv)
{

    esat::WindowInit(800, 800);
    WindowSetMouseVisibility(true);

    //////////////////////////////

    /////////////////////////////

    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
    {

        esat::DrawBegin();
        esat::DrawClear(0, 0, 0);

        ////////////////////////////////

        ////////////////////////////////

        esat::DrawEnd();
        esat::WindowFrame();
    }

    /////////////////////////

    /////////////////////////

    esat::WindowDestroy();

    return 0;
}