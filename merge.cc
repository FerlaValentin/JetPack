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

const int kScreenWidth = 512;
const int kScreenHeight = 384;
double delta_time;

#include "jugador.cc"
#include "nave.cc"
#include "interface.cc"

// FPS
unsigned char fps = 25;
double current_time;
double last_time = 0;

/*struct PlayerVariables
{
    Jugador player;
    bool ascender;
    Bala punteroBalas;
    bool moverLeft;
    bool moverRight;
    int frame;
};

 (Jugador* player, bool ascender, Bala* punteroBalas, bool moverLeft, bool moverRight, int* frame,
            int* head_y, int* body_y, int* tail_y, int speed, bool rocket_started, COL::object gasofa, COL::object prueba_nave, ItemDrop itemdrop, int contador_gasofa, int numero_max_gasofa, Sprites *spritesItems)*/
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

void InitiateAll(Sprites **spritesColores, Sprites **spritesPersonaje, Bala **punteroBalas, Sprites **spritesItems, Jugador *player,
                 esat::SpriteHandle* nave1, esat::SpriteHandle* nave2, esat::SpriteHandle* nave3, esat::SpriteHandle* rosa, COL::object* gasofa,
                COL::object* prueba_nave, ItemDrop* itemdrop, esat::SpriteHandle** platform_sprite, TPlatform** g_platforms){
    esat::WindowInit(kScreenWidth, kScreenHeight);
	esat::WindowSetMouseVisibility(true);
	srand((unsigned)time(nullptr));
	last_time = esat::Time();

    //MALLOCS
    *spritesColores = (Sprites*)malloc(4 * sizeof(Sprites));
    *spritesPersonaje = (Sprites*)malloc(16 * sizeof(Sprites));
    *punteroBalas = (Bala*)malloc(20 * sizeof(Bala));
    *spritesItems = (Sprites*)malloc(6 * sizeof(Sprites));

    //SPRITES
    LoadShipSprites(nave1, nave2, nave3, rosa);
    InstanciarSpritesColores(*spritesColores);
    InstanciarSpritesPlayer(*spritesPersonaje);
    InstanciarSpritesItems(*spritesItems);
    //!RECURRE A VARIABLES GLOBALES DE INTERFACE.CC
    InitPlatformSprites(*platform_sprite, *g_platforms);

    //INSTANCIAR
    InstanciarBalas(*punteroBalas);
    InstanciarPlayer(player);
    //!HAY QUE SUSTITUIR PRUEBA_NAVE POR LA NAVE? CREO QUE LA FUNCION INSTANCIA LA NAVE TAMBIEN. IGUAL HAY QUE CAMBIARLO
    InstaciarGasofa_Nave(gasofa, prueba_nave, (*spritesItems)[5]);
    //AudioInit();
    InstanciarItems(itemdrop, *spritesItems);
}

void GetInput(bool* moverLeft, bool* moverRight, bool* ascender, Bala* punteroBalas, Jugador player, bool* rocket_started){
    *moverLeft = (esat::IsKeyPressed('A') || esat::IsKeyPressed('a'));
    *moverRight = (esat::IsKeyPressed('D') || esat::IsKeyPressed('d'));
    *ascender = (esat::IsKeyPressed('W') || esat::IsKeyPressed('w'));
    if((esat::IsKeyDown('P')) || (esat::IsKeyDown('p'))) *rocket_started = true;
    CrearDisparos(punteroBalas, player);
}

void Update(Jugador* player, bool ascender, Bala* punteroBalas, bool moverLeft, bool moverRight, int* frame,
            int* head_y, int* body_y, int* tail_y, int speed, bool rocket_started, COL::object &gasofa, COL::object &prueba_nave, ItemDrop* itemdrop, int &contador_gasofa, int numero_max_gasofa, Sprites *spritesItems) {
                
    Ascender_Gravedad(player, ascender);
    ActualizarDisparos(punteroBalas, *player);
    LoopMoverJugador(moverLeft, moverRight, player);
    ControlarLimitesPantalla(player, punteroBalas);

    // ! Colisiones
    if (player->colisiona)
        ColisionJugador(player); // Actualizar colider a player
    if (player->muerto)
        ResetPlayer_OnDead(player);
    
    ActualizarColisionesItems(player, gasofa, prueba_nave, *itemdrop);
    //!Meter aqui la nave
    LoopGasofa(*player, gasofa, prueba_nave, contador_gasofa, numero_max_gasofa);
    LoopPickItems(*player, itemdrop, spritesItems);
    ColisionPlayerPlatforma(*player);

    //!Cambiar también el tope de la altura para que no toque el HUD
    ControlarLimitesPantalla(player, punteroBalas);
    *frame = ActualizarAnimacionJugador(*player);
    MoverNave(head_y, body_y, tail_y, speed, rocket_started);
}

