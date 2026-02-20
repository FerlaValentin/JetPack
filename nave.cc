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


void LoadShipSprites(esat::SpriteHandle* nave1, esat::SpriteHandle* nave2, esat::SpriteHandle* nave3,
                  esat::SpriteHandle* rosa){
    *nave1 = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_1_2x.png");
    *nave2 = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_1_2x.png");
    *nave3 = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_1_2x.png");
    *rosa = esat::SpriteFromFile("rosa.png");
}

void DrawShip(esat::SpriteHandle nave1, esat::SpriteHandle nave2, esat::SpriteHandle nave3, 
              esat::SpriteHandle rosa, int head_x, int head_y, int body_x, int body_y, int tail_x, int tail_y){
  esat::DrawSprite(rosa, head_x, head_y);
  esat::DrawSprite(nave1, head_x, head_y);

  esat::DrawSprite(rosa, body_x, body_y);
  esat::DrawSprite(nave2, body_x, body_y);

  esat::DrawSprite(rosa, tail_x, tail_y);
  esat::DrawSprite(nave3, tail_x, tail_y);
}

void MoverNave(int* head_y, int* body_y, int* tail_y, int speed, double delta_time, bool rocket_started){
  if(rocket_started && *head_y >= 60){
    *head_y -= speed * delta_time;
    *body_y -= speed * delta_time;
    *tail_y -= speed * delta_time;
  }
}
