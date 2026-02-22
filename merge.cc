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
void InitiateFrame()
{
    // Calculate time elapsed since the last frame
    current_time = esat::Time();
    delta_time = (current_time - last_time) / 1000.0;
    // Limit delta_time to avoid large jumps
    if (delta_time > 0.1)
        delta_time = 0.1;

    last_time = current_time;

    esat::DrawBegin();
    esat::DrawClear(0, 0, 0);
}

void InitiateAll(Sprites **spritesColores, Sprites **spritesPersonaje, Bala **punteroBalas, Sprites **spritesItems, Jugador *player,
                 COL::object *gasofa, COL::object *prueba_nave, ItemDrop *itemdrop, esat::SpriteHandle **platform_sprite, TPlatform **g_platforms,
                esat::SpriteHandle **loading_sprite, TGame *game, esat::SpriteHandle* sprite_lives, Sprites** spritesNave, Nave* nave)
{
    esat::WindowInit(kScreenWidth, kScreenHeight);
    esat::WindowSetMouseVisibility(true);
    srand((unsigned)time(nullptr));
    last_time = esat::Time();

    // MALLOCS
    *spritesColores = (Sprites *)malloc(4 * sizeof(Sprites));
    *spritesPersonaje = (Sprites *)malloc(16 * sizeof(Sprites));
    *punteroBalas = (Bala *)malloc(20 * sizeof(Bala));
    *spritesItems = (Sprites *)malloc(6 * sizeof(Sprites));
    *spritesNave = InstanciarSpritesNave(16);

    // SPRITES
    InstanciarSpritesColores(*spritesColores);
    InstanciarSpritesPlayer(*spritesPersonaje);
    InstanciarSpritesItems(*spritesItems);
    InitSpriteNave(*spritesNave);

    // Fuente
    LoadFonts();

    //! RECURRE A VARIABLES GLOBALES DE INTERFACE.CC
    InitPlatformSprites(platform_sprite, g_platforms);
    InitLoadingSprites(loading_sprite);

    // INSTANCIAR
    InstanciarBalas(*punteroBalas);
    InstanciarPlayer(player);
    //! HAY QUE SUSTITUIR PRUEBA_NAVE POR LA NAVE? CREO QUE LA FUNCION INSTANCIA LA NAVE TAMBIEN. IGUAL HAY QUE CAMBIARLO
    InstaciarGasofa_Nave(gasofa, prueba_nave, (*spritesItems)[5]);
    AudioInit();
    InstanciarItems(itemdrop, *spritesItems);
    InstanciarNave(nave);
    InitGameVariables(game);
    InitLivesSprite(sprite_lives);
}

void GetInput(bool *moverLeft, bool *moverRight, bool *ascender, Bala *punteroBalas, Jugador player,
             TGame* game, int* menu_selection_player, int* menu_selection_control)
{
    if(game->current_screen == TScreen::GAME_SCREEN){
        if (!player.muerto)
        {
            *moverLeft = (esat::IsKeyPressed('A') || esat::IsKeyPressed('a'));
            *moverRight = (esat::IsKeyPressed('D') || esat::IsKeyPressed('d'));
            *ascender = (esat::IsKeyPressed('W') || esat::IsKeyPressed('w'));
            CrearDisparos(punteroBalas, player);
        }
    }
    if(game->current_screen == TScreen::MAIN_MENU){
      if (esat::IsKeyPressed('1')) *menu_selection_player = 0;
      if (esat::IsKeyPressed('2')) *menu_selection_player = 1;
      if (esat::IsKeyPressed('3')) *menu_selection_control = 0;
      if (esat::IsKeyPressed('4')) *menu_selection_control = 1;
      if (esat::IsKeyPressed('5')) (*game).current_screen = TScreen::GAME_SCREEN;
    }
}

