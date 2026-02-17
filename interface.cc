// TODO(@jhony): Optimize this scripts

extern const int KWindow_Width;
extern const int KWindow_Height; 

enum TScreen {
  MAIN_MENU,
  GAME_SCREEN
};

// Platform collition = sprite_width * size x sprite_height
struct TPlatform {
  esat::Vec2 pos;
  int size;
};

// test
const unsigned char kplatform_numbers = 3;

esat::SpriteHandle* platform = nullptr;


void ReserveMemory(){
  printf("[DEBUG] sizeof(esat::SpriteHandle) %d\n", sizeof(esat::SpriteHandle));
  platform = (esat::SpriteHandle*)malloc(4 * sizeof(esat::SpriteHandle));
  if (platform == nullptr){
    printf("Error: reserve memory\n");
  }else{
    printf("All ok\n");
  }
}

void InitPlatformSprites(){
  ReserveMemory();

  *(platform + 0) = esat::SpriteFromFile("./SPRITES/TERRENO/terreno_1_2x.png"); // First platform
  *(platform + 1) = esat::SpriteFromFile("./SPRITES/TERRENO/terreno_2_2x.png"); // Middle platform
  *(platform + 2) = esat::SpriteFromFile("./SPRITES/TERRENO/terreno_3_2x.png"); // Last platform

  for (int i = 0; i < 4;  ++i){
    if ((platform + i) == nullptr){
      printf("Error: on sprite %d\n", i);
    }
  }
}

void LoadFonts(){
  // printf("[DEBUG] call LoadFonts\n");
  esat::DrawSetTextFont("assets/fonts/zx_spectrum-7.ttf");
}


// TODO: fix color bg
void MainMenu(){
  // printf("[DEBUG] call MainMenu\n");
  DrawHeader();

  esat::DrawSetTextSize(20);
  esat::DrawText(60, 70, "JETPAC GAME SELECTION");

  esat::DrawText(60, 110, "1  1 PLAYER GAME");
  esat::DrawText(60, 140, "2  2 PLAYER GAME");
  esat::DrawText(60, 170, "3  KEYBOARD");
  esat::DrawText(60, 200, "4  KEMPSTON JOYSTICK"); // not selectable

  esat::DrawText(60, 250, "5  START GAME");
  
  esat::DrawText(10, 350, "@1983 A.C.G. ALL RIGHTS RESERVED");
}

void GeneratePlatform(){ 
  TPlatform* platforms = nullptr;
  platforms = (TPlatform*)malloc(kplatform_numbers * sizeof(TPlatform));
  *(platforms + 0) = {70.0f, 150.0f, 6};
  *(platforms + 1) = {240.0f, 180.0f, 4};
  *(platforms + 2) = {390.0f, 100.0f, 6};

  float* bg = (float*)malloc(8 * sizeof(float));

  int w = esat::SpriteWidth(*(platform));
  int h = esat::SpriteHeight(*(platform));

  for (int i = 0; i < kplatform_numbers; ++i){
    TPlatform* p = platforms + i;

    *(bg + 0) = p->pos.x;
    *(bg + 1) = p->pos.y;
    *(bg + 2) = p->pos.x + (w * p->size);
    *(bg + 3) = p->pos.y;
    *(bg + 4) = p->pos.x + (w * p->size);
    *(bg + 5) = p->pos.y + h;
    *(bg + 6) = p->pos.x;
    *(bg + 7) = p->pos.y + h;

    esat::DrawSetStrokeColor(0, 0, 0, 0); // hide borders
    esat::DrawSetFillColor(0, 255, 0, 255);
    esat::DrawSolidPath(bg, 4);

    // Draw platforms
    for (int j = 0; j < p->size; ++j){
      if (j == 0){
        esat::DrawSprite(*(platform + 0), p->pos.x, p->pos.y);
      }else if (j == p->size - 1){
        esat::DrawSprite(*(platform + 2), p->pos.x + (j * w), p->pos.y);
      }else{
        esat::DrawSprite(*(platform + 1), p->pos.x + (j * w), p->pos.y);
      }
    }
  }

  free(platforms);
  free(bg);

}

void GenerateFloor(){
  float* floor = (float*)malloc(8 * sizeof(float));
  float h = esat::SpriteHeight(*(platform));

  *(floor + 0) = 0.0f;
  *(floor + 1) = (float)(KWindow_Height - (int)h);
  *(floor + 2) = (float)KWindow_Width;
  *(floor + 3) = (float)(KWindow_Height - (int)h);
  *(floor + 4) = (float)KWindow_Width;
  *(floor + 5) = (float)KWindow_Height;
  *(floor + 6) = 0.0f;
  *(floor + 7) = (float)KWindow_Height;

  esat::DrawSetStrokeColor(0, 0, 0, 0); // hide borders
  esat::DrawSetFillColor(208, 208, 0, 255);
  esat::DrawSolidPath(floor, 4);

  for (int j = 0; j < KWindow_Width/esat::SpriteWidth(*(platform)); ++j){
    esat::DrawSprite(*(platform + 0), j * esat::SpriteWidth(*(platform)), KWindow_Height - h);
  }

  free(floor);
}

// Basic game screen
void GameScreen(){
  DrawHeader();
  GeneratePlatform();
  GenerateFloor();
}

// function to select the screen to show
void ScreenSelector(TScreen current_screen){
  switch (current_screen){
    case MAIN_MENU:
      // printf("[DEBUG] Main menu selected\n");
      MainMenu();
      break;
    case GAME_SCREEN:
      GameScreen();
      break;
    default:
      printf("[DEBUG] not screen\n");
      break;
  }  
}

void TestMousePosition(){
  if(esat::MouseButtonDown(0))
    printf("[DEBUG] Mouse position: %f, %f\n", (float) esat::MousePositionX(), (float) esat::MousePositionY());
}


