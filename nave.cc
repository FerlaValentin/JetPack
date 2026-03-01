/*
// Windows dimensions with default values
const int KWindow_Width = 800;
const int KWindow_Height = 800;

// FPS
double delta_time;
unsigned char fps = 25;
double current_time;
double last_time;*/
#include "merge.h"
#include "nave.h"
#include "jugador.h"

Sprites *InstanciarSpritesNave(int numSprite)
{
    return (Sprites *)malloc(sizeof(Sprites) * numSprite);
}

void InitSpriteNave(Sprites *punteroSprites)
{
    punteroSprites[0].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_1_2x.png");
    punteroSprites[1].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_1_2x.png");
    punteroSprites[2].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_1_2x.png");
    punteroSprites[3].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_2_2x.png");
    punteroSprites[4].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_2_2x.png");
    punteroSprites[5].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_2_2x.png");
    punteroSprites[6].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_3_2x.png");
    punteroSprites[7].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_3_2x.png");
    punteroSprites[8].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_3_2x.png");
    punteroSprites[9].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cabeza_nave_4_2x.png");
    punteroSprites[10].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cuerpo_nave_4_2x.png");
    punteroSprites[11].sprite = esat::SpriteFromFile("./SPRITES/NAVE/cola_nave_4_2x.png");
    punteroSprites[12].sprite = esat::SpriteFromFile("./SPRITES/NAVE/fondo_blanco.png");
    punteroSprites[13].sprite = esat::SpriteFromFile("./SPRITES/NAVE/fondo_rosa.png");
    punteroSprites[14].sprite = esat::SpriteFromFile("./SPRITES/NAVE/explosion_1_2x.png");
    punteroSprites[15].sprite = esat::SpriteFromFile("./SPRITES/NAVE/explosion_2_2x.png");
}

/////////////////////// Partes Nave ///////////////////////

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
    cabeza.colocada = false;
    cabeza.colisionNave = false;
    parteNave[0] = cabeza;

    ParteNave cuerpo;
    cuerpo.parteNaveConfig.position.x = 256;
    cuerpo.parteNaveConfig.position.y = 180 - 32;
    cuerpo.parteNaveConfig.height = measure;
    cuerpo.parteNaveConfig.width = measure;
    cuerpo.parteNaveConfig.colision = COL::CreateColision(cuerpo.parteNaveConfig);
    cuerpo.recogido = false;
    cuerpo.colocada = false;
    cuerpo.colisionNave = false;
    parteNave[1] = cuerpo;

    ParteNave cola;
    // cola.parteNaveConfig.position.x = 325;
    cola.parteNaveConfig.position.x = kScreenWidth - 180;
    cola.parteNaveConfig.position.y = (kScreenHeight - 16) - 32;
    cola.parteNaveConfig.height = kScreenHeight;
    cola.parteNaveConfig.width = measure;
    cola.recogido = true;
    cola.colocada = true;
    parteNave[2] = cola;
}

void ActualizarColisionParteNave(ParteNave *parteNave)
{
    for (int i = 0; i < 2; i++)
    {
        parteNave[i].parteNaveConfig.colision = COL::CreateColision(parteNave[i].parteNaveConfig);
    }
}

void MoverParte(ParteNave *parteNave, Nave *nave)
{
    // este for es e 2 porque la base ya està posicionada
    for (int i = 0; i < 2; i++)
    {
        // si parte nave esta arriba de nave
        if (parteNave[i].colisionNave && !parteNave[i].colocada)
        {
            // el jugador la suelta, centra la parte y la hace caer
            parteNave[i].recogido = false;
            parteNave[i].parteNaveConfig.position.x = nave->pos.x;
            parteNave[i].parteNaveConfig.position.y += 2;

            // si la parte toca la nave entoces la monta
            if (parteNave[i].parteNaveConfig.position.y >= nave->pos.y)
            {
                // dato curioso el indice està al reves entonces el de arriba es 0 y el de abajo es 1
                // pero no importa porque despues se muestra la nave de todos modos
                parteNave[i].parteNaveConfig.position.y = nave->nave_config.height - (i*32) - 32; 
                parteNave[i].colocada = true;
            }
        }
    }
}

// esto detecta colision entre la nave y la parte
void ColisionColocarPartes(Nave *nave, ParteNave *parte_nave, Jugador *player)
{
    for (int i = 0; i < 2; i++)
    {
        if ((COL::CheckColision(nave->nave_config.colision, parte_nave[i].parteNaveConfig.colision)))
        {
            parte_nave[i].colisionNave = true;
            parte_nave[i].recogido = false;
        }
    }
}

