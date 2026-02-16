#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// esto es solo como referencia, se va a cambiar
struct game_memory{
  int P1, P2, HI;   // 1-UP 2-UP  HIGHSCORE
  int LVL;          // Nivel
  int ShipStatus;   // ponele que de 0 a 8 rapresentando el progreso de reparacion y combustible
  // etc...
};

/*esto de MemoryBlock tambien se puede transformare 
  en mas variables llamadas que despues pongo 
  todo junto adentro de la funcion*/
void loadFromFile(game_memory **MemoryBlock){
  // este FILE *fichero va a estar en el main y aca simplemente carga
  FILE *fichero;
  fichero = fopen("save.dat", "rb");

  if(fichero != NULL){
    game_memory valor;
    fread(&valor, sizeof(game_memory), 1, fichero);

    *MemoryBlock = (game_memory*)malloc(1*sizeof(game_memory));
    **(MemoryBlock) = valor;
    fclose(fichero);
  }
  else{
    printf("No se pudo encontrar un punto de guardado");
  }
}

void saveProgress(game_memory **MemoryBlock){
  FILE *fichero;
  fichero = fopen("save.dat", "wb");
  fwrite((MemoryBlock), sizeof(game_memory), 1, fichero);
  
  fclose(fichero);
}