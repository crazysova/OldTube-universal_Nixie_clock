/*
  Скетч к проекту "Часы на ГРИ версия 2"
  Страница проекта (схемы, описания): https://alexgyver.ru/nixieclock_v2/
  Исходники на GitHub: https://github.com/AlexGyver/NixieClock_v2
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver Technologies, 2018
  https://AlexGyver.ru/
  Платы часов, изменения в коде: crazysova
  https://oshwlab.com/crazysova/nexie-clock-cccr_copy_copy
*/

/*
  Управление:
  - Настройка времени:
  - ВЕРХНЯЯ (синяя термоусадка) - короткое нажатия "+".
  - СРЕДНЯЯ (красная термоусадка) - долгое нажатие настройка времени, коротное переключение между часами и минутами.
  - НИЖНЯЯ  (жолтая термоусадка) - короткое нажатиями "-". 

  - Управление эффектами В РЕЖИМЕ ЧАСОВ:
     -Клик по средней кнопке останавливает и запускает перебор цветов подсветки
    - Удержание нижней кнопки включает и выключает "глюки"
    - Клик по нижней кнопке переключает режимы подсветки ламп
      - Дыхание
      - Постоянное свечение
      - Отключена
    - Клик по верхней кнопке переключает режимы перелистывания цифр
      - Без эффекта
      - Плавное угасание
      - Перемотка по порядку числа
      - Перемотка по катодам
      - Поезд
      - Резинка
    - Удержание верхней кнопки показывает температуру
*/
/*
Ошибки в прошивке:
  - При отключении WS2812 перестоют переключатся эффекты переключения цифр.
  - При выключении глюков перестаёт гореть последняя лампа при 8 лампах.
  - На 4 и 5 режиме эффект не останавливается при 8 лампах
*/

// ************************** НАСТРОЙКИ **************************
//Включи для использования датчика температуры
//#define DS18B20
//Включи для использования будильника
//#define ALM_TIMEOUT 30      // таймаут будильника
//#define FREQ 900            // частота писка будильника
//Включи для использования датчика освещёности
//#define LIGHT_SENSOR
//Включи для использования шим генератора высокого напряжения
//#define DUTY 180        // скважность ШИМ. От скважности зависит напряжение! у меня 175 вольт при значении 180 и 145 вольт при 120
//Включи для использования адресных светодиодов
#define WS2812
//Выбери плату OldTube
//#define IN17_6PTS
//#define IN17_4PTS_V2
//#define IN12_4PTS_V1
//#define IN12_4PTS_V2
#define IN12_8PTS_V1
//#define IN18_4PTS

// ======================= ЭФФЕКТЫ =======================
// эффекты перелистывания часов
byte FLIP_EFFECT = 1;
// Выбранный активен при первом запуске и меняется кнопками. Запоминается в память
// 0 - нет эффекта
// 1 - плавное угасание и появление (рекомендуемая скорость: 100-150)
// 2 - перемотка по порядку числа (рекомендуемая скорость: 50-80)
// 3 - перемотка по порядку катодов в лампе (рекомендуемая скорость: 30-50)
// 4 - поезд (рекомендуемая скорость: 50-170)
// 5 - резинка (рекомендуемая скорость: 50-150)


// =======================  ЯРКОСТЬ =======================
#define NIGHT_LIGHT 0       // менять яркость от времени суток (1 вкл, 0 выкл)
#define NIGHT_START 23      // час перехода на ночную подсветку (BRIGHT_N)
#define NIGHT_END 7         // час перехода на дневную подсветку (BRIGHT)

#define INDI_BRIGHT 24      // яркость цифр дневная (1 - 24) !на 24 могут быть фантомные цифры!
#define INDI_BRIGHT_N 3     // яркость ночная (1 - 24)

#define DOT_BRIGHT 200       // яркость точки дневная (1 - 255)
#define DOT_BRIGHT_N 100     // яркость точки ночная (1 - 255)

#define BACKL_BRIGHT 250    // макс. яркость подсветки ламп дневная (0 - 255)
#define BACKL_BRIGHT_N 50   // макс. яркость подсветки ламп ночная (0 - 255, 0 - подсветка выключена)
#define BACKL_MIN_BRIGHT 20 // мин. яркость подсветки ламп в режиме дыхание (0 - 255)
#define BACKL_PAUSE 400     // пазуа "темноты" между вспышками подсветки ламп в режиме дыхание, мс

