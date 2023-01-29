#define MAX_SPEED 30
#define BUGS_AMOUNT 3
int16_t speed[BUGS_AMOUNT];
int16_t pos[BUGS_AMOUNT];
CRGB bugColors[BUGS_AMOUNT];

void lightBugs() {
  if (true) {
    loadingFlag = false;
    for (int i = 0; i < BUGS_AMOUNT; i++) {
      bugColors[i] = CHSV(random(0, 9) * 28, 255, 255);
      pos[i] = random(0, NUM_LEDS);
      speed[i] += random(-5, 6);
    }
  }
  FastLED.clear();
  for (int i = 0; i < BUGS_AMOUNT; i++) {
    speed[i] += random(-5, 6);
    if (speed[i] == 0) speed[i] += (-5, 6);

    if (abs(speed[i]) > MAX_SPEED) speed[i] = 0;
    pos[i] += speed[i] / 10;
    if (pos[i] < 0) {
      pos[i] = 0;
      speed[i] = -speed[i];
    }
    if (pos[i] > NUM_LEDS - 1) {
      pos[i] = NUM_LEDS - 1;
      speed[i] = -speed[i];
    }
    leds[pos[i]] = bugColors[i];
  }
}