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