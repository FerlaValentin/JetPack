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

#include "nave.cc"
#include "jugador.cc"
#include "colisiones.h"
#include "enemigos.h"

// FPS
unsigned char fps = 25;
double current_time;
double last_time = 0;

struct ParteNave
{
    esat::Vec2 pos;
    COL::object parteNaveConfig;
    bool recogido;
};

// definicion funciones agregar luego

ParteNave *parteNave = nullptr;

bool primeraColocada = false;

void ReservaMemoriaNave(ParteNave **parteNave)
{
    *parteNave = (ParteNave *)malloc(sizeof(ParteNave) * 3);
}

void InstanciarPartesDeLaNave(ParteNave *parteNave)
{
    int measure = 32;

    ParteNave cabeza;
    cabeza.parteNaveConfig.position.x = 105;
    cabeza.parteNaveConfig.position.y = 150 - 32;
    cabeza.parteNaveConfig.height = measure;
    cabeza.parteNaveConfig.width = measure;
    cabeza.parteNaveConfig.colision = COL::CreateColision(cabeza.parteNaveConfig);
    cabeza.recogido = false;
    parteNave[0] = cabeza;

    ParteNave cuerpo;
    cuerpo.parteNaveConfig.position.x = 256;
    cuerpo.parteNaveConfig.position.y = 180 - 32;
    cuerpo.parteNaveConfig.height = measure;
    cuerpo.parteNaveConfig.width = measure;
    cuerpo.parteNaveConfig.colision = COL::CreateColision(cuerpo.parteNaveConfig);
    cuerpo.recogido = false;
    parteNave[1] = cuerpo;

    ParteNave cola;
    //cola.parteNaveConfig.position.x = 325;
    cola.parteNaveConfig.position.x = kScreenWidth - 170;
    cola.parteNaveConfig.position.y = (kScreenHeight - 16) - 32;
    cola.parteNaveConfig.height = kScreenHeight;
    cola.parteNaveConfig.width = measure;
    cola.recogido = true;
    parteNave[2] = cola;
}

void DibujarPartesNave(ParteNave *parteNave, Sprites *punteroSprites)
{
    int height = 32;

    for (int i = 0; i < 3; i++)
    {
        esat::DrawSprite(punteroSprites[12].sprite, parteNave[i].parteNaveConfig.position.x, parteNave[i].parteNaveConfig.position.y);
    }

    for (int i = 0; i < 3; i++)
    {
        esat::DrawSprite(punteroSprites[i].sprite, parteNave[i].parteNaveConfig.position.x, parteNave[i].parteNaveConfig.position.y);
    }
}

void ActualizarColisionParteNave(ParteNave *parteNave)
{
    for (int i = 0; i < 2; i++)
    {
        parteNave[i].parteNaveConfig.colision = COL::CreateColision(parteNave[i].parteNaveConfig);
    }
}
// Colision de parte con nave.
// Esta funcion necesitarás ajustarla un poco, te recomiendo que sigas por aqui

void ActualizarColocarNave(Nave *nave, ParteNave *parteNave){
    for(int i=0; i<2; i++){
        if(!parteNave[i].recogido){
            parteNave[i].parteNaveConfig.position.x = nave->pos.x;
            parteNave[i].parteNaveConfig.position.y = nave->pos.y + ((i)*32);
            // while(parteNave[i].parteNaveConfig.position.y != nave->pos.y + ((i)*32)){
            //     parteNave[i].parteNaveConfig.position.y  += 1;
            // }
        }

    }
}

void ColisionColocarPartes(Nave *nave, ParteNave *parte_nave, Jugador *player)
{
    for (int i = 0; i < 2; i++)
    {
        if ((COL::CheckColision(nave->nave_config.colision, parte_nave[i].parteNaveConfig.colision)) /*&& parte_nave[i].recogido*/)
        {
            GravedadItem(parteNave[i].parteNaveConfig);
            //ActualizarColocarNave(nave, parteNave);
        }
    }

}

void ActualizarPosParteNave(ParteNave *parteNave, Jugador *player)
{

    for (int i = 0; i < 2; i++)
    {
        if (parteNave[i].recogido)
        {
            parteNave[i].parteNaveConfig.position.x = player->pos.x + 32;
            parteNave[i].parteNaveConfig.position.y = player->pos.y;
        }
    }
}

