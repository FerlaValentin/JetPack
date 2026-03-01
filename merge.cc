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

// #include "colisiones.h"
// #include "nave.cc"
// #include "enemigos.h"
// #include "jugador.cc"

#include "merge.h"
#include "interface.h"
#include "colisiones.h"
#include "nave.h"
#include "enemigos.h"
#include "jugador.h"
#include "audio.h"
#include "data.h"
#include "level_config.h"
// FPS
unsigned char fps = 25;
double current_time;
double last_time = 0;
double delta_time;

// INCLUIR LUEGO
bool LevelCompleted(Nave *nave)
{
    if (nave->pos.y + nave->height * 2 <= nave->height)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SetUp_LevelForPlayer(ENE::EnemyManager *mgr, int level)
{
    ENE::ResetEnemies(mgr);
    LevelConfig config = GetLevelConfig(level);
    for (int i = 0; i < config.amount_enemies; i++)
    {
        ENE::SpawnEnemy(mgr, config.enemy_type, -32, rand()% 344);
    }
}

void SwitchSessions(PlayerSession sessions[2], PlayerSession **current_session, TGame *game,
                    Nave *nave, ParteNave *punteroParteNave)
{
    (*current_session)->player.level = (*current_session)->level;

    Jugador *dying = &(*current_session)->player;
    dying->muerto          = false;
    dying->colisiona       = true;
    dying->timer_muerte    = 0.0f;
    dying->timer_inv_actual = 0.0f;
    dying->pos.x = kScreenWidth / 2;
    dying->pos.y = kScreenHeight - dying->spriteHeight - 16;

    int indice_actual = dying->player_id - 1;
    int otro_indice   = 1 - indice_actual;

    if (sessions[otro_indice].player.vidas > 0)
    {
        game->current_player_id = otro_indice + 1;
        *current_session = &sessions[otro_indice];
        SetUp_LevelForPlayer(&(*current_session)->enemies, (*current_session)->level);
        printf("Cambio de sesion, jugador nuevo %d nivel %d vidas %d\n",
               (*current_session)->player.player_id,
               (*current_session)->level,
               (*current_session)->player.vidas);
    }
    else
    {
        DeletePlayerDataFiles();
        DeleteGameDataFiles();
        game->current_screen = TScreen::GAME_OVER;
    }
}



void DebugRefillFuel(Nave *nave)
{
    if (esat::IsSpecialKeyPressed(esat::kSpecialKey_F1))
    {
        nave->fuelAmount ++;   // Ajusta a tu nombre real de variables
        printf("[DEBUG] Fuel rellenado\n");
    }
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

void InitiateAll(Sprites **spritesColores, Sprites **spritesPersonaje, Bala **punteroBalas, Sprites **spritesItems,
                 ItemDrop *gasofa, ItemDrop *itemdrop, esat::SpriteHandle **platform_sprite, TPlatform **g_platforms,
                 esat::SpriteHandle **loading_sprite, TGame *game, esat::SpriteHandle *sprite_lives, Sprites **spritesNave, Nave *nave,
                 ENE::VisualEffect **g_fx_pool, esat::SpriteHandle **g_fx_sprites, ParteNave **punteroParteNave, PlayerSession sessions[])
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
    // *spritesNave = InstanciarSpritesNave(16); // ! cambiar importante
    *spritesNave = (Sprites *)malloc(16 * sizeof(Sprites));
    *punteroParteNave = (ParteNave *)malloc(sizeof(ParteNave) * 3);
    
    // SPRITES
    InstanciarSpritesColores(*spritesColores);
    InstanciarSpritesPlayer(*spritesPersonaje);
    InstanciarSpritesItems(*spritesItems);
    InitSpriteNave(*spritesNave);
    InitPlatformSprites(platform_sprite, g_platforms);
    InitLoadingSprites(loading_sprite);
    
    // INSTANCIAR
    InstanciarBalas(*punteroBalas);
    InstaciarGasofa_Nave(gasofa, (*spritesItems)[5]);
    AudioInit();
    InstanciarItems(itemdrop, *spritesItems);
    InstanciarNave(nave);
    InitGameVariables(game);
    InitLivesSprite(sprite_lives); // Fuente
    LoadFonts();
    InstanciarPartesDeLaNave(*punteroParteNave);
    ENE::InitVFXSystem(g_fx_pool, g_fx_sprites);
    
    // Load game data and player data if exists
    LoadGameDataFromFile(game);
    //LoadPlayerDataFromFile(&sessions[game->current_player_id - 1].player, game->current_player_id);
    LoadHiScoreFromFile(&game->hi_socore);
    for(int i = 0; i < 2; i++)
    {
        InstanciarPlayer(&(sessions[i].player), i + 1);
        ENE::InitManager(&(sessions[i].enemies), 10);
        sessions[i].level = 1; 
        //SetUp_LevelForPlayer(&sessions[i].enemies, sessions[i].level);
    }
    if (game->current_screen == TScreen::GAME_SCREEN)
    {
        if (LoadPlayerDataFromFile(&sessions[0].player, 1))
            sessions[0].level = sessions[0].player.level;
        else
            printf("Error load player data 1");
        
        if (game->num_players == 2 && LoadPlayerDataFromFile(&sessions[1].player, 2))
            sessions[1].level = sessions[1].player.level;
        else
            printf("Error load player data 2");
        
        SetUp_LevelForPlayer(&sessions[game->current_player_id - 1].enemies, sessions[game->current_player_id - 1].level);
    }

    /***
    
    */
}

void GetInput(bool *moverLeft, bool *moverRight, bool *ascender, Bala *punteroBalas, PlayerSession *current_session, TGame *game, int *menu_selection_player, int *menu_selection_control, 
                Nave *nave, ItemDrop *itemdrop, ItemDrop *gasofa, Sprites *spritesItems, ParteNave *punteroParteNave)
{
    Jugador* player = &current_session->player;
    if (game->current_screen == TScreen::GAME_SCREEN)
    {
        if (!player->muerto)
        {
            *moverLeft = (esat::IsKeyPressed('A') || esat::IsKeyPressed('a'));
            *moverRight = (esat::IsKeyPressed('D') || esat::IsKeyPressed('d'));
            *ascender = (esat::IsKeyPressed('W') || esat::IsKeyPressed('w'));
            CrearDisparos(punteroBalas, *player);
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
            InstanciarNave(nave);
            InstanciarItems(itemdrop, spritesItems);
            InstaciarGasofa_Nave(gasofa, spritesItems[5]);
            InstanciarPlayer(player, 1);
            InstanciarBalas(punteroBalas);
            InstanciarPartesDeLaNave(punteroParteNave);
            ENE::ResetEnemies(&current_session->enemies);
            SetUp_LevelForPlayer(&(*current_session).enemies, current_session->level);
            game->current_player_id = 1;
            if (*menu_selection_player == 1)
            {
                game->num_players = 2;
                // Save player 2 data
                Jugador player2;
                InstanciarPlayer(&player2, 2);
                player2.player_id = 2;
                player2.isActive = false;
                player2.muerto = true;
                player2.colisiona = false;
                SavePlayerDataToFile(player, &player2);
            }
            else
            {
                game->num_players = 1;
                SavePlayerDataToFile(player);
            }
            game->current_screen = TScreen::GAME_SCREEN;
            game->label_timer_blink = 3.0f;
        }
    }
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

    player->puntos++;
}


void Update(PlayerSession sessions[2], PlayerSession **current_session, bool *ascender, Bala *punteroBalas, bool *moverLeft, bool *moverRight, int *frame,
            ItemDrop &gasofa, ItemDrop *itemdrop, Sprites *spritesItems, TPlatform *g_platforms,
            TGame *game, float *timer, float *menu_blink_timer, bool *menu_highlight_white, Nave *nave,
            ParteNave *punteroParteNave, ENE::VisualEffect *g_fx_pool, esat::SpriteHandle *g_fx_sprites)
{
    if (game->current_screen != TScreen::GAME_SCREEN)
        ScreenSelector(game, timer, menu_blink_timer, menu_highlight_white);
    else
    {
        Jugador *player = &(*current_session)->player;
        ENE::EnemyManager *mgr = &(*current_session)->enemies;
        
        int level = (*current_session)->level;
        
        bool vida_perdida = false;
        if (player->muerto || !player->colisiona)
        {
            ResetPlayer_OnDead(player, ascender, moverLeft, moverRight, &vida_perdida);          
        }
        

        ActualizarDisparos(punteroBalas, *player);
        ControlarLimitesPantalla(player, punteroBalas);
        //! Cambiar también el tope de la altura para que no toque el HUD
        *frame = ActualizarAnimacionJugador(*player);

        // Pasar vidas y puntos a la interfaz
        UpdateInterface(&player->puntos, &player->vidas, &player->player_id, game);
        // TestValues(player); // @jhony: remove this
        RellenarFuelNave(nave);

        // ! Colisiones

        ActualizarColisionesItems(gasofa, *itemdrop, nave);
        LoopGasofa(*player, gasofa, nave, g_platforms);
        LoopPickItems(*player, itemdrop, spritesItems, *nave, g_platforms);

        LoopMoverJugador(*moverLeft, *moverRight, player);
        bool isOnPlatform = !player->volando;
        Ascender_Gravedad(player, *ascender);
        ColisionJugador(player); // Actualizar colider a player
        ActualizarColisionParteNave(punteroParteNave);

        ColisionPlayerPlatforma(*player, g_platforms); // No subir porque da error
        AnimationDust(player, isOnPlatform, g_fx_pool, g_fx_sprites);
        ColisionDisparos(punteroBalas, mgr, g_fx_pool, g_fx_sprites);

        MoverParte(punteroParteNave, nave);

        MoverNave(nave, player->config_colision, &player->muerto, &player->vidas);
        ColisionPartesNaveJugador(punteroParteNave, player);   // detecta colision entre parte y jugador
        ColisionColocarPartes(nave, punteroParteNave, player); // esto apila las partes
        
        if (LevelCompleted(nave))
        {
            printf("Entra level completed\n");
            (*current_session)->level++;
            (*current_session)->player.level = (*current_session)->level;
            printf("session %d level %d session %d level %d\n", sessions[0].player.player_id, sessions[0].level, sessions[1].player.player_id, sessions[1].level);
            printf("current %d level %d\n", (*current_session)->player.player_id, (*current_session)->level);
            SetUp_LevelForPlayer(&(*current_session)->enemies, (*current_session)->level);
        }
        ENE::UpdateEnemies(mgr, g_fx_pool, g_fx_sprites);
        EnemieAIAdvanced(mgr, player, g_platforms, g_fx_pool, g_fx_sprites);
        EnemiesCollision(mgr, player, &gasofa, *frame, game, g_fx_pool, g_fx_sprites);

        if (vida_perdida && game->num_players == 2 && nave->direccion == Direction::STATIC)
            SwitchSessions(sessions, current_session, game, nave, punteroParteNave);
            //SetUp_LevelForPlayer(&(*current_session)->enemies, (*current_session)->level);
    }
}

void DrawAll(PlayerSession *current_session,Sprites *spritesColores, Sprites *spritesPersonaje, Bala *punteroBalas, int frame,
             ItemDrop gasofa, Sprites *spritesItems, ItemDrop itemdrop, TPlatform *g_platforms, esat::SpriteHandle *platform_sprite,
             TGame game, esat::SpriteHandle *loading_sprite, int menu_selection_player, int menu_selection_control, int menu_highlight_white, esat::SpriteHandle sprite_vidas,
             Nave *nave, Sprites *spritesNave, ENE::VisualEffect *g_fx_pool, esat::SpriteHandle *g_fx_sprites, ParteNave *punteroParteNave)
{
    Jugador player = current_session->player;
    ENE::EnemyManager mgr = current_session->enemies;

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
        if (!player.colisiona && !player.muerto)
        {
            DibujarColoresJugador(spritesColores, player, 3);
            DibujarJugador(spritesPersonaje, player, frame);
        }
        DibujarItems(itemdrop, spritesItems);
        DibujarPartesNave(punteroParteNave, spritesNave);
        DibujarNave(nave, spritesNave, punteroParteNave);
        DibujarGasofa(gasofa, spritesItems, *nave);
        COL::ShowColision(player.config_colision.colision);
        ENE::DrawEnemies(mgr);
        ENE::DrawActiveVFX(g_fx_pool, g_fx_sprites);
    }
    if(game.current_screen == TScreen::GAME_OVER){
      GameOverScreen(&game, delta_time);
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
             ItemDrop *gasofa, ItemDrop *itemdrop, PlayerSession **current_session, ENE::VisualEffect **g_fx_pool_pointer, esat::SpriteHandle **g_fx_sprites_pointer)
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

    esat::SpriteRelease(gasofa->item_config.sprite);
    esat::SpriteRelease(itemdrop->item_config.sprite);
    FreeAudio();

    // Free enemigos
    ENE::FreeVFX(g_fx_pool_pointer, g_fx_sprites_pointer);
    for(int i = 0; i < 2; ++i)
        ENE::FreeManager(&(*current_session)[i].enemies);
    // free(*current_session);
    // *current_session = nullptr;
}

