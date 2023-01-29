#define stormRoutine2_Scale 30
#define stormRoutine2_Speed 20

#define e_sns_DENSE (32U) // плотность снега

void stormRoutine2() {
	bool isColored = stormRoutine2_Speed & 0x01;

  // заполняем головами комет
  uint8_t Saturation = 0U;    // цвет хвостов
  uint8_t e_TAIL_STEP = 127U; // длина хвоста
  if (isColored)
    Saturation = stormRoutine2_Scale * 2.55;
  else
    e_TAIL_STEP = 255U - stormRoutine2_Scale * 2.5;

  for (uint8_t x = 0U; x < WIDTH - 1U; x++)
    if (!random8(e_sns_DENSE) && !getPixColorXY(wrapX(x), HEIGHT - 1U) && !getPixColorXY(wrapX(x + 1U), HEIGHT - 1U) && !getPixColorXY(wrapX(x - 1U), HEIGHT - 1U))
      drawPixelXY(x, HEIGHT - 1U, CHSV(random8(), Saturation, random8(64U, 255U)));

  // сдвигаем по диагонали
  for (uint8_t y = 0U; y < HEIGHT - 1U; y++)
    for (uint8_t x = 0; x < WIDTH; x++)
      drawPixelXY(wrapX(x + 1U), y, getPixColorXY(x, y + 1U));

  // уменьшаем яркость верхней линии, формируем "хвосты"
  for (uint8_t i = 0U; i < WIDTH; i++)
    fadePixel(i, HEIGHT - 1U, e_TAIL_STEP);
}