//Solo para testear luego se borra
/*void TestValues(Jugador *player){
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Keypad_0)) (*player).puntos += 1;
  if(esat::IsSpecialKeyDown(esat::kSpecialKey_Keypad_1)) (*player).vidas += 1;
  if(esat::IsSpecialKeyDown(esat::kSpecialKey_Keypad_2)) (*player).player_id == 1 ? (*player).player_id = 2 : (*player).player_id = 1;
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Keypad_3)) (*player).puntos -= 1;
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Keypad_4)) (*player).vidas -= 1;
  if(esat::IsSpecialKeyPressed(esat::kSpecialKey_Keypad_5)) (*player).player_id -= 1;
}*/
  if(esat::IsSpecialKeyDown(esat::kSpecialKey_Keypad_4)) (*player).vidas -= 1;

  if(esat::IsSpecialKeyDown(esat::kSpecialKey_Keypad_5)) SavePlayerData(player);
  if(esat::IsSpecialKeyDown(esat::kSpecialKey_Keypad_6)) LoadPlayerData(player);
}

void Update(Jugador *player, bool ascender, Bala *punteroBalas, bool moverLeft, bool moverRight, int *frame,
            COL::object &gasofa, COL::object &prueba_nave, ItemDrop *itemdrop, int &contador_gasofa, int numero_max_gasofa, 
            Sprites *spritesItems, TPlatform* g_platforms, TGame* game, float* timer, float* menu_blink_timer, bool* menu_highlight_white,
            Nave* nave)
{
    if(game->current_screen != TScreen::GAME_SCREEN)
        ScreenSelector(game, timer, menu_blink_timer, menu_highlight_white);
    else{
        Ascender_Gravedad(player, ascender);
        ActualizarDisparos(punteroBalas, *player);
        LoopMoverJugador(moverLeft, moverRight, player);
        ControlarLimitesPantalla(player, punteroBalas);
    
        // Pasar vidas y puntos a la interfaz
        UpdateInterface(&player->puntos, &player->vidas, &player->player_id, game);
        //TestValues(player);

        if (esat::IsKeyDown('Y') || esat::IsKeyDown('y'))
            player->muerto = true;
        // ! Colisiones
        if (player->colisiona && !player->muerto)
            ColisionJugador(player); // Actualizar colider a player
        if (player->muerto || !player->colisiona)
            ResetPlayer_OnDead(player);
        ActualizarColisionesItems(player, gasofa, prueba_nave, *itemdrop);
        //! Meter aqui la nave
        LoopGasofa(*player, gasofa, prueba_nave, contador_gasofa, numero_max_gasofa);
        LoopPickItems(*player, itemdrop, spritesItems);
        ColisionPlayerPlatforma(*player, g_platforms);

        //! Cambiar también el tope de la altura para que no toque el HUD
        ControlarLimitesPantalla(player, punteroBalas);
        *frame = ActualizarAnimacionJugador(*player);
        TriggerNave(nave);
        MoverNave(nave);
    }
}

void DrawAll(Sprites *spritesColores, Sprites *spritesPersonaje, Bala *punteroBalas, Jugador player, int frame, 
            COL::object gasofa, Sprites *spritesItems, ItemDrop itemdrop, TPlatform* g_platforms, esat::SpriteHandle* platform_sprite,
            TGame game, esat::SpriteHandle* loading_sprite, int menu_selection_player, int menu_selection_control, int menu_highlight_white, esat::SpriteHandle sprite_vidas)
{
    if(game.current_screen == TScreen::IMAGE)
      InitialImage(loading_sprite);
    if(game.current_screen == TScreen::MAIN_MENU)
      MainMenu(menu_selection_player, menu_selection_control, menu_highlight_white, game, sprite_vidas);
    if(game.current_screen == TScreen::GAME_SCREEN){
        GameScreen(g_platforms, platform_sprite, game, sprite_vidas, player.vidas);
        DibujarDisparos(punteroBalas);
        if (!player.muerto && player.colisiona)
        {
            DibujarColoresJugador(spritesColores, player, frame);
            DibujarJugador(spritesPersonaje, player, frame);
        }
        if (!player.colisiona)
        {
            DibujarColoresJugador(spritesColores, player, 3);
            DibujarJugador(spritesPersonaje, player, frame);
        }

        DibujarGasofa(gasofa, spritesItems);
        DibujarItems(itemdrop, spritesItems);
    }
}

