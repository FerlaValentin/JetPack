

// TODO: pass data ??
void DrawHeader(){
  esat::DrawSetFillColor(255, 255, 255);
  esat::DrawSetTextSize(14);

  esat::DrawText(10, 16, "1UP");
  esat::DrawText(10, 32, "000000"); // score 1up placeholder

  esat::DrawText(240, 16, "HI"); 
  esat::DrawText(220, 32, "000000"); // Hi-Score placeholder
  
  esat::DrawText(470, 16, "2UP");
  esat::DrawText(436, 32, "000000"); // score 2up placeholder
}