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

#include "globalfunctions.h"
#include "jugador.cc"
#include "nave.cc"

// FPS
double delta_time;
unsigned char fps = 25;
double current_time;
double last_time = 0;

void InitiateFrame(){
    // Calculate time elapsed since the last frame
    current_time = esat::Time();
    delta_time = (current_time - last_time) / 1000.0;
    // Limit delta_time to avoid large jumps
    if (delta_time > 0.1) delta_time = 0.1;
        
    last_time = current_time;

    esat::DrawBegin();
    esat::DrawClear(0,0,0);
}

void InitiateAll(Sprites **spritesColores, Sprites **spritesPersonaje, Bala **punteroBalas, Jugador *player, esat::SpriteHandle* nave1, esat::SpriteHandle* nave2, esat::SpriteHandle* nave3, esat::SpriteHandle* rosa){
    esat::WindowInit(GLO::kScreenWidth, GLO::kScreenHeight);
	esat::WindowSetMouseVisibility(true);
	srand((unsigned)time(nullptr));
	last_time = esat::Time();

    //MALLOCS
    *spritesColores = (Sprites*)GLO::AllocateMemory(4, sizeof(Sprites));
    *spritesPersonaje = (Sprites*)GLO::AllocateMemory(16, sizeof(Sprites));
    *punteroBalas = (Bala*)GLO::AllocateMemory(20, sizeof(Bala));

    //SPRITES
    LoadShipSprites(nave1, nave2, nave3, rosa);
    InstanciarSpritesColores(*spritesColores);
    InstanciarSpritesPlayer(*spritesPersonaje);

    //INSTANCIAR
    InstanciarBalas(*punteroBalas);
    InstanciarPlayer(player);
}

void GetInput(bool* moverLeft, bool* moverRight, bool* ascender, Bala* punteroBalas, Jugador player){
    *moverLeft = (esat::IsKeyPressed('A') || esat::IsKeyPressed('a'));
    *moverRight = (esat::IsKeyPressed('D') || esat::IsKeyPressed('d'));
    *ascender = (esat::IsKeyPressed('W') || esat::IsKeyPressed('w'));
    CrearDisparos(punteroBalas, player);
}

void Update(Jugador* player, bool ascender, Bala* punteroBalas, bool moverLeft, bool moverRight, int* frame,
            int* head_y, int* body_y, int* tail_y, int speed){
    Ascender_Gravedad(player, ascender, delta_time);
    ActualizarDisparos(punteroBalas, *player, delta_time);
    LoopMoverJugador(moverLeft, moverRight, player, delta_time);
    //!Cambiar también el tope de la altura para que no toque el HUD
    ControlarLimitesPantalla(player, punteroBalas);
    *frame = ActualizarAnimacionJugador(*player, delta_time);
    MoverNave(head_y, body_y, tail_y, speed);
}

void DrawAll(Sprites* spritesColores, Sprites* spritesPersonaje, Bala* punteroBalas, Jugador player, int frame, 
        esat::SpriteHandle nave1, esat::SpriteHandle nave2, esat::SpriteHandle nave3, esat::SpriteHandle rosa,
        int head_x, int head_y, int body_x, int body_y, int tail_x, int tail_y){
    DibujarColoresJugador(spritesColores, player, frame);
    DibujarJugador(spritesPersonaje, player, frame);
    DibujarDisparos(punteroBalas);
    DrawShip(nave1, nave2, nave3, rosa, head_x, head_y, body_x, body_y, tail_x, tail_y);
}

void FinishFrame(){
    // Finish drawing
    esat::DrawEnd();
    // Control frame speed (fps)
    do{
        current_time = esat::Time();
    }while((current_time - last_time) <= 1000.0/fps);
    esat::WindowFrame();
}

void FreeAll(Sprites **spritesColores, Sprites **spritesPersonaje, esat::SpriteHandle* nave1,
            esat::SpriteHandle* nave2, esat::SpriteHandle* nave3, esat::SpriteHandle* rosa, Bala **punteroBalas){
    for(int i = 0; i < 4; ++i)
        esat::SpriteRelease((**spritesColores).sprite);
    free(*spritesColores);
    *spritesColores = nullptr;

    for(int i = 0; i < 16; ++i)
        esat::SpriteRelease((**spritesPersonaje).sprite);
    free(*spritesPersonaje);
    *spritesPersonaje = nullptr;

    free(*punteroBalas);
    *punteroBalas = nullptr;

    esat::SpriteRelease(*nave1);
    esat::SpriteRelease(*nave2);
    esat::SpriteRelease(*nave3);
    esat::SpriteRelease(*rosa);
}

int esat::main(int argc, char **argv){
    const int terrain_height = 16;
    int frame;
    bool moverLeft, moverRight, ascender;
    Sprites *spritesColores = nullptr, *spritesPersonaje = nullptr;
    esat::SpriteHandle nave1, nave2, nave3, rosa;
    Bala *punteroBalas = nullptr;
    Jugador player;
    //Datos Nave
    int pink_x, pink_y;
    int tail_x = 600, tail_y = GLO::kScreenWidth - terrain_height;
    int body_x = 600, body_y = 736;
    int head_x = 600, head_y = 704;
    int speed = 2;

    InitiateAll(&spritesColores, &spritesPersonaje, &punteroBalas, &player, &nave1, &nave2, &nave3, &rosa);
    
    // Main game loop
    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
        InitiateFrame();

        GetInput(&moverLeft, &moverRight, &ascender, punteroBalas, player);
        Update(&player, ascender, punteroBalas, moverLeft, moverRight, &frame, &head_y, &body_y, &tail_y, speed);
        DrawAll(spritesColores, spritesPersonaje, punteroBalas, player, frame, nave1, nave2, nave3, rosa, head_x, head_y,
                body_x, body_y, tail_x, tail_y);
        FinishFrame();
    }

    // Destroy window
    esat::WindowDestroy();
    FreeAll(&spritesColores, &spritesPersonaje, &nave1, &nave2, &nave3, &rosa, &punteroBalas);
    return 0;
}
