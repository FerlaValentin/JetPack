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
#include "enemigos.h"

ENE::EnemyManager *enemies = (ENE::EnemyManager*)malloc(sizeof(ENE::EnemyManager));
COL::object test, test2;
COL::vec2 mymouse;

int esat::main(int argc, char **argv)
{

    esat::WindowInit(256*2,192*2);
    WindowSetMouseVisibility(true);

    //////////////////////////////

    srand(time(NULL));
    ENE::InitManager(enemies,10);
    ENE::InitVFXSystem();
    ENE::SpawnEnemy(enemies,ENE::KMeteorites,-32,rand()%250);

    /////////////////////////////

    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
    {

        esat::DrawBegin();
        esat::DrawClear(0, 0, 0);

        ////////////////////////////////
        ENE::UpdateAndDraw(enemies);
        ENE::DrawActiveVFX();
        ////////////////////////////////

        esat::DrawEnd();
        esat::WindowFrame();
    }

    esat::WindowDestroy();

    /////////////////////////

    ENE::FreeManager(enemies);
    ENE::FreeVFX();
    /////////////////////////

    return 0;
}