//---УТИЛИТЫ v2.3---

#define _WIDTH WIDTH 
#define THIS_X x
#define THIS_Y y
#define MATRIX_TYPE 0 
#define SEGMENTS 1 

#define TRACK_STEP 50

// служебные функции

// очистить все
void clearAll() {
  fillAll(CRGB::Black);
}

// залить все
void fillAll(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

// функция отрисовки точки по координатам X Y
void drawPixelXY(int8_t x, int8_t y, CRGB color) {
  if (x < 0 || x > WIDTH - 1 || y < 0 || y > HEIGHT - 1) return;
  int thisPixel = getPixelNumber(x, y) * SEGMENTS;
  for (byte i = 0; i < SEGMENTS; i++) {
    leds[thisPixel + i] = color;
  }
}

// получить номер пикселя в ленте по координатам
uint16_t getPixelNumber(int8_t x, int8_t y) {
  return __getPixelNumber(15-y, x);
}
uint16_t __getPixelNumber(int8_t x, int8_t y) {
  if ((THIS_Y % 2 == 0) || MATRIX_TYPE) {               // если чётная строка
    return (THIS_Y * _WIDTH + THIS_X);
  } else {                                              // если нечётная строка
    return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1);
  }
}

// функция получения цвета пикселя в матрице по его координатам
uint32_t getPixColorXY(int8_t x, int8_t y) {
  return getPixColor(getPixelNumber(x, y));
}

// функция получения цвета пикселя по его номеру
uint32_t getPixColor(int thisSegm) {
  int thisPixel = thisSegm * SEGMENTS;
  if (thisPixel < 0 || thisPixel > NUM_LEDS - 1) return 0;
  return (((uint32_t)leds[thisPixel].r << 16) | ((long)leds[thisPixel].g << 8 ) | (long)leds[thisPixel].b);
}

// получить случайный цвет
CRGB randomColor(){
  return CRGB(random(0,255),random(0,255),random(0,255));
}

// получить случайный цвет 2й вариант
CRGB randomColor2(){
  return CHSV(random(0,255),255,255);
}

// обновить яркость
void refreshBrightness(boolean theShow){
  FastLED.setBrightness(brightness);
  if (theShow)
    FastLED.show();
}
void refreshBrightness(){
  FastLED.setBrightness(brightness);
  FastLED.show();
}

// потухание
void fade() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((uint32_t)getPixColor(i) == 0) continue;
    leds[i].fadeToBlackBy(TRACK_STEP);
  }
}

// перезапуск таймера автоматической смены режимов
void resetTimerAutoSwitchingModes(){
  timerAutoSwitchingModes = millis() + (TIME_AUTO_MODE_CHANGEOVER * 1000);
}

// координатная позиция 2D
struct Vector2I{
  Vector2I();
  Vector2I(int __X, int __Y){
    X = __X;
    Y = __Y;
  }
  int X;
  int Y;
};

// бегущая строка
void ticker(String text, CRGB textColor, boolean clear){
  resetString();
  FastLED.setBrightness(BRIGHTNESS_WHEN_OUTPUT_TEXT);
  for (;;){
    if (fillString(text, textColor, clear))break;
    FastLED.show();
  }
}

extern int __bss_end;
extern void *__brkval;
// Функция, возвращающая количество свободного ОЗУ
int memoryFree() {
  int freeValue;
  if ((int)__brkval == 0)
    freeValue = ((int)&freeValue) - ((int)&__bss_end);
  else
    freeValue = ((int)&freeValue) - ((int)__brkval);
  return freeValue;
}

// rgb16 bit to rgb32 bit
unsigned long rgb16_to_rgb32(unsigned long a)
{
  unsigned long r = (a & 0xF800) >11;
  unsigned long g = (a & 0x07E0) >5;
  unsigned long b = (a & 0x001F);

  r = r * 255 / 31;
  g = g * 255 / 63;
  b = b * 255 / 31;
  
  return (r << 16) | (g << 8) | b;

  /* Or for BGR0:
  return (r << 8) | (g << 16) | (b << 24);
  */
}

// функция плавного угасания цвета для всех пикселей
void fader(byte step) {
  for (byte x = 0; x < WIDTH; x++)
    for (byte y = 0; y < HEIGHT; y++)
      fadePixel(x, y, step);
}
void fadePixel(byte i, byte j, byte step) {
  int pixelNum = getPixelNumber(i, j);
  if (getPixColor(pixelNum) == 0) return;

  if (leds[pixelNum].r >= 30 || leds[pixelNum].g >= 30 || leds[pixelNum].b >= 30)
    leds[pixelNum].fadeToBlackBy(step);
  else
    leds[pixelNum] = 0;
}

void editBrightness(){
  
}

void drawCircle(int x0, int y0, int radius, const CRGB &color){
  int a = radius, b = 0;
  int radiusError = 1 - a;

  if (radius == 0) {
    drawPixelXY(x0, y0, color);
    return;
  }

  while (a >= b)  {
    drawPixelXY(a + x0, b + y0, color);
    drawPixelXY(b + x0, a + y0, color);
    drawPixelXY(-a + x0, b + y0, color);
    drawPixelXY(-b + x0, a + y0, color);
    drawPixelXY(-a + x0, -b + y0, color);
    drawPixelXY(-b + x0, -a + y0, color);
    drawPixelXY(a + x0, -b + y0, color);
    drawPixelXY(b + x0, -a + y0, color);
    b++;
    if (radiusError < 0)
      radiusError += 2 * b + 1;
    else
    {
      a--;
      radiusError += 2 * (b - a + 1);
    }
  }
}

uint8_t wrapX(int8_t x){
  return (x + WIDTH)%WIDTH;
}
uint8_t wrapY(int8_t y){
  return (y + HEIGHT)%HEIGHT;
}