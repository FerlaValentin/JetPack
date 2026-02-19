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
SoLoud::Wav s_launch;

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
    case launch:      Soloud.play(s_launch);      break;
  }
  printf("audio reproducido");
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
    s_launch.load("./assets/audio/spacedep1.wav");
}

void FreeAudio(){
  Soloud.deinit();
}
const int KWindow_Width = 256 * 2;
const int KWindow_Height = 192 * 2;
double delta_time;
unsigned char fps = 25;
double current_time;
double last_time;


int esat::main(int argc, char **argv) {

	esat::WindowInit(KWindow_Width, KWindow_Height);
	esat::WindowSetMouseVisibility(true);

	srand((unsigned)time(nullptr));
	last_time = esat::Time();

  // Main game loop
  while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
    
    // Calculate time elapsed since the last frame
    current_time = esat::Time();
    delta_time = (current_time - last_time) / 1000.0;
    // Limit delta_time to avoid large jumps
    if (delta_time > 0.1) delta_time = 0.1;
    
    last_time = current_time;

    esat::DrawBegin();
    esat::DrawClear(0,0,0);



    // Finish drawing
    esat::DrawEnd();
    // Control frame speed (fps)
    do{
      current_time = esat::Time();
    }while((current_time - last_time) <= 1000.0/fps);
    esat::WindowFrame();
  }

  // Destroy window
  esat::WindowDestroy();
  return 0;
}