void DrawAll(Sprites* spritesColores, Sprites* spritesPersonaje, Bala* punteroBalas, Jugador player, int frame, 
        esat::SpriteHandle nave1, esat::SpriteHandle nave2, esat::SpriteHandle nave3, esat::SpriteHandle rosa,
        int head_x, int head_y, int body_x, int body_y, int tail_x, int tail_y, COL::object gasofa, Sprites* spritesItems, ItemDrop itemdrop){
    GameScreen();
    DibujarDisparos(punteroBalas);
    if (!player.muerto) {
        DibujarColoresJugador(spritesColores, player, frame);
        DibujarJugador(spritesPersonaje, player, frame);
    } else if (!player.colisiona){
        DibujarColoresJugador(spritesColores, player, 0);
        DibujarJugador(spritesPersonaje, player, frame);
    }

    DibujarGasofa(gasofa, spritesItems);
    DibujarItems(itemdrop, spritesItems);
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

void FreeAll(Sprites **spritesColores, Sprites **spritesPersonaje, Sprites **spritesItem, esat::SpriteHandle* nave1,
            esat::SpriteHandle* nave2, esat::SpriteHandle* nave3, esat::SpriteHandle* rosa, Bala **punteroBalas,
            COL::object* gasofa, ItemDrop* itemdrop){
    for(int i = 0; i < 4; ++i)
        esat::SpriteRelease((*spritesColores)[i].sprite);
    free(*spritesColores);
    *spritesColores = nullptr;

    for(int i = 0; i < 16; ++i)
        esat::SpriteRelease((*spritesPersonaje)[i].sprite);
    free(*spritesPersonaje);
    *spritesPersonaje = nullptr;

    for(int i = 0; i < 6; ++i)
        esat::SpriteRelease((*spritesItem)[i].sprite);
    free(*spritesItem);
    *spritesItem = nullptr;
    
    free(*punteroBalas);
    *punteroBalas = nullptr;

    esat::SpriteRelease(*nave1);
    esat::SpriteRelease(*nave2);
    esat::SpriteRelease(*nave3);
    esat::SpriteRelease(*rosa);
    esat::SpriteRelease(gasofa->sprite);
    esat::SpriteRelease(itemdrop->item_config.sprite);
}

int esat::main(int argc, char **argv){
    const int terrain_height = 16, numero_max_gasofa = 3;
    int frame, contador_gasofa = 0;
    bool moverLeft, moverRight, ascender;
    Sprites *spritesColores = nullptr, *spritesPersonaje = nullptr, *spritesItems = nullptr;
    esat::SpriteHandle nave1, nave2, nave3, rosa;
    Bala *punteroBalas = nullptr;
    Jugador player;
    COL::object gasofa;
    COL::object prueba_nave;
    ItemDrop itemdrop;

    // plataformas
    const unsigned char kplatform_numbers = 3;
    esat::SpriteHandle* platform_sprite = nullptr;
    esat::SpriteHandle* loading_sprite = nullptr;
    TPlatform* g_platforms = nullptr;
    TGame game;
        // TODO: move top
    float timer = 0;
    int menu_selection_player = 0;   /* 0 = 1 player, 1 = 2 player */
    int menu_selection_control = 0;   /* 0 = keyboard, 1 = kempston */
    float menu_blink_timer = 0.0f;
    bool menu_highlight_white = true;

    InitiateAll(&spritesColores, &spritesPersonaje, &punteroBalas, &spritesItems, &player, &nave1, &nave2, &nave3, &rosa, &gasofa, &prueba_nave, &itemdrop, &platform_sprite, &g_platforms);

    //Datos Nave
    int pink_x, pink_y;
    int tail_x = 420, tail_y = kScreenHeight - esat::SpriteHeight(nave3) - terrain_height;
    int body_x = 420, body_y = tail_y - esat::SpriteHeight(nave3);
    int head_x = 420, head_y = body_y - esat::SpriteHeight(nave2);
    int speed = 1;
    bool rocket_started = false;

    // Main game loop
    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
        InitiateFrame();

        GetInput(&moverLeft, &moverRight, &ascender, punteroBalas, player, &rocket_started);
        Update(&player, ascender, punteroBalas, moverLeft, moverRight, &frame, &head_y, &body_y, &tail_y, speed, rocket_started, gasofa, prueba_nave, &itemdrop, contador_gasofa, numero_max_gasofa, spritesItems);
        DrawAll(spritesColores, spritesPersonaje, punteroBalas, player, frame, nave1, nave2, nave3, rosa, head_x, head_y,
                body_x, body_y, tail_x, tail_y, gasofa, spritesItems, itemdrop);
        FinishFrame();
    }

    // Destroy window
    esat::WindowDestroy();
    FreeAll(&spritesColores, &spritesPersonaje, &spritesItems, &nave1, &nave2, &nave3, &rosa, &punteroBalas, &gasofa, &itemdrop);
    return 0;
}
