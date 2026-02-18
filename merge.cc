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

//FUNCIONES PARA ABSTRAER
#include "valentin.h"

//JUGADOR.CC BELTRAN
#include "jugador.cc"

// Windows dimensions with default values
const int KWindow_Width = 256 * 2;
const int KWindow_Height = 192 * 2;

// FPS
double delta_time;
unsigned char fps = 25;
double current_time;
double last_time;

void Init(Sprites **spritesColores, Sprites **spritesPersonaje, Bala **punteroBalas, Jugador *player){
	srand((unsigned)time(nullptr));
	last_time = esat::Time();
    *spritesColores = (Sprites*)VFL::AllocateMemory(4, sizeof(Sprites));
    *spritesPersonaje = (Sprites*)VFL::AllocateMemory(16, sizeof(Sprites));
    *punteroBalas = (Bala*)VFL::AllocateMemory(20, sizeof(Bala));
    InstanciarSpritesColores(*spritesColores);
    InstanciarSpritesPlayer(*spritesPersonaje);
    InstanciarBalas(*punteroBalas);
    InstanciarPlayer(player);
    esat::WindowInit(KWindow_Width, KWindow_Height);
	esat::WindowSetMouseVisibility(true);
}

void GetInput(bool* moverLeft, bool* moverRight, bool* ascender){
    *moverLeft = (esat::IsKeyPressed('A') || esat::IsKeyPressed('a'));
    *moverRight = (esat::IsKeyPressed('D') || esat::IsKeyPressed('d'));
    *ascender = (esat::IsKeyPressed('W') || esat::IsKeyPressed('w'));
}

void Update(Jugador* player, bool ascender){
    Ascender_Gravedad(player, ascender);
}

int esat::main(int argc, char **argv) {
    Sprites *spritesColores = nullptr, *spritesPersonaje = nullptr;
    Bala *punteroBalas = nullptr;
    Jugador player;
    bool moverLeft, moverRight, ascender;

    Init(&spritesColores, &spritesPersonaje, &punteroBalas, &player);
    // Main game loop
    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
        
        // Calculate time elapsed since the last frame
        current_time = esat::Time();
        delta_time = (current_time - last_time) / 1000.0;
        // Limit delta_time to avoid large jumps
        if (delta_time > 0.1) delta_time = 0.1;
        
        last_time = current_time;

        esat::DrawBegin();
        esat::DrawClear(0,0,0);

        GetInput(&moverLeft, &moverRight, &ascender);
        Update(&player, ascender);

        // Finish drawing
        esat::DrawEnd();
        // Control frame speed (fps)
        do{
        current_time = esat::Time();
        }while((current_time - last_time) <= 1000.0/fps);
        esat::WindowFrame();
    }

    // Destroy window
    esat::WindowDestroy();
    return 0;
}
