byte hue;
unsigned long timerRainbow = 0;
void rainbow1() {
  if (timerRainbow < millis())
    timerRainbow = millis() + 50;
  else
    return;

  hue += 1;
  fillAll(CHSV(hue, 255, 255));
}
byte hue2;
void rainbow2() {
  if (timerRainbow < millis())
    timerRainbow = millis() + 25;
  else
    return;

  byte cook = hue2;
  for (byte j = 0; j < HEIGHT; j++) {
    for (byte i = 0; i < WIDTH; i++) {
      drawPixelXY(j, i, CHSV(cook, 255, 255));
    }
    cook += 5;
  }
  hue2++;
}