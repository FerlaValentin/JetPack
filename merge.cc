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

void InstantiateSprites(Sprites *spritesColores, Sprites *spritesPersonaje, esat::SpriteHandle* nave1, esat::SpriteHandle* nave2, esat::SpriteHandle* nave3, esat::SpriteHandle* rosa){
    CargarSprites(nave1, nave2, nave3, rosa);
    InstanciarSpritesColores(spritesColores);
    InstanciarSpritesPlayer(spritesPersonaje);
}

void InitiateAll(Sprites **spritesColores, Sprites **spritesPersonaje, Bala **punteroBalas, Jugador *player, esat::SpriteHandle* nave1, esat::SpriteHandle* nave2, esat::SpriteHandle* nave3, esat::SpriteHandle* rosa){
    esat::WindowInit(GLO::kScreenWidth, GLO::kScreenHeight);
	esat::WindowSetMouseVisibility(true);
	srand((unsigned)time(nullptr));
	last_time = esat::Time();
    *spritesColores = (Sprites*)GLO::AllocateMemory(4, sizeof(Sprites));
    *spritesPersonaje = (Sprites*)GLO::AllocateMemory(16, sizeof(Sprites));
    *punteroBalas = (Bala*)GLO::AllocateMemory(20, sizeof(Bala));
    InstantiateSprites(*spritesColores, *spritesPersonaje, nave1, nave2, nave3, rosa);
    InstanciarBalas(*punteroBalas);
    InstanciarPlayer(player);
}

void GetInput(bool* moverLeft, bool* moverRight, bool* ascender, Bala* punteroBalas, Jugador player){
    *moverLeft = (esat::IsKeyPressed('A') || esat::IsKeyPressed('a'));
    *moverRight = (esat::IsKeyPressed('D') || esat::IsKeyPressed('d'));
    *ascender = (esat::IsKeyPressed('W') || esat::IsKeyPressed('w'));
    CrearDisparos(punteroBalas, player);
}

void Update(Jugador* player, bool ascender, Bala* punteroBalas, bool moverLeft, bool moverRight, int* frame/*, COL::object* */){
    Ascender_Gravedad(player, ascender, delta_time);
    ActualizarDisparos(punteroBalas, *player, delta_time);
    LoopMoverJugador(moverLeft, moverRight, player, delta_time);
    //!Cambiar también el tope de la altura para que no toque el HUD
    ControlarLimitesPantalla(player, punteroBalas);
    *frame = ActualizarAnimacionJugador(*player, delta_time);
}

void DrawAll(Sprites* spritesColores, Sprites* spritesPersonaje, Bala* punteroBalas, Jugador player, int frame){
    DibujarColoresJugador(spritesColores, player, frame);
    DibujarJugador(spritesPersonaje, player, frame);
    DibujarDisparos(punteroBalas);
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
}

int esat::main(int argc, char **argv) {
    int frame;
    bool moverLeft, moverRight, ascender;
    Sprites *spritesColores = nullptr, *spritesPersonaje = nullptr;
    esat::SpriteHandle nave1, nave2, nave3, rosa;
    Bala *punteroBalas = nullptr;
    Jugador player;
    //Datos Nave
    int g_pink_x, g_pink_y;
    int g_head_x = 600, g_head_y = 704;
    int g_body_x = 600, g_body_y = 736;
    int g_tail_x = 600, g_tail_y = 768;
    int g_speed = 2;

    InitiateAll(&spritesColores, &spritesPersonaje, &punteroBalas, &player, &nave1, &nave2, &nave3, &rosa);
    
    // Main game loop
    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
        InitiateFrame();

        GetInput(&moverLeft, &moverRight, &ascender, punteroBalas, player);
        Update(&player, ascender, punteroBalas, moverLeft, moverRight, &frame);
        DrawAll(spritesColores, spritesPersonaje, punteroBalas, player, frame);

        FinishFrame();
    }

    // Destroy window
    esat::WindowDestroy();
    FreeAll(&spritesColores, &spritesPersonaje, &nave1, &nave2, &nave3, &rosa, &punteroBalas);
    return 0;
}