void ColisionPartesNaveJugador(ParteNave *parteNave, Jugador *player, bool &primera_colocada)
{
    if (COL::CheckColision(player->config_colision.colision, parteNave[1].parteNaveConfig.colision) && !primera_colocada)
    {
        parteNave[1].recogido = true;
        primera_colocada = true;
        printf("Colision objeto 1\n");
    }
    if (primera_colocada)
    {
        if (COL::CheckColision(player->config_colision.colision, parteNave[0].parteNaveConfig.colision))
        {
            parteNave[1].recogido = false;
            parteNave[0].recogido = true;
            printf("Colision objeto 0\n");
        }
    }
    ActualizarPosParteNave(parteNave, player);

    COL::ShowColision(parteNave[0].parteNaveConfig.colision);
    COL::ShowColision(parteNave[1].parteNaveConfig.colision);
}

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
                 COL::object *gasofa, ItemDrop *itemdrop, esat::SpriteHandle **platform_sprite, TPlatform **g_platforms,
                 esat::SpriteHandle **loading_sprite, TGame *game, esat::SpriteHandle *sprite_lives, Sprites **spritesNave, Nave *nave)
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
    InitPlatformSprites(platform_sprite, g_platforms);
    InitLoadingSprites(loading_sprite);

    // INSTANCIAR
    InstanciarBalas(*punteroBalas);
    InstanciarPlayer(player);
    //! HAY QUE SUSTITUIR PRUEBA_NAVE POR LA NAVE? CREO QUE LA FUNCION INSTANCIA LA NAVE TAMBIEN. IGUAL HAY QUE CAMBIARLO
    InstaciarGasofa_Nave(gasofa, (*spritesItems)[5]);
    AudioInit();
    InstanciarItems(itemdrop, *spritesItems);
    InstanciarNave(nave);
    InitGameVariables(game);
    InitLivesSprite(sprite_lives); // Fuente
    LoadFonts();

    //

    ReservaMemoriaNave(&parteNave);
    InstanciarPartesDeLaNave(parteNave);
}

void GetInput(bool *moverLeft, bool *moverRight, bool *ascender, Bala *punteroBalas, Jugador player,
              TGame *game, int *menu_selection_player, int *menu_selection_control)
{
    if (game->current_screen == TScreen::GAME_SCREEN)
    {
        if (!player.muerto)
        {
            *moverLeft = (esat::IsKeyPressed('A') || esat::IsKeyPressed('a'));
            *moverRight = (esat::IsKeyPressed('D') || esat::IsKeyPressed('d'));
            *ascender = (esat::IsKeyPressed('W') || esat::IsKeyPressed('w'));
            CrearDisparos(punteroBalas, player);
        }
    }
    if (game->current_screen == TScreen::MAIN_MENU)
    {
        if (esat::IsKeyPressed('1'))
            *menu_selection_player = 0;
        if (esat::IsKeyPressed('2'))
            *menu_selection_player = 1;
        if (esat::IsKeyPressed('3'))
            *menu_selection_control = 0;
        if (esat::IsKeyPressed('4'))
            *menu_selection_control = 1;
        if (esat::IsKeyPressed('5'))
        {
            if (*menu_selection_player == 1)
            {
                // Save player 2 data
                Jugador player2;
                InstanciarPlayer(&player2);
                player2.player_id = 2;
                player2.isActive = false;
                SavePlayerDataToFile(&player, &player2);
            }
            else
            {
                SavePlayerDataToFile(&player);
            }
            game->current_screen = TScreen::GAME_SCREEN;
        }

        //
    }
}

void SwitchPlayer(Jugador *player)
{
    Jugador tmp;
    tmp = *player;
    printf("[DEBUG] Switching player to %d\n", player->player_id == 1 ? 2 : 1);
    LoadPlayerDataFromFile(player, player->player_id == 1 ? 2 : 1);
    SavePlayerDataToFile(&tmp, player);
}

// Solo para testear luego se borra
void TestValues(Jugador *player)
{
    if (esat::IsSpecialKeyPressed(esat::kSpecialKey_Keypad_0))
        (*player).puntos += 1;
    if (esat::IsSpecialKeyDown(esat::kSpecialKey_Keypad_1))
        (*player).vidas += 1;
    if (esat::IsSpecialKeyDown(esat::kSpecialKey_Keypad_2))
        (*player).player_id == 1 ? (*player).player_id = 2 : (*player).player_id = 1;
    if (esat::IsSpecialKeyPressed(esat::kSpecialKey_Keypad_3))
        (*player).puntos -= 1;
    if (esat::IsSpecialKeyDown(esat::kSpecialKey_Keypad_4))
        (*player).vidas -= 1;

    if (esat::IsSpecialKeyDown(esat::kSpecialKey_Keypad_8))
        SwitchPlayer(player);
}

