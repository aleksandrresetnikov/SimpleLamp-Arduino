unsigned long timerKrin1 = 0;
byte colorCorectKrin = 0;
void krin() {
  if (timerKrin1 < millis()) {
    byte corect = 0;
    while (corect < 16) {
      for (byte x = corect; x < WIDTH; x++) {
        drawPixelXY(x, corect, CHSV(colorCorectKrin, 255, 255));
        colorCorectKrin++;
      }
      for (byte y = corect; y < HEIGHT; y++) {
        drawPixelXY(corect, y, CHSV(colorCorectKrin, 255, 255));
        colorCorectKrin++;
      }
      corect++;
    }
    timerKrin1 = millis() + 20;
  }
}