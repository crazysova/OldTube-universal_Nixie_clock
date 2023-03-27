#ifdef WS2812
void backlBrightTick() {
  if (backlBrightTimer.isReady()) {
    if (backlMaxBright > 0) {
      // Плавное зажигание и гашение с перебором цветов по радуге
      if (BACKL_MODE == 0) {
        if (backlBrightDirection) {
          if (!backlBrightFlag) {
            backlBrightFlag = true;
            backlBrightTimer.setInterval((float)BACKL_STEP / backlMaxBright / 2 * BACKL_TIME);
          }
          backlBrightCounter += BACKL_STEP;
          if (backlBrightCounter >= backlMaxBright) {
            backlBrightDirection = false;
            backlBrightCounter = backlMaxBright;
          }
        } else {
          backlBrightCounter -= BACKL_STEP;
          if (backlBrightCounter <= BACKL_MIN_BRIGHT) {
            backlBrightDirection = true;
            backlBrightCounter = BACKL_MIN_BRIGHT;
            backlBrightTimer.setInterval(BACKL_PAUSE);
            backlBrightFlag = false;
          }
        }
        strip.setBrightness(backlBrightCounter);
        //setPWM(DOT, getPWM_CRT(backlBrightCounter));
        if (SAVE_COLOR == 0) {
          nextColor++;
          if (nextColor > 1530)nextColor = 1;
        }
        strip.fill(mWHEEL(nextColor));
        strip.show();
      }
      //Бегущяя радуга
      else if (BACKL_MODE == 1) {
        static byte counter = 0;
        for (byte i = 0; i < NUMLEDS; i++) {
          strip.setHSV(i, counter + i * (255 / NUMLEDS), 255, 255);  // можно так
        }
        counter += 1;
        strip.show();
      }
      // Статичный цвет
      else if (BACKL_MODE == 2) {
        strip.setBrightness(255);
        if (SAVE_COLOR == 0) {
          nextColor++;
          if (nextColor > 1530)nextColor = 1;
        }
        strip.fill(mWHEEL(nextColor));
        strip.show();
      }
      // Волна
      /*
        else if (BACKL_MODE == 2) {
        if (backlBrightDirection) {
          if (!backlBrightFlag) {
            backlBrightFlag = true;
            backlBrightTimer.setInterval((float)WAVE_STEP / WAVE_MAX_BRIGHT / 2 * WAVE_TIME);
          }
          nextColor += WAVE_STEP;
          if (nextColor >= WAVE_MAX_BRIGHT) {
            backlBrightDirection = false;
            nextColor = WAVE_MAX_BRIGHT;
          }
        } else {
          nextColor -= WAVE_STEP;
          if (nextColor <= WAVE_MIN_BRIGHT) {
            backlBrightDirection = true;
            nextColor = WAVE_MIN_BRIGHT;
            backlBrightTimer.setInterval(WAVE_PAUSE);
            backlBrightFlag = false;
          }
          nextColor = nextColor + 20;
          //if (nextColor > 1230)nextColor = 0;
          strip.setLED(0, mWHEEL(nextColor));
          strip.setLED(1, mWHEEL(nextColor + 100));
          strip.setLED(2, mWHEEL(nextColor + 200));
          strip.setLED(3, mWHEEL(nextColor + 300));
          strip.show();
        }
        }*/
      else {
        //digitalWrite(BACKL, 0);
        strip.clear();
        strip.show();
      }
    }
  }
}
#endif

void dotBrightTick() {
  if (dotBrightFlag && dotBrightTimer.isReady()) {
    if (dotBrightDirection) {
      dotBrightCounter += dotBrightStep;
      if (dotBrightCounter >= dotMaxBright) {
        dotBrightDirection = false;
        dotBrightCounter = dotMaxBright;
      }
    } else {
      dotBrightCounter -= dotBrightStep;
      if (dotBrightCounter <= 0) {
        dotBrightDirection = true;
        dotBrightFlag = false;
        dotBrightCounter = 0;
      }
    }
    //setPWM(DOT, getPWM_CRT(dotBrightCounter));
    //setPWM(DOT_D, getPWM_CRT(dotBrightCounter)); //Плавно не маргала потому что пин не подерживал режим шим
    //setPin(DOT_U, dotBrightFlag); // Будет просто мигать
    setPin(DOT, dotBrightFlag); // Будет просто мигать
#ifdef IN12_4PTS_V1
    setPin(DOT_U, dotBrightFlag); // Будет просто мигать
    setPin(DOT_D, dotBrightFlag); // Будет просто мигать
#endif
  }
}

void changeBright() {
#if (NIGHT_LIGHT == 1)
  // установка яркости всех светилок от времени суток
  if ( (hrs >= NIGHT_START && hrs <= 23)
       || (hrs >= 0 && hrs < NIGHT_END) ) {
    indiMaxBright = INDI_BRIGHT_N;
    dotMaxBright = DOT_BRIGHT_N;
    backlMaxBright = BACKL_BRIGHT_N;
  } else {
    indiMaxBright = INDI_BRIGHT;
    dotMaxBright = DOT_BRIGHT;
    backlMaxBright = BACKL_BRIGHT;
  }
  for (byte i = 0; i < FULL_LAMP; i++) {
    indiDimm[i] = indiMaxBright;
  }

  dotBrightStep = ceil((float)dotMaxBright * 2 / DOT_TIME * DOT_TIMER);
  if (dotBrightStep == 0) dotBrightStep = 1;

  if (backlMaxBright > 0)
    backlBrightTimer.setInterval((float)BACKL_STEP / backlMaxBright / 2 * BACKL_TIME);
  indiBrightCounter = indiMaxBright;

  //change PWM to apply backlMaxBright in case of maximum bright mode
  //if (BACKL_MODE == 1) setPWM(BACKL, backlMaxBright);
#endif
}
