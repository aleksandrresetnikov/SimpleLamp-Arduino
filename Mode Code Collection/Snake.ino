byte motion = 0;
void snake() {
  if (leds[motion] != CHSV(motion, 255, 255)) {
    leds[motion] = CHSV(motion, 255, 255);
    FastLED.show();
  } else {
    leds[motion] = CRGB::Black;
    FastLED.show();
  }
  delay(5);

  motion++;
}