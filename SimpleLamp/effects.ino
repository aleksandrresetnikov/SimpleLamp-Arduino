#define NUM_LAYERSMAX 2
uint8_t noise3d[NUM_LAYERSMAX][WIDTH][HEIGHT];     // двухслойная маска или хранилище свойств в размер всей матрицы
uint8_t __hue, __hue2;

// ----------------------------- радуга от гайвера ----------------------------------
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
// ---------------------------------- яркий --------------------------------------
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

// --------------------------------- конфетти ------------------------------------
void sparklesRoutine() {
  delay(5);
  for (byte i = 0; i < modes[0].scale; i++) {
    byte x = random(0, WIDTH);
    byte y = random(0, HEIGHT);
    if (getPixColorXY(x, y) == 0)
      leds[getPixelNumber(x, y)] = CHSV(random(0, 255), 255, 255);
  }
  fader(70);
}

// ------------------------------------ змея --------------------------------------
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

// ----------------------------------- грибы --------------------------------------
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

// ----------------------------------- радуга -------------------------------------
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
// ----------------------------------- мячик --------------------------------------
#define BALLS_COUNT 3

struct ballMesh {
  bool kirXball = true;
  bool kirYball = true;

  byte xBall = WIDTH / 2;
  byte yBall = HEIGHT / 2;

  byte corectXball = 2;
  byte corectYball = 1;

  byte colorBall = 0;
};

ballMesh balls[BALLS_COUNT];

void ball() {
  fader(70);
  //clearAll();

  for (byte i = 0; i < BALLS_COUNT; i++) {
    if (balls[i].kirXball) {
      balls[i].xBall += balls[i].corectXball;
      if (balls[i].xBall >= WIDTH) {
        balls[i].kirXball = false;
        balls[i].colorBall += 5;
      }
    } else {
      balls[i].xBall -= balls[i].corectXball;
      if (balls[i].xBall <= 0) {
        balls[i].kirXball = true;
        balls[i].colorBall += 5;
      }
    }

    if (balls[i].kirYball) {
      balls[i].yBall += balls[i].corectYball;
      if (balls[i].yBall >= HEIGHT) {
        balls[i].kirYball = false;
        balls[i].colorBall += 5;
      }
    } else {
      balls[i].yBall -= balls[i].corectYball;
      if (balls[i].yBall <= 0) {
        balls[i].kirYball = true;
        balls[i].colorBall += 5;
      }
    }
  }

  ballDraw();
  delay(25);
}

void ballDraw() {
  for (byte i = 0; i < BALLS_COUNT; i++) {
    /*drawPixelXY(balls[i].xBall + 1, balls[i].yBall, CHSV(balls[i].colorBall, 255, 255));
    drawPixelXY(balls[i].xBall, balls[i].yBall + 1, CHSV(balls[i].colorBall, 255, 255));
    drawPixelXY(balls[i].xBall, balls[i].yBall, CHSV(balls[i].colorBall, 255, 255));
    drawPixelXY(balls[i].xBall + 1, balls[i].yBall + 1, CHSV(balls[i].colorBall, 255, 255));*/

    drawPixelXY(balls[i].xBall, balls[i].yBall, CHSV(balls[i].colorBall, 255, 255));

    drawPixelXY(balls[i].xBall + 1, balls[i].yBall, CHSV(balls[i].colorBall, 255, 255));
    drawPixelXY(balls[i].xBall, balls[i].yBall + 1, CHSV(balls[i].colorBall, 255, 255));
    drawPixelXY(balls[i].xBall + 1, balls[i].yBall + 1, CHSV(balls[i].colorBall, 255, 95));//
    drawPixelXY(balls[i].xBall + 1, balls[i].yBall - 1, CHSV(balls[i].colorBall, 255, 95));//
    
    drawPixelXY(balls[i].xBall - 1, balls[i].yBall, CHSV(balls[i].colorBall, 255, 255));
    drawPixelXY(balls[i].xBall, balls[i].yBall - 1, CHSV(balls[i].colorBall, 255, 255));
    drawPixelXY(balls[i].xBall - 1, balls[i].yBall - 1, CHSV(balls[i].colorBall, 255, 95));//
    drawPixelXY(balls[i].xBall - 1, balls[i].yBall + 1, CHSV(balls[i].colorBall, 255, 95));//
  }
}