void FinishFrame()
{
    // Finish drawing
    esat::DrawEnd();
    // Control frame speed (fps)
    do
    {
        current_time = esat::Time();
    } while ((current_time - last_time) <= 1000.0 / fps);
    esat::WindowFrame();
}

void FreeAll(Sprites **spritesColores, Sprites **spritesPersonaje, Sprites **spritesItem, Bala **punteroBalas,
             COL::object *gasofa, ItemDrop *itemdrop)
{
    for (int i = 0; i < 4; ++i)
        esat::SpriteRelease((*spritesColores)[i].sprite);
    free(*spritesColores);
    *spritesColores = nullptr;

    for (int i = 0; i < 16; ++i)
        esat::SpriteRelease((*spritesPersonaje)[i].sprite);
    free(*spritesPersonaje);
    *spritesPersonaje = nullptr;

    for (int i = 0; i < 6; ++i)
        esat::SpriteRelease((*spritesItem)[i].sprite);
    free(*spritesItem);
    *spritesItem = nullptr;

    free(*punteroBalas);
    *punteroBalas = nullptr;
    
    esat::SpriteRelease(gasofa->sprite);
    esat::SpriteRelease(itemdrop->item_config.sprite);
    FreeAudio();
}

int esat::main(int argc, char **argv)
{
    const int terrain_height = 16, numero_max_gasofa = 3;
    int frame, contador_gasofa = 0;
    bool moverLeft, moverRight, ascender;
    Sprites *spritesColores = nullptr, *spritesPersonaje = nullptr, *spritesItems = nullptr, *SpritesNaves;
    Bala *punteroBalas = nullptr;
    Jugador player;
    COL::object gasofa;
    COL::object prueba_nave;
    ItemDrop itemdrop;
    Nave nave;

    // plataformas
    esat::SpriteHandle *platform_sprite = nullptr;
    esat::SpriteHandle *loading_sprite = nullptr;
    esat::SpriteHandle sprite_lives;
    TPlatform *g_platforms = nullptr;
    TGame game;
    // TODO: move top
    float timer = 0;
    int menu_selection_player = 0;  /* 0 = 1 player, 1 = 2 player */
    int menu_selection_control = 0; /* 0 = keyboard, 1 = kempston */
    float menu_blink_timer = 0.0f;
    bool menu_highlight_white = true;

    InitiateAll(&spritesColores, &spritesPersonaje, &punteroBalas, &spritesItems, &player, &gasofa, &prueba_nave, &itemdrop, &platform_sprite, &g_platforms, &loading_sprite, &game,
                &sprite_lives, &SpritesNaves, &nave);

    // Main game loop
    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
    {
        InitiateFrame();

        GetInput(&moverLeft, &moverRight, &ascender, punteroBalas, player, &game, &menu_selection_player, &menu_selection_control);
        Update(&player, ascender, punteroBalas, moverLeft, moverRight, &frame, gasofa, prueba_nave, &itemdrop, contador_gasofa, 
                numero_max_gasofa, spritesItems, g_platforms, &game, &timer, &menu_blink_timer, &menu_highlight_white, &nave);
        DrawAll(spritesColores, spritesPersonaje, punteroBalas, player, frame, gasofa, spritesItems, itemdrop, g_platforms, platform_sprite, 
                game, loading_sprite, menu_selection_player, menu_selection_control, menu_highlight_white, sprite_lives);

        FinishFrame();
    }

    // Destroy window
    esat::WindowDestroy();
    FreeAll(&spritesColores, &spritesPersonaje, &spritesItems, &punteroBalas, &gasofa, &itemdrop);
    return 0;
}