int esat::main(int argc, char **argv)
{
    int frame;
    bool moverLeft, moverRight, ascender;
    Sprites *spritesColores = nullptr, *spritesPersonaje = nullptr, *spritesItems = nullptr, *SpritesNaves = nullptr;
    Bala *punteroBalas = nullptr;
    ParteNave *parteNave = nullptr;

    PlayerSession sessions[2];     
    PlayerSession* current_session = nullptr;

    // Jugador player;

    ItemDrop gasofa;
    COL::object prueba_nave;
    ItemDrop itemdrop;
    Nave nave;
    
    // enemigos
    // ENE::EnemyManager *enemies;

    ENE::VisualEffect *g_fx_pool = nullptr;
    esat::SpriteHandle *g_fx_sprites = nullptr;
    int level = 1;
    bool toggle = false;

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

    InitiateAll(&spritesColores, &spritesPersonaje, &punteroBalas, &spritesItems, &gasofa, &itemdrop, &platform_sprite, &g_platforms, &loading_sprite, &game,
                &sprite_lives, &SpritesNaves, &nave, &g_fx_pool, &g_fx_sprites, 
                &parteNave, sessions);

    current_session = &sessions[game.current_player_id - 1];
    // colocar tambien al cambiar de jugador
    // Main game loop
    while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape))
    {
        InitiateFrame();

        GetInput(&moverLeft, &moverRight, &ascender, punteroBalas, current_session, &game, &menu_selection_player, &menu_selection_control, &nave, &itemdrop, &gasofa, spritesItems, parteNave);
        Update(sessions, &current_session, &ascender, punteroBalas, &moverLeft, &moverRight, &frame, gasofa, &itemdrop, spritesItems, g_platforms, &game, &timer,
               &menu_blink_timer, &menu_highlight_white, &nave, parteNave, g_fx_pool, g_fx_sprites);
        DrawAll(current_session, spritesColores, spritesPersonaje, punteroBalas, frame, gasofa, spritesItems, itemdrop, g_platforms, platform_sprite,
                game, loading_sprite, menu_selection_player, menu_selection_control, menu_highlight_white, sprite_lives, &nave, SpritesNaves, g_fx_pool, g_fx_sprites, parteNave);

        FinishFrame();
    }

    // Save game data and player data if game is close on mid game
    // Save hi-score always when game is closed
    SaveHiScoreToFile(game.hi_socore);
    if(game.current_screen == TScreen::GAME_SCREEN){
        sessions[0].player.level = sessions[0].level;
        if (game.num_players == 2)
            sessions[1].player.level = sessions[1].level;
        if (game.num_players == 2)
            SavePlayerDataToFile(&sessions[0].player, &sessions[1].player);
        else
            SavePlayerDataToFile(&sessions[0].player);

      SaveGameDataToFile(&game);
    }else{
      DeleteGameDataFiles();
      DeletePlayerDataFiles();
    }

    // Destroy window
    esat::WindowDestroy();
    FreeAll(&spritesColores, &spritesPersonaje, &spritesItems, &punteroBalas, &gasofa, &itemdrop, &current_session, &g_fx_pool, &g_fx_sprites);
    return 0;
}