// -------------------------------------- огонь ---------------------------------------------
//these values are substracetd from the generated values to give a shape to the animation
const unsigned char valueMask[8][16] PROGMEM = {
  { 32, 0, 0, 0, 0, 0, 0, 32, 32, 0, 0, 0, 0, 0, 0, 32 },
  { 64, 0, 0, 0, 0, 0, 0, 64, 64, 0, 0, 0, 0, 0, 0, 64 },
  { 96, 32, 0, 0, 0, 0, 32, 96, 96, 32, 0, 0, 0, 0, 32, 96 },
  { 128, 64, 32, 0, 0, 32, 64, 128, 128, 64, 32, 0, 0, 32, 64, 128 },
  { 160, 96, 64, 32, 32, 64, 96, 160, 160, 96, 64, 32, 32, 64, 96, 160 },
  { 192, 128, 96, 64, 64, 96, 128, 192, 192, 128, 96, 64, 64, 96, 128, 192 },
  { 255, 160, 128, 96, 96, 128, 160, 255, 255, 160, 128, 96, 96, 128, 160, 255 },
  { 255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255 }
};

//these are the hues for the fire,
//should be between 0 (red) to about 25 (yellow)
const unsigned char hueMask[8][16] PROGMEM = {
  { 1, 11, 19, 25, 25, 22, 11, 1, 1, 11, 19, 25, 25, 22, 11, 1 },
  { 1, 8, 13, 19, 25, 19, 8, 1, 1, 8, 13, 19, 25, 19, 8, 1 },
  { 1, 8, 13, 16, 19, 16, 8, 1, 1, 8, 13, 16, 19, 16, 8, 1 },
  { 1, 5, 11, 13, 13, 13, 5, 1, 1, 5, 11, 13, 13, 13, 5, 1 },
  { 1, 5, 11, 11, 11, 11, 5, 1, 1, 5, 11, 11, 11, 11, 5, 1 },
  { 0, 1, 5, 8, 8, 5, 1, 0, 0, 1, 5, 8, 8, 5, 1, 0 },
  { 0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 }
};

void fireRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    //FastLED.clear();
    generateLine();
  }
  if (pcnt >= 100) {
    shiftUp();
    generateLine();
    pcnt = 0;
  }
  drawFrame(pcnt);
  pcnt += 30;
}

// Randomly generate the next line (matrix row)

void generateLine() {
  for (uint8_t x = 0; x < WIDTH; x++) {
    line[x] = random(64, 255);
  }
}

void shiftUp() {
  for (uint8_t y = HEIGHT - 1; y > 0; y--) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x - 15;
      if (y > 7) continue;
      matrixValue[y][newX] = matrixValue[y - 1][newX];
    }
  }

  for (uint8_t x = 0; x < WIDTH; x++) {
    uint8_t newX = x;
    if (x > 15) newX = x - 15;
    matrixValue[0][newX] = line[newX];
  }
}

// draw a frame, interpolating between 2 "key frames"
// @param pcnt percentage of interpolation

