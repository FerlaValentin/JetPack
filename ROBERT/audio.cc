#include <esat/window.h>
#include <esat/draw.h>
#include <esat/input.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <time.h>
#include <esat_extra/soloud/soloud.h>
#include <esat_extra/soloud/soloud_wav.h>
#include <stdio.h>

enum audio{
  enemyDies,
  fallObject,
  getFuel,
  getItem,
  shoot,
  landing,
  start,
  intro,
  menu,
  launch
};

/*
escribir extern SoLoud::Soloud Soloud;
para llamar al reproductor de audio

poner AudioInit(); antes del while en el main
y poner FreeAudio(); antes de cerrar el programa
*/

SoLoud::Soloud Soloud;

SoLoud::Wav enemyDeath;
SoLoud::Wav fallSound;
SoLoud::Wav fuelSound;
SoLoud::Wav itemSound;
SoLoud::Wav fireSound;
SoLoud::Wav landingSound;
SoLoud::Wav starSound;
SoLoud::Wav introSound;
SoLoud::Wav menuSound;
SoLoud::Wav launchSound;

void PlayAudio(audio selectAudio) {
  switch(selectAudio) {
    case enemyDies:   Soloud.play(enemyDeath);    break;
    case fallObject:  Soloud.play(fallSound);     break;
    case getFuel:     Soloud.play(fuelSound);     break;
    case getItem:     Soloud.play(itemSound);     break;
    case shoot:       Soloud.play(fireSound);     break;
    case landing:     Soloud.play(landingSound);  break;
    case start:       Soloud.play(starSound);     break;
    case intro:       Soloud.play(introSound);    break;
    case menu:        Soloud.play(menuSound);     break;
    case launch:      Soloud.play(launchSound);   break;
  }
}

void AudioInit() {
    Soloud.init();

    enemyDeath.load("./assets/audio/enemydies.wav");
    fallSound.load("./assets/audio/fallobject.wav");
    fuelSound.load("./assets/audio/get_fuel.wav");
    itemSound.load("./assets/audio/get_item.wav");
    fireSound.load("./assets/audio/shoot.wav");
    landingSound.load("./assets/audio/startgameplay.wav");
    starSound.load("./assets/audio/startgameplay.wav");
    introSound.load("./assets/audio/intro.wav");
    menuSound.load("./assets/audio/menu_intro.wav");
    launchSound.load("./assets/audio/spacedep1.wav");
}

void FreeAudio(){
  Soloud.deinit();
}