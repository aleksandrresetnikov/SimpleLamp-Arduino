#define snowRoutine_Scale 30

void snowRoutine()
{
  #if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings){
      setModeSettings(88U+random8(9U), 170U+random8(36U));
    }
  #endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
  
  // сдвигаем всё вниз
  for (uint8_t x = 0U; x < WIDTH; x++)
  {
    for (uint8_t y = 0U; y < HEIGHT - 1; y++)
    {
      drawPixelXY(x, y, getPixColorXY(x, y + 1U));
    }
  }

  for (uint8_t x = 0U; x < WIDTH; x++)
  {
    // заполняем случайно верхнюю строку
    // а также не даём двум блокам по вертикали вместе быть
    if (getPixColorXY(x, HEIGHT - 2U) == 0U && (random(0, 100 - snowRoutine_Scale) == 0U))
      drawPixelXY(x, HEIGHT - 1U, 0xE0FFFF - 0x101010 * random(0, 4));
    else
      drawPixelXY(x, HEIGHT - 1U, 0x000000);
  }
}