void drawFrame(int pcnt) {
  int nextv;

  //each row interpolates with the one before it
  for (unsigned char y = HEIGHT - 1; y > 0; y--) {
    for (unsigned char x = 0; x < WIDTH; x++) {
      uint8_t newX = x;
      if (x > 15) newX = x - 15;
      if (y < 8) {
        nextv =
          (((100.0 - pcnt) * matrixValue[y][newX]
            + pcnt * matrixValue[y - 1][newX])
           / 100.0)
          - pgm_read_byte(&(valueMask[y][newX]));

        CRGB color = CHSV(
          modes[1].scale * 2.5 + pgm_read_byte(&(hueMask[y][newX])),  // H
          255,                                                        // S
          (uint8_t)max(0, nextv)                                      // V
        );

        leds[getPixelNumber(x, y)] = color;
      } else if (y == 8 && SPARKLES) {
        if (random(0, 20) == 0 && getPixColorXY(x, y - 1) != 0) drawPixelXY(x, y, getPixColorXY(x, y - 1));
        else drawPixelXY(x, y, 0);
      } else if (SPARKLES) {

        // старая версия для яркости
        if (getPixColorXY(x, y - 1) > 0)
          drawPixelXY(x, y, getPixColorXY(x, y - 1));
        else drawPixelXY(x, y, 0);
      }
    }
  }

  //first row interpolates with the "next" line
  for (unsigned char x = 0; x < WIDTH; x++) {
    uint8_t newX = x;
    if (x > 15) newX = x - 15;
    CRGB color = CHSV(//2.5
      modes[1].scale * 2.5 + pgm_read_byte(&(hueMask[0][newX])),                      // H
      255,                                                                            // S
      (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0)  // V
    );
    leds[getPixelNumber(newX, 0)] = color;
  }
}

// ---------------------------- расброс цветов -----------------------------------
byte ChueScatterColors = 0;
byte CvalueScatterColors = 0;
bool triger = false;
bool showTriger = true;
void scatterColors() {
  if (showTriger) {
    if (!triger) {
      ChueScatterColors = random(0, 255);
      triger = true;
    } else {
      CvalueScatterColors += 10;
      if (CvalueScatterColors >= 240) {
        showTriger = false;
        triger = false;
      }
    }
  } else
    smoothExtinction();

  if (CvalueScatterColors > 3)
    fillAll(CHSV(ChueScatterColors, 255, CvalueScatterColors));
}

void smoothExtinction() {
  CvalueScatterColors -= 2;
  if (CvalueScatterColors <= 3)
    showTriger = true;
}

// -------------------------------- "пули" ---------------------------------------
#define BULLET_COUNT 5

struct bulletSprite {
  byte movingType = random(1, 9);
  byte bulletHue = random(0, 255);
  char bulletX = random(0, WIDTH);
  char bulletY = random(0, HEIGHT);
};

bulletSprite bulletSprits[BULLET_COUNT];

void bullets() {
  for (int i = 0; i < BULLET_COUNT; i++) {

    switch (bulletSprits[i].movingType) {
      case 1:
        bulletSprits[i].bulletX++;
        if (bulletSprits[i].bulletX >= WIDTH) {
          bulletSprits[i].movingType++;
          bulletSprits[i].bulletX = random(0, WIDTH);
          bulletSprits[i].bulletY = 0;
        }
        break;
      case 2:
        bulletSprits[i].bulletY++;
        if (bulletSprits[i].bulletY >= HEIGHT) {
          bulletSprits[i].movingType++;
          bulletSprits[i].bulletY = random(0, HEIGHT);
          bulletSprits[i].bulletX = WIDTH;
        }
        break;
      case 3:
        bulletSprits[i].bulletX--;
        if (bulletSprits[i].bulletX <= 0) {
          bulletSprits[i].movingType++;
          bulletSprits[i].bulletX = random(0, WIDTH);
          bulletSprits[i].bulletY = HEIGHT;
        }
        break;
      case 4:
        bulletSprits[i].bulletY--;
        if (bulletSprits[i].bulletY <= 0) {
          bulletSprits[i].movingType++;
          bulletSprits[i].bulletX = 0;
          bulletSprits[i].bulletY = 0;
        }
      case 5:
        bulletSprits[i].bulletY++;
        bulletSprits[i].bulletX++;
        if (bulletSprits[i].bulletY >= HEIGHT || bulletSprits[i].bulletX >= WIDTH) {
          bulletSprits[i].movingType++;
          bulletSprits[i].bulletX = WIDTH;
          bulletSprits[i].bulletY = 0;
        }
        break;
      case 6:
        bulletSprits[i].bulletY++;
        bulletSprits[i].bulletX--;
        if (bulletSprits[i].bulletY >= HEIGHT || bulletSprits[i].bulletX <= 0) {
          bulletSprits[i].movingType++;
          bulletSprits[i].bulletX = WIDTH;
          bulletSprits[i].bulletY = HEIGHT;
        }
        break;
      case 7:
        bulletSprits[i].bulletY--;
        bulletSprits[i].bulletX--;
        if (bulletSprits[i].bulletY <= 0 || bulletSprits[i].bulletX <= 0) {
          bulletSprits[i].movingType++;
          bulletSprits[i].bulletX = 0;
          bulletSprits[i].bulletY = HEIGHT;
        }
      case 8:
        bulletSprits[i].bulletY--;
        bulletSprits[i].bulletX++;
        if (bulletSprits[i].bulletY <= 0 || bulletSprits[i].bulletX >= WIDTH) {
          bulletSprits[i].movingType++;
          bulletSprits[i].bulletX = random(0, WIDTH);
          bulletSprits[i].bulletY = random(0, HEIGHT);
        }
        break;
      default:
        bulletSprits[i].movingType = 1;
        break;
    }
  }
  clearAll();
  //drawPixelXY(bulletX,bulletY,CHSV(bulletHue,255,255));

  for (int i = 0; i < BULLET_COUNT; i++) {
    drawPixelXY(bulletSprits[i].bulletX, bulletSprits[i].bulletY, CRGB::White);
  }
}

