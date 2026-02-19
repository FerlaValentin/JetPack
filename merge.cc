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
#include "globalfunctions.h"

//JUGADOR.CC BELTRAN
#include "jugador.cc"

// Windows dimensions with default values
const int KWindow_Width = 256 * 2;
const int KWindow_Height = 192 * 2;

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

//!Si object prueba se queda, descomentar parametro
void InitiateAll(Sprites **spritesColores, Sprites **spritesPersonaje, Bala **punteroBalas, Jugador *player/*, COL::object *cubo_prueba*/){
	srand((unsigned)time(nullptr));
	last_time = esat::Time();
    *spritesColores = (Sprites*)GLO::AllocateMemory(4, sizeof(Sprites));
    *spritesPersonaje = (Sprites*)GLO::AllocateMemory(16, sizeof(Sprites));
    *punteroBalas = (Bala*)GLO::AllocateMemory(20, sizeof(Bala));
    InstanciarSpritesColores(*spritesColores);
    InstanciarSpritesPlayer(*spritesPersonaje);
    InstanciarBalas(*punteroBalas);
    InstanciarPlayer(player);
    //!No se si es algo que se va a quedar o es solo testeo
    //InstanciarCubo(cubo_prueba, *spritesPersonaje);
    esat::WindowInit(GLO::kScreenWidth, GLO::kScreenHeight);
	esat::WindowSetMouseVisibility(true);
}

void GetInput(bool* moverLeft, bool* moverRight, bool* ascender, Bala* punteroBalas, Jugador player){
    *moverLeft = (esat::IsKeyPressed('A') || esat::IsKeyPressed('a'));
    *moverRight = (esat::IsKeyPressed('D') || esat::IsKeyPressed('d'));
    *ascender = (esat::IsKeyPressed('W') || esat::IsKeyPressed('w'));
    CrearDisparos(punteroBalas, player);
}

void Update(Jugador* player, bool ascender, Bala* punteroBalas, bool moverLeft, bool moverRight, int* frame/*, COL::object* */){
    Ascender_Gravedad(player, ascender);
    //!Dónde se define object.width/height de las balas para CreateColision?
    //!Hablar con Beltran para quitar ese break. El for debe para al encontrar la primera bala inactiva 
    ActualizarDisparos(punteroBalas, *player);
    LoopMoverJugador(moverLeft, moverRight, player);
    //!He cambiado algunos valores que incluian la altura del terreno
    //!Cambiar también el tope de la altura para que no toque el HUD
    ControlarLimitesPantalla(player, punteroBalas);
    //!No se si es algo que se va a quedar o es solo testeo
    /*ActualizarColisiones(&player, cubo_prueba);
    ColisionPlayer(player, cubo_prueba);
    ColisionDisparos(punteroBalas, cubo_prueba);*/
    *frame = ActualizarAnimacionJugador(*player);
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

int esat::main(int argc, char **argv) {
    int frame;
    bool moverLeft, moverRight, ascender;
    Sprites *spritesColores = nullptr, *spritesPersonaje = nullptr;
    Bala *punteroBalas = nullptr;
    Jugador player;
    //!No se si es algo que se va a quedar o es solo testeo. Actualizar parametros Init y Update
    //COL::object cubo_prueba;

    InitiateAll(&spritesColores, &spritesPersonaje, &punteroBalas, &player/*, &cubo_prueba*/);
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
    return 0;
}