// ======================= ЭФФЕКТ ВОЛНА - WS2812E =======================
#define WAVE_STEP 200
#define WAVE_PAUSE 1
#define WAVE_TIME 5000
#define WAVE_MIN_BRIGHT 0 //1530
#define WAVE_MAX_BRIGHT 1230

// ======================= ГЛЮКИ =======================
#define GLITCH_MIN 30       // минимальное время между глюками, с
#define GLITCH_MAX 120      // максимальное время между глюками, с

// ====================== МИГАНИЕ =======================
#define DOT_TIME 500        // время мигания точки, мс
#define DOT_TIMER 20        // шаг яркости точки, мс

#define BACKL_STEP 2        // шаг мигания подсветки
#define BACKL_TIME 5000     // период подсветки, мс

// ================== АНТИОТРАВЛЕНИЕ ====================
#define BURN_TIME 10        // период обхода индикаторов в режиме очистки, мс
#define BURN_LOOPS 3        // количество циклов очистки за каждый период
#define BURN_PERIOD 15      // период антиотравления, минут

// *********************** ДЛЯ РАЗРАБОТЧИКОВ ***********************
byte BACKL_MODE = 3;                          // Выбранный активен при запуске и меняется кнопками
byte FLIP_SPEED[] = {0, 15, 80, 80, 100, 70}; // скорость эффектов, мс (количество не меняй)
byte FLIP_EFFECT_NUM = sizeof(FLIP_SPEED);    // количество эффектов
boolean GLITCH_ALLOWED = 1;                   // 1 - включить, 0 - выключить глюки. Управляется кнопкой
int SAVE_COLOR = 700;                         // Цвет подсветки по умолчанию strip.fill(mWHEEL(nextColor));

// дешифратор
#define DECODER0 A0
#define DECODER1 A2
#define DECODER2 A3
#define DECODER3 A1

// распиновка ламп
#ifdef IN17_6PTS
  #define PIEZO 10  // пищалка
  #define KEY0 1    // 4
  #define KEY1 2    // 3
  #define KEY2 3    // 2
  #define KEY3 4    // 1
  #define KEY4 8    // 6
  #define KEY5 7    // 5
  #define KEY6 5    // 
  #define KEY7 6    // 
  #define DOT 0     // 
  #define BTN1 11   // кнопка 1
  #define BTN2 12   // кнопка 2
  #define BTN3 13   // кнопка 3
  #define BACKL 9   // подсветка
  #define LIGHT_SENSOR //ADS7
  #define FULL_LAMP 6 // количество ламп
  const byte digitMask[] = { 6, 3, 2, 7, 4, 5, 1, 9, 8, 0 };    // тут вводим свой порядок пинов
  const byte opts[] = { KEY3, KEY2, KEY1, KEY0, KEY5, KEY4 };   // порядок индикаторов слева направо
  const byte cathodeMask[] = {1,7,2,8,0,6,3,9,4,5};             // порядок катодов
#endif

// пины
#ifdef IN12_4PTS_V1
  #define PIEZO 9   // пищалка
  #define KEY0 1    // часы      PD1
  #define KEY1 2    // часы      PD2
  #define KEY2 3    // минуты    PB6
  #define KEY3 4    // минуты    PB7
  #define KEY4 20   // минуты    PB6
  #define KEY5 21   // минуты    PB7
  #define BTN1 11   // кнопка    MOSI
  #define BTN2 12   // кнопка    MISO
  #define BTN3 13   // кнопка    SCK
  #define DOT_U 0   // точка     PD0 (точка up)
  #define DOT_D 5   // точка     PD5 (точка down)
  #define LED_PIN 6 // подсветка PD6
  #define NUMLEDS 4 // кол-во светодиодов
  #define FULL_LAMP 4 // количество ламп
  const byte digitMask[] = {3, 2, 7, 4, 1, 0, 8, 9, 5, 6};// тут вводим свой порядок пинов
  const byte opts[] = {KEY0, KEY1, KEY2, KEY3};              // порядок индикаторов справа налево (для IN-12 turned) и ин-14
  const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3}; // порядок катодов in12
#endif

