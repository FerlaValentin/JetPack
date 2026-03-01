// Save player data
// Save game data
#include "data.h"
#include "jugador.h"

static PlayerSaveData JugadorToSaveData(Jugador *player)
{
  PlayerSaveData sd;
  sd.player_id = player->player_id;
  sd.level     = player->level;
  sd.vidas     = player->vidas;
  sd.puntos    = player->puntos;
  return sd;
}

static void ApplySaveDataToJugador(Jugador *player, PlayerSaveData *sd)
{
  player->player_id = sd->player_id;
  player->level     = sd->level;
  player->vidas     = sd->vidas;
  player->puntos    = sd->puntos;
}

void SavePlayerDataToFile(Jugador *player1, Jugador *player2)
{
  FILE *fichero;
  fichero = fopen("save.dat", "wb");
  if (fichero != NULL)
  {
    PlayerSaveData sd1 = JugadorToSaveData(player1);
    fwrite(&sd1, sizeof(PlayerSaveData), 1, fichero);
    printf("guardar jugador %d nivel %d\n", sd1.player_id, sd1.level);
    if (player2 != nullptr)
    {
      PlayerSaveData sd2 = JugadorToSaveData(player2);
      fwrite(&sd2, sizeof(PlayerSaveData), 1, fichero);
      printf("guardar jugador %d nivel %d\n", sd2.player_id, sd2.level);
    }
    fclose(fichero);
  }
}

bool LoadPlayerDataFromFile(Jugador *player, int player_id)
{
  FILE *fichero;
  PlayerSaveData tmp;
  bool is_loaded = false;
  if ((fichero = fopen("save.dat", "rb")) != nullptr)
  {
    while (fread(&tmp, sizeof(PlayerSaveData), 1, fichero) != 0)
    {
      if (tmp.player_id == player_id)
      {
        ApplySaveDataToJugador(player, &tmp);
        is_loaded = true;
      }
    }
    fclose(fichero);
  }
  else
  {
    printf("[ERROR] Error loading player data\n");
  }
  printf("[DEBUG] Player data loaded for player %d level %d lives %d\n", player->player_id, player->level, player->vidas);
  return is_loaded;
}

void DeletePlayerDataFiles()
{
  if (remove("save.dat") != 0)
  {
    printf("Error deleting file\n");
  }
}

void SaveGameDataToFile(TGame* game){
  FILE *fichero;
  fichero = fopen("game.dat", "wb");
  if (fichero != NULL){
    fwrite(game, sizeof(TGame), 1, fichero);
    fclose(fichero);
  }
}

void LoadGameDataFromFile(TGame *game){
  FILE *fichero;
  TGame tmp;
  if ((fichero = fopen("game.dat", "rb")) != nullptr){
    while (fread(&tmp, sizeof(TGame), 1, fichero) != 0){
      *game = tmp;
      game->label_timer_blink = 0.0f;
      if (game->num_players != 1 && game->num_players != 2)
        game->num_players = 1;
    }
    fclose(fichero);
  }
}

void SaveHiScoreToFile(int hi_score){
  FILE *fichero;
  fichero = fopen("hi_score.dat", "wb");
  if (fichero != NULL){
    fwrite(&hi_score, sizeof(int), 1, fichero);
    fclose(fichero);
  }
}

void LoadHiScoreFromFile(int *hi_score){
  FILE *fichero;
  int tmp;
  if ((fichero = fopen("hi_score.dat", "rb")) != nullptr){
    while (fread(&tmp, sizeof(int), 1, fichero) != 0){
      *hi_score = tmp;
    }
    fclose(fichero);
  }
}

void DeleteGameDataFiles(){
  if (remove("game.dat") != 0){
    printf("Error deleting file\n");
  }
}