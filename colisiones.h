//supuestamente esto es pa que todos los archivos .cc que hayan incluido este header no lo compilen por cada archivo
#pragma once 
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

namespace COL{

    struct vec2
    {
        float x, y;
    };

    struct colision
    {
        vec2 p1, p2;
    };

    struct object
    {
        esat::SpriteHandle sprite;
        int width, height;
        vec2 position;
        colision colision;
    };

    enum dir
    {
        top,
        down,
        left,
        right,
    };

    colision CreateColision(object object)
    {
        colision final;
        final.p1.x = object.position.x;
        final.p1.y = object.position.y;
        final.p2.x = object.position.x + object.width;
        final.p2.y = object.position.y + object.height;
        return final;
    }

    bool CheckColision(colision colision1, colision colision2)
    {
        if (colision1.p2.x > colision2.p1.x && colision1.p1.x < colision2.p2.x && colision1.p2.y > colision2.p1.y && colision1.p1.y < colision2.p2.y)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool WindowsColision(colision colision1, dir dir){
        colision screen={0,0,800,800};

        switch(dir){
            case left:
            if (colision1.p1.x < screen.p1.x)
            {return true;}
            else
            {return false;}
            break;

            case down:
            if (colision1.p2.y > screen.p2.y)
            {return true;}
            else
            {return false;}
            break;

            case top:
            if (colision1.p1.y < screen.p1.y)
            {return true;}
            else
            {return false;}
            break;

            case right:
            if (colision1.p2.x > screen.p2.x)
            {return true;}
            else
            {return false;}
            break;
        }
    }

    void ShowColision(colision colision){
        esat::DrawSetStrokeColor(255,255,255,255);

        esat::DrawLine(colision.p1.x,colision.p1.y,colision.p2.x,colision.p1.y);
        esat::DrawLine(colision.p2.x,colision.p1.y,colision.p2.x,colision.p2.y);
        esat::DrawLine(colision.p2.x,colision.p2.y,colision.p1.x,colision.p2.y);
        esat::DrawLine(colision.p1.x,colision.p2.y,colision.p1.x,colision.p1.y);
    }

}