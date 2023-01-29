#define DELAY_STAINS_DRAW 5
#define STAINS_COUNT 10

struct Stain {
  byte ChueStainsColors = random(0, 255);
  byte CvalueStainsColors = random(0, 240);
  ;
  bool StainsTriger = random(0, 2);

  byte StainsX = random(0, 16);
  byte StainsY = random(0, 16);
  byte StainsSize = random(2, 7);
};

Stain Stains[STAINS_COUNT];

void stains() {
  for (int i = 0; i < STAINS_COUNT; i++) {
    if (Stains[i].StainsTriger) {
      Stains[i].CvalueStainsColors += 20;
      if (Stains[i].CvalueStainsColors >= 230)
        Stains[i].StainsTriger = false;
    } else {
      Stains[i].CvalueStainsColors -= 10;
      if (Stains[i].CvalueStainsColors <= 15) {
        Stains[i].CvalueStainsColors = 0;
        Stains[i].ChueStainsColors = random(0, 255);
        Stains[i].StainsTriger = true;
        Stains[i].StainsX = random(0, 16);
        Stains[i].StainsY = random(0, 16);
        Stains[i].StainsSize = random(2, 7);
      }
    }
  }

  clearAll();
  drawStains();
  delay(DELAY_STAINS_DRAW);
}

void drawStains() {
  for (int i = 0; i < STAINS_COUNT; i++) {
    for (int X = Stains[i].StainsX; X < Stains[i].StainsX + Stains[i].StainsSize; X++) {
      for (int Y = Stains[i].StainsY; Y < Stains[i].StainsY + Stains[i].StainsSize; Y++) {
        drawPixelXY(X, Y, CHSV(Stains[i].ChueStainsColors, 255, Stains[i].CvalueStainsColors));
      }
    }
  }
}