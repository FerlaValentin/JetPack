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

#ifndef COLISIONES_H
#define COLISIONES_H

namespace COL
{

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

    colision CreateColision(object object);

    bool CheckColision(colision colision1, colision colision2);

    bool WindowsColision(colision colision1, dir dir, int offset);

    bool SpecificColision(colision colision1, colision colision2, dir dir);

    void ShowColision(colision colision);

}

#endif