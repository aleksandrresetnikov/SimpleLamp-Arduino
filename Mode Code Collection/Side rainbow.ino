#define rainbowRoutine_Scale 40

void rainbowHorVertRoutine(bool isVertical) {
  for (uint8_t i = 0U; i < (isVertical?WIDTH:HEIGHT); i++) {
    //CRGB thisColor;
    //hsv2rgb_spectrum(CHSV(__hue + i * (rainbowRoutine_Scale % 67U) * 2U, 255U, 255U), thisColor); // так ещё хуже стало на низкой яркости
    CHSV thisColor = CHSV((uint8_t)(__hue + i * (rainbowRoutine_Scale % 67U) * 2U), 255U, 255U);

    for (uint8_t j = 0U; j < (isVertical?HEIGHT:WIDTH); j++)
      drawPixelXY((isVertical?i:j), (isVertical?j:i), thisColor);
  }
}
void rainbowRoutine() {
  #if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings){
      uint8_t tmp = 7U+random8(50U);
      if (tmp>14) tmp += 19U;
      if (tmp>67) tmp += 6U;
      setModeSettings(tmp , 150U+random8(86U));
    }
  #endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)

  __hue += 4U;
  if (rainbowRoutine_Scale < 34U)           // если масштаб до 34
    rainbowHorVertRoutine(false);
  else if (rainbowRoutine_Scale > 67U)      // если масштаб больше 67
    rainbowHorVertRoutine(true);
  else                                          // для масштабов посередине
    for (uint8_t i = 0U; i < WIDTH; i++)
      for (uint8_t j = 0U; j < HEIGHT; j++)
      {
        float twirlFactor = 9.0F * ((rainbowRoutine_Scale-33) / 100.0F);      // на сколько оборотов будет закручена матрица, [0..3]
        CRGB thisColor = CHSV((uint8_t)(__hue + ((float)WIDTH / (float)HEIGHT * i + j * twirlFactor) * ((float)255 / (float)maxDim)), 255U, 255U);
        drawPixelXY(i, j, thisColor);
      }
}