void setup() {
  #ifdef WS2812
    strip.setBrightness(BACKL_BRIGHT);    // яркость (0-255)
    strip.clear();
    strip.show();
  #endif
  // случайное зерно для генератора случайных чисел
  randomSeed(analogRead(6) + analogRead(7));

  // настройка пинов на выход
  pinMode(DECODER0, OUTPUT);
  pinMode(DECODER1, OUTPUT);
  pinMode(DECODER2, OUTPUT);
  pinMode(DECODER3, OUTPUT);
  pinMode(KEY0, OUTPUT);
  pinMode(KEY1, OUTPUT);
  pinMode(KEY2, OUTPUT);
  pinMode(KEY3, OUTPUT);
  pinMode(KEY4, OUTPUT);
  pinMode(KEY5, OUTPUT);
  pinMode(KEY6, OUTPUT);
  pinMode(KEY7, OUTPUT);
  pinMode(DOT, OUTPUT);
  pinMode(PIEZO, OUTPUT);
#ifdef IN12_4PTS_V1  
  pinMode(DOT_D, OUTPUT);
  digitalWrite(PIEZO, 1); // PNP транзистор отключить
#endif

#ifdef WS2812
  pinMode(LED_PIN, OUTPUT);
#endif

  // задаем частоту ШИМ на 9 и 10 выводах 31 кГц
  //TCCR1B = TCCR1B & 0b11111000 | 1;    // ставим делитель 1

  // включаем ШИМ
  //setPWM(9, DUTY);

  // перенастраиваем частоту ШИМ на пинах 3 и 11 на 7.8 кГц и разрешаем прерывания COMPA
  //TCCR2B = (TCCR2B & B11111000) | 2;    // делитель 8
  //OCR2A = 254;
  //TCCR2B = TCCR2B & B11111000 | B00000001; 
  //TCCR2A |= (1 << WGM21);   // включить CTC режим для COMPA
  //TIMSK2 |= (1 << OCIE2A);  // включить прерывания по совпадению COMPA
 
  //___________________________________________
  //https://www.arduinoslovakia.eu/application/timer-calculator
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;

  // 12048.192771084337 Hz (16000000/((165+1)*8))
  OCR2A = 165;
  // CTC
  TCCR2A |= (1 << WGM21);
  // Prescaler 8
  TCCR2B |= (1 << CS21);
  // Output Compare Match A Interrupt Enable
  TIMSK2 |= (1 << OCIE2A);
  //___________________________________________

  // ---------- RTC -----------
  rtc.begin();
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  DateTime now = rtc.now();
  secs = now.second();
  mins = now.minute();
  hrs = now.hour();
  date = now.day();
  month = now.month();
  year = now.year();

  // EEPROM
  if (EEPROM.read(1023) != 100) {   // первый запуск
    EEPROM.put(1023, 100);
    EEPROM.put(0, FLIP_EFFECT);
    EEPROM.put(1, BACKL_MODE);
    EEPROM.put(2, GLITCH_ALLOWED);
  }
  EEPROM.get(0, FLIP_EFFECT);
  EEPROM.get(1, BACKL_MODE);
  EEPROM.get(2, GLITCH_ALLOWED);
  SAVE_COLOR = readIntFromEEPROM(4);
  nextColor = SAVE_COLOR;

  /*if (EEPROM.read(100) != 66) {   // проверка на первый запуск. 66 от балды
    EEPROM.write(100, 66);
    EEPROM.write(0, 0);     // часы будильника
    EEPROM.write(1, 0);     // минуты будильника
    }
    alm_hrs = EEPROM.read(0);
    alm_mins = EEPROM.read(1);*/

  sendTime(hrs, mins, secs);  // отправить время на индикаторы
  changeBright();       // изменить яркость согласно времени суток

  // установить яркость на индикаторы
  for (byte i = 0; i < FULL_LAMP; i++)
    indiDimm[i] = indiMaxBright;

  // расчёт шага яркости точки
  dotBrightStep = ceil((float)dotMaxBright * 2 / DOT_TIME * DOT_TIMER);
  if (dotBrightStep == 0) dotBrightStep = 1;

  // дыхание подсветки
  //if (backlMaxBright > 0)
  backlBrightTimer.setInterval((float)BACKL_STEP / backlMaxBright / 2 * BACKL_TIME);
  //backlBrightTimer.setInterval(30);
  // стартовый период глюков
  glitchTimer.setInterval(random(GLITCH_MIN * 1000L, GLITCH_MAX * 1000L));
  indiBrightCounter = indiMaxBright;

  // скорость режима при запуске
  flipTimer.setInterval(FLIP_SPEED[FLIP_EFFECT]);
  //almTimer.stop();
}
