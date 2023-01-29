void brightLight() {
  fillAll(CRGB::White);
  FastLED.setBrightness(255);
  FastLED.show();
  delay(1000);
  for (;;) {
    if (!digitalRead(BUTTON_POVER) || !digitalRead(BUTTON_NEXT_EFFECT)) break;
  }
  refreshBrightness();
  clearAll();
  FastLED.show();
}