void settingsTick() {
  if (curMode == 1) {
    if (blinkTimer.isReady()) {
      sendTime(changeHrs, changeMins, secs);
      lampState = !lampState;
      if (lampState) {
        anodeStates[0] = 1;
        anodeStates[1] = 1;
        anodeStates[2] = 1;
        anodeStates[3] = 1;
      } else {
        if (!currentDigit) {
          anodeStates[0] = 0;
          anodeStates[1] = 0;
          anodeStates[2] = 1;
          anodeStates[3] = 1;
        } else {
          anodeStates[0] = 1;
          anodeStates[1] = 1;
          anodeStates[2] = 0;
          anodeStates[3] = 0;
        }
      }
    }
  }
}

void buttonsTick() {
  btnSet.tick();
  btnL.tick();
  btnR.tick();

  if (curMode == 1) {
    if (btnSet.isClick()) {
      currentDigit = !currentDigit;  // настройка времени
    }
    if (btnR.isClick()) {
      if (!currentDigit) {
        changeHrs++;
        if (changeHrs > 23) changeHrs = 0;
      } else {
        changeMins++;
        if (changeMins > 59) {
          changeMins = 0;
          changeHrs++;
          if (changeHrs > 23) changeHrs = 0;
        }
      }
      sendTime(changeHrs, changeMins, 0);
    }
    if (btnL.isClick()) {
      if (!currentDigit) {
        changeHrs--;
        if (changeHrs < 0) changeHrs = 23;
      } else {
        changeMins--;
        if (changeMins < 0) {
          changeMins = 59;
          changeHrs--;
          if (changeHrs < 0) changeHrs = 23;
        }
      }
      sendTime(changeHrs, changeMins, 0);
    }
  } else if (curMode == 0) {
    // переключение эффектов цифр
    if (btnR.isClick()) {
      if (++FLIP_EFFECT >= FLIP_EFFECT_NUM) FLIP_EFFECT = 0;
      EEPROM.put(0, FLIP_EFFECT);
      flipTimer.setInterval(FLIP_SPEED[FLIP_EFFECT]);
      for (byte i = 0; i < FULL_LAMP; i++) {
        indiDimm[i] = indiMaxBright;
        anodeStates[i] = 1;
      }
      // показать эффект
      newTimeFlag = true;
      // показать номер эффекта
      for (byte i = 0; i < FULL_LAMP; i++) indiDigits[i] = FLIP_EFFECT; 
    }

    // переключение эффектов подсветки
    if (btnL.isClick()) {
      if (++BACKL_MODE > 3) BACKL_MODE = 0;
      EEPROM.put(1, BACKL_MODE);
      /*
        if (BACKL_MODE == 1) {
        //setPWM(BACKL, backlMaxBright);
        strip.setBrightness(backlMaxBright);
        } else if (BACKL_MODE == 2) {
        //digitalWrite(BACKL, 0);
        strip.clear();
        strip.show();
        }
      */
    }

    // переключение глюков
    if (btnL.isHolded()) {/**/
      GLITCH_ALLOWED = !GLITCH_ALLOWED;
      EEPROM.put(2, GLITCH_ALLOWED);
      if (!GLITCH_ALLOWED) {
        glitchFlag = true;
        indiState = 0;
        glitchCounter = 0;
        glitchMax = 10;
        glitchIndic = random(0, FULL_LAMP);
        glitchTimer.setInterval(random(1, 6) * 20);
      } else if (GLITCH_ALLOWED) {
        indiDimm[glitchIndic] = indiState * indiMaxBright;
        indiState = !indiState;
        glitchTimer.setInterval(random(1, 6) * 20);
        glitchCounter++;
        if (glitchCounter > glitchMax) {
          glitchTimer.setInterval(random(GLITCH_MIN * 1000L, GLITCH_MAX * 1000L));
          glitchFlag = false;
          indiDimm[glitchIndic] = indiMaxBright;
        }
      }
    }

    // Остановка перебора цвета подсветки
    if (btnSet.isClick()) {
      if (SAVE_COLOR == 0) {
        SAVE_COLOR = nextColor;
        writeIntIntoEEPROM(4, SAVE_COLOR);
      } else SAVE_COLOR = 0;
    }
  }

// Настройка времени
  if (btnSet.isHolded()) {
    anodeStates[0] = 1;
    anodeStates[1] = 1;
    anodeStates[2] = 1;
    anodeStates[3] = 1;
    currentDigit = false;
    //curMode = !curMode;
    newTimeFlag = true;
    if (++curMode >= 2) curMode = 0;
    switch (curMode) {
      case 0:
        hrs = changeHrs;
        mins = changeMins;
        secs = 0;
        rtc.adjust(DateTime(2022, 12, 01, hrs, mins, 0));
        changeBright();
        newTimeFlag = true;
        break;
      case 1:
        changeHrs = hrs;
        changeMins = mins;
        break;
    }
  }

#ifdef DS18B20
  if (btnR.isHolded()) {
    TemperaturaFlag = 1;
    getTemperatura();
  }

  if (btnR.isRelease() && TemperaturaFlag == 1) {
    TemperaturaFlag = 0;
    newTimeFlag = true;
    anodeStates[0] = 1;
    anodeStates[1] = 1;
    anodeStates[2] = 1;
    anodeStates[3] = 1;
  }
#endif
}