// ---------------------------- красочный дождь ----------------------------------
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

// --------------------------------- пятна ---------------------------------------
#define DELAY_STAINS_DRAW 5
#define STAINS_COUNT 10

struct Stain {
  byte ChueStainsColors = random(0, 255);
  byte CvalueStainsColors = random(0, 240);
  ;
  bool StainsTriger = random(0, 2);

  byte StainsX = random(0, 16);
  byte StainsY = random(0, 16);
  byte StainsSize = random(2, 7);
};

Stain Stains[STAINS_COUNT];

void stains() {
  for (int i = 0; i < STAINS_COUNT; i++) {
    if (Stains[i].StainsTriger) {
      Stains[i].CvalueStainsColors += 20;
      if (Stains[i].CvalueStainsColors >= 230)
        Stains[i].StainsTriger = false;
    } else {
      Stains[i].CvalueStainsColors -= 10;
      if (Stains[i].CvalueStainsColors <= 15) {
        Stains[i].CvalueStainsColors = 0;
        Stains[i].ChueStainsColors = random(0, 255);
        Stains[i].StainsTriger = true;
        Stains[i].StainsX = random(0, 16);
        Stains[i].StainsY = random(0, 16);
        Stains[i].StainsSize = random(2, 7);
      }
    }
  }

  clearAll();
  drawStains();
  delay(DELAY_STAINS_DRAW);
}

void drawStains() {
  for (int i = 0; i < STAINS_COUNT; i++) {
    for (int X = Stains[i].StainsX; X < Stains[i].StainsX + Stains[i].StainsSize; X++) {
      for (int Y = Stains[i].StainsY; Y < Stains[i].StainsY + Stains[i].StainsSize; Y++) {
        drawPixelXY(X, Y, CHSV(Stains[i].ChueStainsColors, 255, Stains[i].CvalueStainsColors));
      }
    }
  }
}

// --------------------------------- линия  --------------------------------------
int16_t position;
boolean direction;

void lighter() {
  FastLED.clear();
  if (direction) {
    position++;
    if (position > NUM_LEDS - 2) {
      direction = false;
    }
  } else {
    position--;
    if (position < 1) {
      direction = true;
    }
  }
  leds[position] = CRGB::White;
}

// -------------------------------- светлячки  -----------------------------------
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

// ---------------------------------- огонь 2 ------------------------------------
#define COOLING 55
// ИСКРА: Каков шанс (из 255), что загорится новая искра?
// Более высокий шанс = больше ревущего огня. Меньший шанс = больше мерцающего огня.
// По умолчанию 120, рекомендуемый диапазон 50-200.
#define SPARKING 120