void ActualizarPosParteNave(ParteNave *parteNave, Jugador *player)
{
    for (int i = 0; i < 2; i++)
    {
        // si el jugador lo tiene y no esta colocado entonces posicion parte es posicion jugador
        if (parteNave[i].recogido && !parteNave[i].colocada)
        {
            parteNave[i].parteNaveConfig.position.x = player->pos.x + 16;
            parteNave[i].parteNaveConfig.position.y = player->pos.y + 24;
        }
    }
}

void ColisionPartesNaveJugador(ParteNave *parteNave, Jugador *player)
{
    // para no agarrar la punta antes se verifica solo con la parte del centro
    if (COL::CheckColision(player->config_colision.colision, parteNave[1].parteNaveConfig.colision) && !parteNave[1].recogido && !parteNave[1].colisionNave)
    {
        // si detecta colision y no fue puesto en la nave
        parteNave[1].recogido = true;
        // printf("Colision objeto 1\n");
    }
    else if (parteNave[1].colocada)
    {
        if (COL::CheckColision(player->config_colision.colision, parteNave[0].parteNaveConfig.colision) && !parteNave[0].recogido && !parteNave[0].colisionNave)
        {
            parteNave[0].recogido = true;
            // printf("Colision objeto 0\n");
        }
    }
    ActualizarPosParteNave(parteNave, player);
}

// esto es colision parte colision nave
void ColisionColocarPartes(Nave *nave, ParteNave **parte_nave, Jugador *player)
{
    for (int i = 0; i < 2; i++)
    {
        if ((COL::CheckColision(nave->nave_config.colision, parte_nave[i]->parteNaveConfig.colision)))
        {
            parte_nave[i]->colisionNave = true;
            parte_nave[i]->recogido = false;
        }
    }
}

void DibujarPartesNave(ParteNave *parteNave, Sprites *punteroSprites)
{
    // si no todas las partes fueron colocadas entonces dibuja
    if(!parteNave[0].colocada || !parteNave[1].colocada){
        int height = 32;
        for (int i = 0; i < 3; i++)
        {
            esat::DrawSprite(punteroSprites[12].sprite, parteNave[i].parteNaveConfig.position.x, parteNave[i].parteNaveConfig.position.y);
            esat::DrawSprite(punteroSprites[i].sprite, parteNave[i].parteNaveConfig.position.x, parteNave[i].parteNaveConfig.position.y);
        }
    }
}

/////////////////////// NAVE ///////////////////////

void InstanciarNave(Nave *nave)
{
    const int terrain_height = 16;
    nave->vel = 100;
    nave->pos.x = kScreenWidth - 180;
    nave->height = terrain_height * 2;
    nave->nave_config.height = kScreenHeight - terrain_height;      // al parecer esto ahora es donde toca el piso
    nave->pos.y = nave->nave_config.height - (nave->height * 3);
    nave->fuelAmount = 0;
    nave->direccion = Direction::STATIC;
    nave->nave_config.position.x = nave->pos.x;
    nave->nave_config.position.y = 0;
    nave->nave_config.width = 32;
}

void MoverNave(Nave *nave, COL::object P, bool *visible, int* vidas)
{
    const int terrain_height = 16;
    if (nave->direccion == Direction::STATIC &&
        nave->fuelAmount == 6 &&
        COL::CheckColision(P.colision, nave->nave_config.colision) &&
        P.colision.p1.y > (kScreenHeight - (32 * 4)))
    {
        nave->direccion = Direction::UP;
        *visible = true;
        (*vidas)++;
    }
    switch (nave->direccion)
    {
    case Direction::UP:
        nave->pos.y -= nave->vel * delta_time;
        if (nave->pos.y + nave->height * 2 <= nave->height)
        {
            nave->direccion = Direction::DOWN;
            nave->fuelAmount = 0;
        }
        break;

    case Direction::DOWN:
        nave->pos.y += nave->vel * delta_time;
        if (nave->pos.y + nave->height * 3 >= kScreenHeight - terrain_height)
        {
            nave->pos.y = kScreenHeight - terrain_height - nave->height * 3;
            nave->direccion = Direction::STATIC;
            *visible = false;
        }
        break;
    }
}