void Update(Jugador *player, bool ascender, Bala *punteroBalas, bool moverLeft, bool moverRight, int *frame,
            COL::object &gasofa, ItemDrop *itemdrop, Sprites *spritesItems, TPlatform *g_platforms,
            TGame *game, float *timer, float *menu_blink_timer, bool *menu_highlight_white, Nave *nave, bool *primera_colocada)
{
    if (game->current_screen != TScreen::GAME_SCREEN)
        ScreenSelector(game, timer, menu_blink_timer, menu_highlight_white);
    else
    {
        Ascender_Gravedad(player, ascender);
        ActualizarDisparos(punteroBalas, *player);
        LoopMoverJugador(moverLeft, moverRight, player);
        ControlarLimitesPantalla(player, punteroBalas);
        //! Cambiar también el tope de la altura para que no toque el HUD
        *frame = ActualizarAnimacionJugador(*player);
        ColisionPlayerPlatforma(*player, g_platforms);

        // Pasar vidas y puntos a la interfaz
        UpdateInterface(&player->puntos, &player->vidas, &player->player_id, game);
        TestValues(player); // @jhony: remove this

        if (esat::IsKeyDown('Y') || esat::IsKeyDown('y'))
            player->muerto = true;
        // ! Colisiones
        if (player->colisiona && !player->muerto)
            ColisionJugador(player); // Actualizar colider a player
        if (player->muerto || !player->colisiona)
            ResetPlayer_OnDead(player);

        ActualizarColisionesItems(player, gasofa, *itemdrop, nave);
        //! Meter aqui la nave
        LoopGasofa(*player, gasofa, nave);
        LoopPickItems(*player, itemdrop, spritesItems);

        MoverNave(nave);

        //
        //
        ActualizarColisionParteNave(parteNave);

        ColisionPartesNaveJugador(parteNave, player, *primera_colocada);
        printf("parte nave pos x [%f], pos y[%f]", parteNave->parteNaveConfig.position.x, parteNave->parteNaveConfig.position.y);

        ColisionColocarPartes(nave, parteNave, player);
        printf("parte nave pos x [%f], pos y[%f]", parteNave->parteNaveConfig.position.x, parteNave->parteNaveConfig.position.y);

    }
}

void DrawAll(Sprites *spritesColores, Sprites *spritesPersonaje, Bala *punteroBalas, Jugador player, int frame,
             COL::object gasofa, Sprites *spritesItems, ItemDrop itemdrop, TPlatform *g_platforms, esat::SpriteHandle *platform_sprite,
             TGame game, esat::SpriteHandle *loading_sprite, int menu_selection_player, int menu_selection_control, int menu_highlight_white, esat::SpriteHandle sprite_vidas,
             Nave *nave, Sprites *spritesNave)
{
    if (game.current_screen == TScreen::IMAGE)
        InitialImage(loading_sprite);
    if (game.current_screen == TScreen::MAIN_MENU)
        MainMenu(menu_selection_player, menu_selection_control, menu_highlight_white, game, sprite_vidas);
    if (game.current_screen == TScreen::GAME_SCREEN)
    {
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

        DibujarGasofa(gasofa, spritesItems, *nave);
        DibujarItems(itemdrop, spritesItems);
        // DibujarNave(nave, spritesNave);

        //
        DibujarPartesNave(parteNave, spritesNave);

        // debug colisiones
        COL::ShowColision(nave->nave_config.colision);
        COL::ShowColision(player.config_colision.colision);
        COL::ShowColision(parteNave[0].parteNaveConfig.colision);
        COL::ShowColision(parteNave[1].parteNaveConfig.colision);
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
    int frame;
    bool moverLeft, moverRight, ascender;
    Sprites *spritesColores = nullptr, *spritesPersonaje = nullptr, *spritesItems = nullptr, *SpritesNaves = nullptr;
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

    // Partes Nave

    InitiateAll(&spritesColores, &spritesPersonaje, &punteroBalas, &spritesItems, &player, &gasofa, &itemdrop, &platform_sprite, &g_platforms, &loading_sprite, &game,
                &sprite_lives, &SpritesNaves, &nave);

    // Main game loop
    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
    {
        InitiateFrame();
        TestMousePosition();
        GetInput(&moverLeft, &moverRight, &ascender, punteroBalas, player, &game, &menu_selection_player, &menu_selection_control);
        Update(&player, ascender, punteroBalas, moverLeft, moverRight, &frame, gasofa, &itemdrop, spritesItems, g_platforms, &game, &timer,
               &menu_blink_timer, &menu_highlight_white, &nave, &primeraColocada);
        DrawAll(spritesColores, spritesPersonaje, punteroBalas, player, frame, gasofa, spritesItems, itemdrop, g_platforms, platform_sprite,
                game, loading_sprite, menu_selection_player, menu_selection_control, menu_highlight_white, sprite_lives, &nave, SpritesNaves);

        FinishFrame();
    }

    // Destroy window
    esat::WindowDestroy();
    FreeAll(&spritesColores, &spritesPersonaje, &spritesItems, &punteroBalas, &gasofa, &itemdrop);
    return 0;
}
