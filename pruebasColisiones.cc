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

COL::object test, test2;
COL::vec2 mymouse;

int esat::main(int argc, char **argv)
{

    esat::WindowInit(800, 800);
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
    /////////////////////////////

    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
    {

        esat::DrawBegin();
        esat::DrawClear(0, 0, 0);

        ////////////////////////////////
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

        if(COL::WindowsColision(test2.colision,COL::top)){
            printf("ARRIBA\n");
        }
        if(COL::WindowsColision(test2.colision,COL::right)){
            printf("DERECHA\n");
        }
        if(COL::WindowsColision(test2.colision,COL::left)){
            printf("IZQUIERDA\n");
        }
        if(COL::WindowsColision(test2.colision,COL::down)){
            printf("ABAJO\n");
        }

        ////////////////////////////////

        esat::DrawEnd();
        esat::WindowFrame();
    }

    /////////////////////////
    esat::SpriteRelease(test.sprite);
    esat::SpriteRelease(test2.sprite);
    /////////////////////////

    esat::WindowDestroy();

    return 0;
}