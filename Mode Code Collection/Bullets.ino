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