void fire() {
  //random16_add_entropy( random());
  Fire2012WithPalette();  // run simulation frame, using palette colors
}

void Fire2012WithPalette() {
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < SPARKING) {
    int y = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }

  // Step 4.  Map from heat cells to LED colors
  for (int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8(heat[j], 240);
    CRGB color = ColorFromPalette(gPal, colorindex);
    int pixelnumber;
    if (gReverseDirection) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}

// ---------------------------------- плазма 1 ------------------------------------
void plasma() {
  FastLED.clear();
  uint16_t ms = millis();
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      uint8_t index = inoise8(i * 41, j * 41, ms / 4);
      leds[XY(i, j)] = CHSV((index + ms / 200) % 255, 255, brightness);
    }
  }
  FastLED.delay(1);
}

// ---------------------------------- плазма 2 ------------------------------------
void plasma2() {
  FastLED.clear();
  uint16_t ms = millis();
  for (int i = 0; i < WIDTH; i++) {
    for (int j = 0; j < HEIGHT; j++) {
      uint8_t index = inoise8(i * 41, j * 41, ms / 4);
      leds[XY(i, j)] = ColorFromPalette(RainbowColors_p, (index + ms / 200) % 255, brightness);
    }
  }
  FastLED.delay(1);
}
uint8_t XY(uint8_t x, uint8_t y) {
  return (y * WIDTH + x);
}

// ---------------------------------- плазма 3 ------------------------------------
uint32_t plasma3x[1];
uint32_t plasma3y[1];
uint32_t plasma3z[1];
uint32_t plasma3scale_x[1];
uint32_t plasma3scale_y[1];
uint8_t plasma3noise[1][16][16];

DEFINE_GRADIENT_PALETTE(pit){
  0, 3, 3, 3,
  64, 13, 13, 255,  //blue
  128, 3, 3, 3,
  192, 255, 130, 3,  //orange
  255, 3, 3, 3
};
void plasma3() {
  CRGBPalette16 Pal(pit);
  plasma3x[0] = plasma3x[0] + (2 * plasma3noise[0][0][0]) - 255;
  plasma3y[0] = plasma3y[0] + (2 * plasma3noise[0][15][0]) - 255;
  plasma3z[0] += 1 + ((plasma3noise[0][0][15]) / 4);

  plasma3scale_x[0] = 8000 + (plasma3noise[0][0][7] * 16);
  plasma3scale_y[0] = 8000 + (plasma3noise[0][15][7] * 16);

  //calculate new noise data
  uint8_t layer = 0;
  for (uint8_t i = 0; i < WIDTH; i++) {
    uint32_t ioffset = plasma3scale_x[layer] * (i - CentreX);
    for (uint8_t j = 0; j < HEIGHT; j++) {
      uint32_t joffset = plasma3scale_y[layer] * (j - CentreY);
      uint16_t data = inoise16(plasma3x[layer] + ioffset, plasma3y[layer] + joffset, plasma3z[layer]);
      if (data < 11000) data = 11000;
      if (data > 51000) data = 51000;
      data = data - 11000;
      data = data / 161;
      plasma3noise[layer][i][j] = data;
    }
  }

  //map the colors
  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      //I will add this overlay CRGB later for more colors
      //it´s basically a rainbow mapping with an inverted brightness mask
      CRGB overlay = CHSV(plasma3noise[0][y][x], 255, plasma3noise[0][x][y]);
      //here the actual colormapping happens - notice the additional colorshift caused by the down right pixel noise[0][15][15]
      leds[MetaBallsXY(x, y)] = ColorFromPalette(Pal, plasma3noise[0][15][15] + plasma3noise[0][x][y]) + overlay;
    }
  }

  //make it looking nice
  plasma3adjust_gamma();
  //and show it
  FastLED.show();
}

// cheap correction with gamma 2.0
void plasma3adjust_gamma() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].r = dim8_video(leds[i].r);
    leds[i].g = dim8_video(leds[i].g);
    leds[i].b = dim8_video(leds[i].b);
  }
}

