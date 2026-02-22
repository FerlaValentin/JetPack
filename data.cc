// Save player data
// Save game data

void SavePlayerDataToFile(Jugador *player1, Jugador *player2 = nullptr){
  FILE *fichero;
  fichero = fopen("save.dat", "wb");
  if (fichero != NULL){
    fwrite(player1, sizeof(Jugador), 1, fichero);
    if (player2 != nullptr)
      fwrite(player2, sizeof(Jugador), 1, fichero);
    fclose(fichero);
  }
}

void LoadPlayerDataFromFile(Jugador *player, int player_id = 1){
  FILE *fichero;
  Jugador tmp = *player;
  if ((fichero = fopen("save.dat", "rb")) != nullptr){
    while (fread(&tmp, sizeof(Jugador), 1, fichero) != 0){
      if (tmp.player_id == player_id){
        *player = tmp;
      }
    }
    fclose(fichero);
  } else {
    printf("[ERROR] Error loading player data\n");
  }
  printf("[DEBUG] Player data loaded for player %d\n", player_id);
}

void DeletePlayerDataFiles(){
  if (remove("save.dat") != 0){
    printf("Error deleting file\n");
  }
}