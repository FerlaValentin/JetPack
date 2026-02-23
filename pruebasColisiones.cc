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
    test.sprite = esat::SpriteFromFile("SPRITES/JUGADOR/amarillo1.png");
    test.position.x = 200;
    test.position.y = 300;
    test.width = esat::SpriteWidth(test.sprite);
    test.height = esat::SpriteHeight(test.sprite);

    test2.sprite = esat::SpriteFromFile("SPRITES/JUGADOR/amarillo1.png");
    test2.position.x = 200;
    test2.position.y = 300;
    test2.width = esat::SpriteWidth(test2.sprite);
    test2.height = esat::SpriteHeight(test2.sprite);

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

        mymouse = {(float)esat::MousePositionX(), (float)esat::MousePositionY()};
        test2.position = mymouse;

        test.colision = COL::CreateColision(test);
        test2.colision = COL::CreateColision(test2);

        esat::DrawSprite(test.sprite, test.position.x, test.position.y);
        esat::DrawSprite(test2.sprite, test2.position.x, test2.position.y);

        COL::ShowColision(test2.colision);
        COL::ShowColision(test.colision);

        if (COL::CheckColision(test.colision, test2.colision))
        {
            printf("A");
        }

        if(COL::WindowsColision(test2.colision,COL::top,0)){
            printf("ARRIBA\n");
        }
        if(COL::WindowsColision(test2.colision,COL::right,0)){
            printf("DERECHA\n");
        }
        if(COL::WindowsColision(test2.colision,COL::left,0)){
            printf("IZQUIERDA\n");
        }
        if(COL::WindowsColision(test2.colision,COL::down,0)){
            printf("ABAJO\n");
        }
        ////////////////////////////////

        esat::DrawEnd();
        esat::WindowFrame();
    }

    /////////////////////////
    esat::SpriteRelease(test.sprite);
    esat::SpriteRelease(test2.sprite);

    ENE::FreeManager(enemies);
    ENE::FreeVFX();
    /////////////////////////

    esat::WindowDestroy();

    return 0;
}