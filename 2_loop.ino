void loop() {
  if (dotTimer.isReady())calculateTime();                   // каждые 500 мс пересчёт и отправка времени
  if (newTimeFlag && curMode == 0) flipTick();              // перелистывание цифр
  dotBrightTick();                                          // плавное мигание точки
  if (GLITCH_ALLOWED && curMode == 0) glitchTick();         // глюки
  buttonsTick();                                            // кнопки
  settingsTick();                                           // настройки
#ifdef DS18B20
  if (temperaturaTimer.isReady()) sensor.requestTemp();  // получение температуры
  if (TemperaturaFlag != 1) backlBrightTick();           // плавное мигание подсветки ламп если не показыватся температура
#endif

#ifdef WS2812
  backlBrightTick();  // плавное мигание подсветки ламп
#endif
}