bool NaveEstaArriba(Nave *nave)
{
    if (nave->pos.y < 20)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void RellenarFuelNave(Nave *nave)
{
    // Detecta colision y player tiene fuel
    if (esat::IsKeyDown('B') || esat::IsKeyDown('b'))
        if (nave->fuelAmount <= 6)
            nave->fuelAmount++;
}

/*Dibuja las capas del cohete (fondo blanco, fondo rosa y el sprite transparente)
    param max_sprites El máximo de sprites que debe dibujar en cada capa
    param *punteroSprites Donde se guardan los sprites de la nave
    param sprite_ptr_position La posicion dentro del puntero del sprite a dibujar o -1 si tiene que iterar con el indice del for
    param pos_x La posicion en X del sprite a dibujar
    param pos_y La posicion en Y del sprite a dibujar
    param height La altura a la que se debe dibujar cada sprite respecto a pos_y
*/
void DrawRocketLayer(int max_sprites, const Sprites *punteroSprites, int sprite_ptr_position, float pos_x, float pos_y, int height)
{
    for (int i = 0; i < max_sprites; ++i)
    {
        if (sprite_ptr_position >= 0)
            esat::DrawSprite(punteroSprites[sprite_ptr_position].sprite, pos_x, pos_y + height * i);
        if (sprite_ptr_position == -1)
            esat::DrawSprite(punteroSprites[i].sprite, pos_x, pos_y + height * i);
    }
}

/*Rellena el puntero de Vec2 con los puntos del fondo rojo de la explosion
    param explosion_path El puntero de Vec2 para rellenar
    param nave El objeto de la nave para coger
    param explosion_size Los pixeles de ancho/alto del sprite de la explosion
*/
void FillVec2PtrPoints(esat::Vec2 *explosion_path, Nave nave, int explosion_size)
{
    explosion_path[0] = {nave.pos.x, nave.pos.y + nave.height * 3};
    explosion_path[1] = {nave.pos.x + explosion_size, nave.pos.y + nave.height * 3};
    explosion_path[2] = {nave.pos.x + explosion_size, nave.pos.y + nave.height * 3 + explosion_size};
    explosion_path[3] = {nave.pos.x, nave.pos.y + nave.height * 3 + explosion_size};
}

/*Dibuja el fondo rojo de la explosion y el sprite transparente de la explosion
    param explosion_path El puntero con las posiciones Vec2 de los vertices del fondo
    param punteroSprites El puntero con los sprites de las explosiones
*/
void DrawRocketExplosion(esat::Vec2 *explosion_path, const Sprites *punteroSprites)
{
    static float explosion_flicker = 0.0f;
    esat::DrawSetFillColor(255, 0, 0);
    esat::DrawSolidPath(&explosion_path->x, 4);
    if ((int)(explosion_flicker / 0.075) % 2)
        esat::DrawSprite(punteroSprites[14].sprite, explosion_path->x, explosion_path->y);
    else
        esat::DrawSprite(punteroSprites[15].sprite, explosion_path->x, explosion_path->y);
    explosion_flicker += delta_time;
}

/*Prepara el puntero de las posiciones de los vertices del fondo y llama a la funcion de dibujar
    param nave El objeto de la nave para sacar las posiciones del sprite y del fondo
    param explosion_size El ancho/alto del sprite de la explosion
    param punteroSprites El puntero con los sprites de la explosion
*/
void SetRocketExplosion(Nave nave, int explosion_size, const Sprites *punteroSprites)
{
    esat::Vec2 *explosion_path = (esat::Vec2 *)malloc(4 * sizeof(esat::Vec2));
    FillVec2PtrPoints(explosion_path, nave, explosion_size);
    DrawRocketExplosion(explosion_path, punteroSprites);
}

void DibujarNave(Nave *nave, Sprites *punteroSprites, ParteNave *punteroParteNave)
{
    // dibuja la nave solo 
    if(punteroParteNave[0].colocada && punteroParteNave[1].colocada){
        const int terrain_height = 16, pink_sprite_height = 16, max_sprites = 6, explosion_size = 32, rocket_total_height = nave->height * 3,
                  initFuelY = nave->pos.y + rocket_total_height - pink_sprite_height;
    
        // Blanco
        DrawRocketLayer(3, punteroSprites, 12, nave->pos.x, nave->pos.y, nave->height);
        // Rosa
        DrawRocketLayer(nave->fuelAmount, punteroSprites, 13, nave->pos.x, initFuelY, -pink_sprite_height);
        // Nave
        DrawRocketLayer(3, punteroSprites, -1, nave->pos.x, nave->pos.y, nave->height);
    
        // Calcula si el espacio entre el suelo y la base del cohete es como mínimo igual a explosion_size
        if (kScreenHeight - terrain_height - (nave->pos.y + rocket_total_height) >= explosion_size)
            SetRocketExplosion(*nave, explosion_size, punteroSprites);
    }
}