// ---------------------------------- плазменный шар 3 ------------------------------------
// this finds the right index within a serpentine matrix
uint16_t MetaBallsXY(uint8_t x, uint8_t y) {
  uint16_t i;
  if (y & 0x01) {
    uint8_t reverseX = (WIDTH - 1) - x;
    i = (y * WIDTH) + reverseX;
  } else {
    i = (y * WIDTH) + x;
  }
  return i;
}

void MetaBalls() {
  float speed = 1;

  // get some 2 random moving points
  uint8_t x2 = inoise8(millis() * speed, 25355, 685) / 16;
  uint8_t y2 = inoise8(millis() * speed, 355, 11685) / 16;

  uint8_t x3 = inoise8(millis() * speed, 55355, 6685) / 16;
  uint8_t y3 = inoise8(millis() * speed, 25355, 22685) / 16;

  // and one Lissajou function
  uint8_t x1 = beatsin8(23 * speed, 0, 15);
  uint8_t y1 = beatsin8(28 * speed, 0, 15);

  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      uint8_t dx = abs(x - x1);
      uint8_t dy = abs(y - y1);
      uint8_t dist = 2 * sqrt((dx * dx) + (dy * dy));

      dx = abs(x - x2);
      dy = abs(y - y2);
      dist += sqrt((dx * dx) + (dy * dy));

      dx = abs(x - x3);
      dy = abs(y - y3);
      dist += sqrt((dx * dx) + (dy * dy));

      byte color = 1000 / dist;

      if (color > 0 and color < 60) {
        leds[MetaBallsXY(x, y)] = CHSV(color * 9, 255, 255);
      } else {
        leds[MetaBallsXY(x, y)] = CHSV(0, 255, 255);
      }
      // show the 3 points, too
      leds[XY(x1, y1)] = CRGB(255, 255, 255);
      leds[XY(x2, y2)] = CRGB(255, 255, 255);
      leds[XY(x3, y3)] = CRGB(255, 255, 255);
    }
  }
  FastLED.show();
}

// ---------------------------------- цветные линии ------------------------------------
uint8_t colorLinesx[NUM_LEDS];
uint8_t colorLinesy[NUM_LEDS];
int colorLinesStep = 0;
int colorLinesStepRuner = 0;
void colorLines() {
  switch (colorLinesStep) {
    case 0:
      startColorLines();
      colorLinesStep = 1;
      break;
    case 1: colorLinesexample_1(); break;
    case 2: colorLinesexample_2(); break;
    case 3: colorLinesexample_3(); break;
    default: colorLinesStep = 1; break;
  }
  colorLinesStepRuner++;
  if (colorLinesStepRuner >= 5000) {
    colorLinesStepRuner = 0;
    colorLinesStep++;
  }
}
void startColorLines() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {  // precalculate the lookup-tables:
    int angle = (i * 256) / NUM_LEDS;        // on which position on the circle is the led?
    colorLinesx[i] = cos8(angle);            // corrsponding x position in the matrix
    colorLinesy[i] = sin8(angle);            // corrsponding y position in the matrix
  }
}

// moves a noise up and down while slowly shifting to the side
void colorLinesexample_1() {

  uint8_t scale = 1000;  // the "zoom factor" for the noise

  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    uint16_t shift_x = beatsin8(17);    // the x position of the noise field swings @ 17 bpm
    uint16_t shift_y = millis() / 100;  // the y position becomes slowly incremented

    uint32_t real_x = (colorLinesx[i] + shift_x) * scale;  // calculate the coordinates within the noise field
    uint32_t real_y = (colorLinesy[i] + shift_y) * scale;  // based on the precalculated positions

    uint8_t noise = inoise16(real_x, real_y, 4223) >> 8;  // get the noise data and scale it down

    uint8_t index = noise * 3;  // map led color based on noise data
    uint8_t bri = noise;

    CRGB color = CHSV(index, 255, bri);
    leds[i] = color;
  }
}

