void dischargeEpp(){
  EEPROM.write(0, 1);
  EEPROM.write(1, START_MODE);
  EEPROM.write(2, ON_AT_START);
  EEPROM.write(3, BRIGHTNESS);
  EEPROM.write(4, AUTO_MODE_CHANGEOVER);
  menegerEeprom();
}

void menegerEeprom(){
  effectsNum = getEppMode();
  //poverStatus = getPoverStatus();
  brightness = getEppBrightness();
  autoSwitchingModes = getAutoChangeRimsStatus();
}

void startEeprom(){
  if (EEPROM.read(0) == 0)
    dischargeEpp();
  else if (EEPROM.read(0) == 1)
    menegerEeprom();
  else
    dischargeEpp();
}

// ------------------------------- чтение и запись --------------------------------

//режимы
void setEppMode(byte value){
  EEPROM.write(1, value);
}

byte getEppMode(){
  return EEPROM.read(1);
}

//статус активности 
void setPoverStatus(bool value){
  //if (value)EEPROM.write(1, 1);
  //else EEPROM.write(1, 0);
}

bool getPoverStatus(){
  //if (EEPROM.read(2) == 0)return 0;
  //else if (EEPROM.read(2) == 1)return 1;
  //else return 1;
}

//яркость
void setEppBrightness(byte value){
  EEPROM.write(3, value);
}

byte getEppBrightness(){
  return EEPROM.read(3);
}

//автоматическая смена режимов
void setAutoChangeRimsStatus(bool value){
  if (value)EEPROM.write(4, 1);
  else EEPROM.write(4, 0);
}

bool getAutoChangeRimsStatus(){
  if (EEPROM.read(4) == 0)return 0;
  else if (EEPROM.read(4) == 1)return 1;
  else return 0;
}
