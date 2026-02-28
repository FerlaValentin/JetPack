// Save player data
// Save game data
#include "jugador.h"
void SavePlayerDataToFile(Jugador *player1, Jugador *player2)
{
  FILE *fichero;
  fichero = fopen("save.dat", "wb");
  if (fichero != NULL)
  {
    fwrite(player1, sizeof(Jugador), 1, fichero);
    if (player2 != nullptr)
      fwrite(player2, sizeof(Jugador), 1, fichero);
    fclose(fichero);
  }
}

void LoadPlayerDataFromFile(Jugador *player, int player_id)
{
  FILE *fichero;
  Jugador tmp;
  if ((fichero = fopen("save.dat", "rb")) != nullptr)
  {
    while (fread(&tmp, sizeof(Jugador), 1, fichero) != 0)
    {
      if (tmp.player_id == player_id)
      {
        *player = tmp;
      }
    }
    fclose(fichero);
  }
  else
  {
    printf("[ERROR] Error loading player data\n");
  }
  printf("[DEBUG] Player data loaded for player %d\n", player->player_id);
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