// just moving along one axis = "lavalamp effect"
void colorLinesexample_2() {

  uint8_t scale = 1000;  // the "zoom factor" for the noise

  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    uint16_t shift_x = millis() / 10;  // x as a function of time
    uint16_t shift_y = 0;

    uint32_t real_x = (colorLinesx[i] + shift_x) * scale;  // calculate the coordinates within the noise field
    uint32_t real_y = (colorLinesy[i] + shift_y) * scale;  // based on the precalculated positions

    uint8_t noise = inoise16(real_x, real_y, 4223) >> 8;  // get the noise data and scale it down

    uint8_t index = noise * 3;  // map led color based on noise data
    uint8_t bri = noise;

    CRGB color = CHSV(index, 255, bri);
    leds[i] = color;
  }
}

// no x/y shifting but scrolling along z
void colorLinesexample_3() {

  uint8_t scale = 1000;  // the "zoom factor" for the noise

  for (uint16_t i = 0; i < NUM_LEDS; i++) {

    uint16_t shift_x = 0;  // no movement along x and y
    uint16_t shift_y = 0;


    uint32_t real_x = (colorLinesx[i] + shift_x) * scale;  // calculate the coordinates within the noise field
    uint32_t real_y = (colorLinesy[i] + shift_y) * scale;  // based on the precalculated positions

    uint32_t real_z = millis() * 20;  // increment z linear

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;  // get the noise data and scale it down

    uint8_t index = noise * 3;  // map led color based on noise data
    uint8_t bri = noise;

    CRGB color = CHSV(index, 255, bri);
    leds[i] = color;
  }
}


// =============- новый огонь / водопад -===============
#define fire2012WithPalette_Scale 10

// палитра для типа реалистичного водопада (если ползунок Масштаб выставить на 100)
extern const TProgmemRGBPalette16 WaterfallColors_p FL_PROGMEM = {0x000000, 0x060707, 0x101110, 0x151717, 0x1C1D22, 0x242A28, 0x363B3A, 0x313634, 0x505552, 0x6B6C70, 0x98A4A1, 0xC1C2C1, 0xCACECF, 0xCDDEDD, 0xDEDFE0, 0xB2BAB9};

// добавлено изменение текущей палитры (используется во многих эффектах ниже для бегунка Масштаб)
const TProgmemRGBPalette16 *palette_arr[] = {
    &PartyColors_p,
    &OceanColors_p, 
    &LavaColors_p, 
    &HeatColors_p, 
    &WaterfallColors_p, 
    &CloudColors_p, 
    &ForestColors_p, 
    &RainbowColors_p, 
    &RainbowStripeColors_p};
const TProgmemRGBPalette16 *curPalette = palette_arr[0];

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100
#define COOLINGNEW 32
// 8  практически сплошной поток красивой подсвеченной воды ровным потоком сверху донизу. будто бы на столе стоит маленький "родничок"
// 20 ровный водопад с верщиной на свету, где потоки летящей воды наверху разбиваются ветром в белую пену
// 32 уже не ровный водопад, у которого струи воды долетают до земли неравномерно
// чем больше параметр, тем больше тени снизу
// 55 такое, как на видео

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKINGNEW 80 // 30 // 120 // 90 // 60

