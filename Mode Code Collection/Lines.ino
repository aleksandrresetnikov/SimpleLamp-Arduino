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