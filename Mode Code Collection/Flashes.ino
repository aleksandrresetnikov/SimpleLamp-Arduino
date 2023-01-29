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