void effects(){
  switch(effectsNum){
    case 1: rainbowRoutine(); break;
    case 2: snake(); break;
    case 3: krin(); break;
    case 4: rainbow1(); break;
    case 5: rainbow2(); break;
    case 6: ball(); break;
    case 7: fireRoutine(); break;
    case 8: scatterColors(); break;
    case 9: bullets(); break;
    case 10: colorfulRain(); break;
    case 11: stains(); break;
    case 12: lighter();  break;
    case 13: lightBugs(); break;
    case 14: fire(); break;
    case 15: sparklesRoutine();/*plasma();*/ break;//plasma(); - не качественная
    case 16: plasma2(); break;
    case 17: MetaBalls(); break;
    case 18:
    //этот режим не паддерживается на ATmega328P (по причине нехватки ОЗУ)
    //effectsNum++;
    plasma3();
      break;
    case 19:
    //этот режим не паддерживается на ATmega328P (по причине нехватки ОЗУ)
    //effectsNum++;
    colorLines();
      break;
    case 20: fire2012WithPalette(); break;
    case 21: effectsNum++; /*snowRoutine();*/ break;
    case 22: stormRoutine2(); break;
    default:
    effectsNum = 1;
      break;
  }
}

void setNextEffects(){
  lastEffect = effectsNum;
  effectsNum++;
  setEppMode(effectsNum);
}

void effectsStatysReset(){
  motion = 0;
  
  colorCorectKrin = 0;
  timerKrin1 = 0;

  timerRainbow = 0;

  balls[0].corectXball = 1;
  balls[0].corectYball = 2;
  balls[0].colorBall = random(0,255);

  balls[1].corectXball = 2;
  balls[1].corectYball = 1;
  balls[1].colorBall = random(0,255);

  balls[1].kirYball = false;

  balls[2].corectXball = 2;
  balls[2].corectYball = 4;
  balls[2].colorBall = random(0,255);

  balls[2].kirYball = false;
  
  ChueScatterColors = 0;
  CvalueScatterColors = 0;
  triger = false;
  showTriger = true;

  for (int i = 0; i < BULLET_COUNT; i++){
    bulletSprits[i].movingType = 1;
    bulletSprits[i].bulletHue = random(0,255);
    bulletSprits[i].bulletX = random(0,WIDTH);
    bulletSprits[i].bulletY = random(0,HEIGHT);
  }
}
