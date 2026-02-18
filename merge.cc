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

//JUGADOR.CC BELTRAN
#include "jugador.cc"

//MAIN.CC JONATHAN
#include "hud.cc"
#include "interface.cc"

// Windows dimensions with default values
const int KWindow_Width = 256 * 2;
const int KWindow_Height = 192 * 2;

// FPS
double delta_time;
unsigned char fps = 25;
double current_time;
double last_time;

int esat::main(int argc, char **argv) {

    esat::WindowInit(KWindow_Width, KWindow_Height);
	esat::WindowSetMouseVisibility(true);

	srand((unsigned)time(nullptr));
	last_time = esat::Time();
    // puntero a sprites
    Sprites *spritesColores = AsignarMemoriaSprites(4);
    Sprites *spritesPersonaje = AsignarMemoriaSprites(16);
    Bala *punteroBalas = AsignarMemoriaBalas(20);
    InstanciarSpritesColores(spritesColores);
    InstanciarSpritesPlayer(spritesPersonaje);
    InstanciarBalas(punteroBalas);

    Jugador player;
    InstanciarPlayer(&player);

    object cubo_prueba;
    InstanciarCubo(&cubo_prueba, *spritesPersonaje);

    // load font
    LoadFonts();

    InitPlatformSprites();

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

        // Change screen
        ScreenSelector(GAME_SCREEN);

        TestMousePosition();

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
