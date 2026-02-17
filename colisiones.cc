// ESAT Libraries
#include <esat/window.h>
#include <esat/draw.h>
#include <esat/sprite.h>
#include <esat/input.h>
#include <esat/time.h>

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

struct vec2{
    float x, y;
};

struct colision{
    float x1, y1, x2, y2;
};

struct object{
    esat::SpriteHandle sprite;
    int width, height;
    vec2 position;
    colision colision;
};

enum dir{
    top,
    down,
    left,
    right,
};

object test,test2;
vec2 mymouse;

colision CreateColision(object object){
    colision final;
    final.x1 = object.position.x;
    final.y1 = object.position.y;
    final.x2 = object.position.x + object.width;
    final.y2 = object.position.y + object.height;
    return final;
}

bool CheckColision(colision colision1, colision colision2){
    if(colision1.x2 > colision2.x1 && colision1.x1 < colision2.x2 && colision1.y2 > colision2.y1 && colision1.y1 < colision2.y2){
        return true;
    }else{
        return false;
    }
}


// void WindowsColision(int width, int height, Object object, dir dir){
//     switch(dir){
//         case top:

//         break;

//         case down:
//         break;

//         case left:
//         break;

//         case right:
//         break;

//         case default:
//         break;
//     }
// }


int esat::main(int argc, char **argv){
    
	esat::WindowInit(800,800);
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

    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)){

        esat::DrawBegin();
        esat::DrawClear(0,0,0);

        ////////////////////////////////
        mymouse={(float)esat::MousePositionX(),(float)esat::MousePositionY()};
        test2.position = mymouse;

        test.colision = CreateColision(test);
        test2.colision = CreateColision(test2);

        esat::DrawSprite(test.sprite,test.position.x,test.position.y);
        esat::DrawSprite(test2.sprite,test2.position.x,test2.position.y);

        if(CheckColision(test.colision,test2.colision)){
            printf("A");
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