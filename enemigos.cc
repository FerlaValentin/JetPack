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
        meteorites,
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
        bool active;
    };

    struct EnemyManager{
        Enemy* pool=nullptr;
        int n_enemies;
    };
    
    
    void Initpool(EnemyManager **mgr){

        *mgr->pool = (Enemy*)malloc(8*sizeof(Enemy));
        
        for(int i=0;i<8;i++){
            *(mgr->pool+i).type = i;
        }

        *(mgr->pool + 0).data.sprite = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo1_1_2x.png");
        // *(mgr->sprites + 1) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo1_2_2x.png");
    
        *(mgr->pool + 1).data.sprite = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo2_1_2x.png");
        // *(mgr->sprites + 3) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo2_2_2x.png");
    
        *(mgr->pool + 2).data.sprite = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo3_1_2x.png");
        // *(mgr->sprites + 5) = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo3_2_2x.png");
    
        *(mgr->pool + 3).data.sprite = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo4_2x.png");
        *(mgr->pool + 4).data.sprite = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo5_2x.png");
        *(mgr->pool + 5).data.sprite = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo6_2x.png");
        *(mgr->pool + 6).data.sprite = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo7_2x.png");
        *(mgr->pool + 7).data.sprite = esat::SpriteFromFile("SPRITES/ENEMIGOS/enemigo8_2x.png");
    }
    



ENE::EnemyManager *enemies=nullptr;

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

        ///////////////////////////////



        
        ////////////////////////////////

        esat::DrawEnd();
        esat::WindowFrame();
    }

    /////////////////////////

    /////////////////////////

    esat::WindowDestroy();

    return 0;
}