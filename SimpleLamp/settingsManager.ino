void startSettingsManager(){
  ticker("Настройки", CRGB::White,true);
  startSettingsVivo = false;
}

void settingsManager(){
  if (startSettingsVivo)
    startSettingsManager();

  settingsButtonsManager();
}

void settingsButtonsManager(){
  if (!digitalRead(BUTTON_POVER))settingsStatus = false;
  if (!digitalRead(BUTTON_NEXT_EFFECT))settingsStatus = false;
}
