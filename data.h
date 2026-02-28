#ifndef DATA_H
#define DATA_H

#include <cstdio>
#include "jugador.h"

// Guarda uno o dos jugadores en el archivo
void SavePlayerDataToFile(Jugador *player1, Jugador *player2 = nullptr);

// Carga datos de un jugador concreto según su player_id
void LoadPlayerDataFromFile(Jugador *player, int player_id = 1);

// Borra el archivo de guardado
void DeletePlayerDataFiles();

// Guarda datos de la partida en el archivo
void SaveGameDataToFile(TGame* game);

// Carga datos de la partida en el archivo
void LoadGameDataFromFile(TGame* game);

// Guarda el hi-score en el archivo
void SaveHiScoreToFile(int hi_score);

// Carga el hi-score desde el archivo
void LoadHiScoreFromFile(int* hi_score);

// Borra el archivo de guardado de la partida
void DeleteGameDataFiles();


#endif