void fire2012WithPalette() {
  #if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings){
      setModeSettings(random8(7U) ? 46U+random8(26U) : 100U, 195U+random8(40U));
    }
  #endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)

      bool fire_water = fire2012WithPalette_Scale <= 50;
      //uint8_t COOLINGNEW = fire_water ? fire2012WithPalette_Scale * 2  + 20 : (100 - fire2012WithPalette_Scale ) *  2 + 20 ;
      //uint8_t COOLINGNEW = fire2012WithPalette_Scale * 2  + 20 ;
  // Array of temperature readings at each simulation cell
  //static byte heat[WIDTH][HEIGHT]; будет noise3d[0][WIDTH][HEIGHT]

  for (uint8_t x = 0; x < WIDTH; x++) {
    // Step 1.  Cool down every cell a little
    for (uint8_t i = 0; i < HEIGHT; i++) {
      noise3d[0][x][i] = qsub8(noise3d[0][x][i], random8(0, ((COOLINGNEW * 10) / HEIGHT) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (uint8_t k = HEIGHT - 1; k >= 2; k--) {
      noise3d[0][x][k] = (noise3d[0][x][k - 1] + noise3d[0][x][k - 2] + noise3d[0][x][k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKINGNEW) {
      uint8_t y = random8(2);
      noise3d[0][x][y] = qadd8(noise3d[0][x][y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (uint8_t j = 0; j < HEIGHT; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8(noise3d[0][x][j], 240);
      if (fire2012WithPalette_Scale == 100)
        leds[XY(x, (HEIGHT - 1) - j)] = ColorFromPalette(WaterfallColors_p, colorindex);
      else
        leds[XY(x, (HEIGHT - 1) - j)] = ColorFromPalette(CRGBPalette16( CRGB::Black, CHSV(fire2012WithPalette_Scale * 2.57, 255U, 255U) , CHSV(fire2012WithPalette_Scale * 2.57, 128U, 255U) , CRGB::White), colorindex);// 2.57 вместо 2.55, потому что 100 для белого цвета
      //leds[XY(x, (HEIGHT - 1) - j)] = ColorFromPalette(fire_water ? HeatColors_p : OceanColors_p, colorindex);
    }
  }
}


// ------------- снегопад ----------
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


// ------------- метель - 2 -------------
#define stormRoutine2_Scale 30
#define stormRoutine2_Speed 20

//SNOWSTORM / МЕТЕЛЬ # STARFALL / ЗВЕЗДОПАД ***** V1.2
// v1.0 - Updating for GuverLamp v1.7 by PalPalych 12.03.2020
// v1.1 - Fix wrong math & full screen drawing by PalPalych 14.03.2020
// v1.2 - Code optimisation + pseudo 3d by PalPalych 21.04.2020
#define e_sns_DENSE (32U) // плотность снега - меньше = плотнее

void stormRoutine2()// (bool isColored) // сворачиваем 2 эффекта в 1
{
  #if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
    if (selectedSettings){
      uint8_t tmp = 175U+random8(39U);
      if (tmp & 0x01)
        setModeSettings(50U+random8(51U), tmp);
      else
        setModeSettings(50U+random8(24U), tmp);
    }
  #endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)

bool isColored = stormRoutine2_Speed & 0x01; // сворачиваем 2 эффекта в 1
  // заполняем головами комет
  uint8_t Saturation = 0U;    // цвет хвостов
  uint8_t e_TAIL_STEP = 127U; // длина хвоста
  if (isColored)
    Saturation = stormRoutine2_Scale * 2.55;
  else
  {
    e_TAIL_STEP = 255U - stormRoutine2_Scale * 2.5;
  }
  for (uint8_t x = 0U; x < WIDTH - 1U; x++) // fix error i != 0U
  {
    if (!random8(e_sns_DENSE) &&
        !getPixColorXY(wrapX(x), HEIGHT - 1U) &&
        !getPixColorXY(wrapX(x + 1U), HEIGHT - 1U) &&
        !getPixColorXY(wrapX(x - 1U), HEIGHT - 1U))
    {
      drawPixelXY(x, HEIGHT - 1U, CHSV(random8(), Saturation, random8(64U, 255U)));
    }
  }

  // сдвигаем по диагонали
  for (uint8_t y = 0U; y < HEIGHT - 1U; y++)
  {
    for (uint8_t x = 0; x < WIDTH; x++)
    {
      drawPixelXY(wrapX(x + 1U), y, getPixColorXY(x, y + 1U));
    }
  }

  // уменьшаем яркость верхней линии, формируем "хвосты"
  for (uint8_t i = 0U; i < WIDTH; i++)
  {
    fadePixel(i, HEIGHT - 1U, e_TAIL_STEP);
  }
}