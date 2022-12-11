void buttonsMeneger(){
  if (timerButtonPover < millis())
    buttonsPoverMenager();
  else
    if (!digitalRead(BUTTON_POVER))
      doublePressButtonPover();
    
  if (timerButtonNextEffect < millis())
    buttonsNextEffectMenager();
  else
    if (!digitalRead(BUTTON_NEXT_EFFECT))
      doublePressButtonNextEffect();   
}

void buttonsPoverMenager(){
  if (digitalRead(BUTTON_POVER))return;

  timerButtonPoverUnClick = millis() + TIME_SETTINGS_CORECTION;
  for (;;){
    if (timerButtonPoverUnClick < millis()){
      if (digitalRead(BUTTON_POVER))break;
      buttonPoverHold();
    }
    else if (digitalRead(BUTTON_POVER)){
      buttonsPoverMenagerHELPER();
      break;
    }
  }
}

void buttonsNextEffectMenager(){
  if (digitalRead(BUTTON_NEXT_EFFECT))return;
  
  timerButtonNextEffectUnClick = millis() + TIME_BRIGHTNESS_CORECTION;
  for (;;){
    if (timerButtonNextEffectUnClick < millis()){
      if (digitalRead(BUTTON_NEXT_EFFECT)){break;}
      buttonNextEffectHold();
    }
    else if (digitalRead(BUTTON_NEXT_EFFECT)){
      buttonsNextEffectMenagerHELPER();
      break;
    }
  }
}//p.s. ужастный код, я знаю :)

unsigned long helpTimer = 0;
boolean seringWarning = true; 
void buttonPoverHold(){
  //effectsStatysReset();
  //startSettingsVivo = true;
  //settingsStatus = true;
}

boolean brightnessPreValue_state = true;
byte brightnessPreValue = 1;
void buttonNextEffectHold(){
  //return;
  brightness+=1;
  setEppBrightness(brightness);

  if (WARNING_BRIGHTNING_UP && brightness >= WARNING_LEVEL_BRIGHTNESS_INCREASES){
    if (brightnessPreValue_state) {
      brightnessPreValue+=10;
      if (brightnessPreValue >= WARNING_LEVEL_BRIGHTNESS_INCREASES-10) brightnessPreValue_state = !brightnessPreValue_state;
    } else {
      brightnessPreValue-=10;
      if (brightnessPreValue <= 1) brightnessPreValue_state = !brightnessPreValue_state;
    }
    FastLED.setBrightness(brightness - brightnessPreValue);
    FastLED.show();
  } else {
    refreshBrightness();
    Serial.println(brightness);
  }
  if (brightness >= MAX_BRIGHTNESS)brightness = MIN_BRIGHTNESS;

  /*if (WARNING_BRIGHTNING_UP){
    if (brightness >= MAX_BRIGHTNESS - WARNING_LEVEL_BRIGHTNESS_INCREASES){
      if (seringWarning){
        refreshBrightness();
      }
      else{
        FastLED.setBrightness(0);
        FastLED.show();
      }
    }
    if (helpTimer < millis()){
      seringWarning = !seringWarning;
      helpTimer = 500 + millis();
    }
  }
  else
    refreshBrightness();*/

  delay(BRIGHTNESS_INCREASE_DELAY);
  //refreshBrightness();
}

void doublePressButtonPover(){
  effectsStatysReset();
  autoSwitchingModes = !autoSwitchingModes ;
  if (autoSwitchingModes)
    timerAutoSwitchingModes = millis() + (TIME_AUTO_MODE_CHANGEOVER * 1000);
  if (!poverStatus)
    poverStatus = true;
  delay(200);
  timerButtonPover = millis() + BURNING_POWER_BUTTON_PRESS;

  setAutoChangeRimsStatus(autoSwitchingModes);
}

void doublePressButtonNextEffect(){
  
}

void buttonsPoverMenagerHELPER(){
  poverStatus = !poverStatus;
  if (poverStatus == true){
    if (GREETINGS) 
      startAnimation();
    if (autoSwitchingModes)
      timerAutoSwitchingModes = millis() + (TIME_AUTO_MODE_CHANGEOVER * 1000);
  }
  timerButtonPover = millis() + BURNING_POWER_BUTTON_PRESS;
  resetTimerAutoSwitchingModes();
  setPoverStatus(poverStatus);
}

void buttonsNextEffectMenagerHELPER(){
  setNextEffects();
  clearAll();
  effectsStatysReset();
  resetTimerAutoSwitchingModes();
}
