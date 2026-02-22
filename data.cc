// Save player data
// Save game data

void SavePlayerDataToFile(Jugador *player){
  FILE *fichero;
  fichero = fopen("save.dat", "wb");
  if (fichero != NULL){
    fwrite(player, sizeof(Jugador), 1, fichero);
    fclose(fichero);
  }
}

void LoadPlayerDataFromFile(Jugador *player){
  FILE *fichero;
  fichero = fopen("save.dat", "rb");
  if (fichero != NULL){
    fread(player, sizeof(Jugador), 1, fichero);
    fclose(fichero);
  }
}

void DeletePlayerDataFiles(){
  if (remove("save.dat") != 0){
    printf("Error deleting file\n");
  }
}