// пины
#ifdef IN12_8PTS_V1
  #define PIEZO 10   // PB2 пищалка (температура)
  #define KEY0 1     // PD1 часы
  #define KEY1 2     // PD2 часы 
  #define KEY2 3     // PD3 минуты
  #define KEY3 4     // PD4 минуты
  #define KEY4 8     // сек
  #define KEY5 7     // сек
  #define KEY6 5     // темп
  #define KEY7 6     // темп
  #define BTN1 11    // кнопка 1 btnSet
  #define BTN2 12    // кнопка 2 btnL
  #define BTN3 13    // кнопка 3 btnR
  #define DOT 0      // PD0 точка (точка up)
  #define LED_PIN 9  // PB1 пин ленты
  #define NUMLEDS 8  // кол-во светодиодов
  #define FULL_LAMP 8 // количество ламп
  const byte digitMask[] = {3, 2, 7, 4, 1, 0, 8, 9, 5, 6};   // тут вводим свой порядок пинов
  const byte opts[] = { KEY5, KEY4, KEY3, KEY2, KEY1, KEY0, KEY6, KEY7 }; // свой порядок индикаторов
  const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3}; // порядок катодов in12
#endif

// пины
#ifdef IN12_4PTS_V2
  #define PIEZO 10   // PB2 пищалка (температура)
  #define KEY0 1     // PD1 часы
  #define KEY1 2     // PD2 часы 
  #define KEY2 3     // PD3 минуты
  #define KEY3 4     // PD4 минуты
  #define BTN1 11    // кнопка 1 btnSet
  #define BTN2 12    // кнопка 2 btnL
  #define BTN3 13    // кнопка 3 btnR
  #define DOT 0      // PD0 точка (точка up)
  #define LED_PIN 9  // PB1 пин ленты
  #define NUMLEDS 4  // кол-во светодиодов
  #define FULL_LAMP 4 // количество ламп
  const byte digitMask[] = {3, 2, 7, 4, 1, 0, 8, 9, 5, 6};   // тут вводим свой порядок пинов
  const byte opts[] = {KEY0, KEY1, KEY2, KEY3};              // свой порядок индикаторов
  const byte cathodeMask[] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3}; // порядок катодов in12
#endif

// пины
#ifdef IN18_4PTS
  #define PIEZO 10   // PB2 пищалка (температура)
  #define KEY0 1     // PD1 часы
  #define KEY1 2     // PD2 часы 
  #define KEY2 3     // PD3 минуты
  #define KEY3 4     // PD4 минуты
  #define BTN1 11    // кнопка 1 btnSet
  #define BTN2 12    // кнопка 2 btnL
  #define BTN3 13    // кнопка 3 btnR
  #define DOT 0      // PD0 точка (точка up)
  #define LED_PIN 9  // PB1 пин ленты
  #define NUMLEDS 4  // кол-во светодиодов
  #define FULL_LAMP 4 // количество ламп
  const byte digitMask[] = {7,2,3,5,1,9,8,0,6,4};   // тут вводим свой порядок пинов
  const byte opts[] = {KEY0, KEY1, KEY3, KEY2};     // свой порядок индикаторов
  const byte cathodeMask[] = {1,0,2,9,8,3,4,7,6,5}; // порядок катодов
#endif

// пины
#ifdef IN14_4PTS_V2
  #define PIEZO 10   // PB2 пищалка (температура)
  #define KEY0 1     // PD1 часы
  #define KEY1 2     // PD2 часы 
  #define KEY2 3     // PD3 минуты
  #define KEY3 4     // PD4 минуты
  #define BTN1 11    // кнопка 1 btnSet
  #define BTN2 12    // кнопка 2 btnL
  #define BTN3 13    // кнопка 3 btnR
  #define DOT 0      // PD0 точка (точка up)
  #define LED_PIN 9  // PB1 пин ленты
  #define NUMLEDS 4  // кол-во светодиодов
  #define FULL_LAMP 4 // количество ламп
  const byte digitMask[] = {3, 2, 7, 4, 1, 0, 8, 9, 5, 6};   // тут вводим свой порядок пинов
  const byte opts[] = {KEY0, KEY1, KEY2, KEY3};              // свой порядок индикаторов
  const byte cathodeMask[] = {1, 0, 2, 9, 3, 8, 4, 7, 5, 6};  // порядок катодов in14
#endif