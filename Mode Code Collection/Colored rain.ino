#define RAIN_COUNT 14
#define DELAY_RAIN_DRAW 15
struct colorfulRainElement {
  int x = random(0, WIDTH);
  int y = random(0, HEIGHT);
  byte hue = random(0, 255);
  byte corectSpeedY = random(1, 4);
};
colorfulRainElement rainElement[RAIN_COUNT];
void colorfulRain() {
  for (int i = 0; i < RAIN_COUNT; i++) {
    rainElement[i].y -= rainElement[i].corectSpeedY;
    if (rainElement[i].y <= -8) {
      rainElement[i].y = HEIGHT;
      rainElement[i].x = random(0, WIDTH);
      rainElement[i].hue = random(0, 255);
      rainElement[i].corectSpeedY = random(1, 4);
    }
  }
  clearAll();
  for (int i = 0; i < RAIN_COUNT; i++) {
    for (int j = 0; j < 8; j++) {
      drawPixelXY(rainElement[i].x, rainElement[i].y + j, CHSV(rainElement[i].hue, 255, 255));
    }
  }
  delay(DELAY_RAIN_DRAW);
}