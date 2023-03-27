#ifdef DS18B20
void getTemperatura() {
  int8_t temperaturaPlusMinus;
  //backlMaxBright = 0;
  if (sensor.readTemp()) {
    temperatura = sensor.getTempInt(); // Запрашиваем температуру
    temperaturaPlusMinus = temperatura;
    temperatura = abs(temperatura);    // Убираем знак минус
    if (temperaturaPlusMinus > 0) {
      anodeStates[0] = 0;
      anodeStates[1] = 1;
      anodeStates[2] = 1;
      anodeStates[3] = 0;
      indiDigits[1] = (byte)temperatura / 10;
      indiDigits[2] = (byte)temperatura % 10;
      strip.setLED(0, mCOLOR(BLACK));
      strip.setLED(1, mCOLOR(RED));
      strip.setLED(2, mCOLOR(RED));
      strip.setLED(3, mCOLOR(BLACK));
      strip.show(); // выводим изменения на ленту
    }
    else
    {
      anodeStates[0] = 0;
      anodeStates[1] = 1;
      anodeStates[2] = 1;
      anodeStates[3] = 0;
      indiDigits[1] = (byte)temperatura / 10;
      indiDigits[2] = (byte)temperatura % 10;
      strip.setLED(0, mCOLOR(BLACK));
      strip.setLED(1, mCOLOR(NAVY));
      strip.setLED(2, mCOLOR(NAVY));
      strip.setLED(3, mCOLOR(BLACK));
      strip.show(); // выводим изменения на ленту
    }
  }
  else
  {
    anodeStates[0] = 1;
    anodeStates[1] = 0;
    anodeStates[2] = 0;
    anodeStates[3] = 1;

    indiDigits[0] = 9;
    indiDigits[3] = 9;
  }
}
#endif
