#include <FastLED.h>
#include <EEPROM.h>
#include "fonts.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>

#define STASSID "Simple_Lamp"
#define STAPSK  "SANE5K_Lamp"


#define CURRENT_LIMIT 0                       // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define NUM_LEDS 256                          // количество светодиодов
#define DATA_PIN 12                            // пмн на матрицу
#define CLOCK_PIN 13                          // !!!не нужно!!!
#define BUTTON_MENEGER 1                      // обработка нажатий кнопок
#define BUTTON_POVER 16                       // пин кнопки питания
#define BUTTON_NEXT_EFFECT 5                  // пин кнопки смены режима


#define WIDTH 16                              // ширина матрицы
#define HEIGHT 16                             // высота матрицы


#define MODE_AMOUNT 14                        // количество режимов
#define START_MODE 20                         // режим при старте


#define BRIGHTNESS 15                         // начальная яркость
#define MIN_BRIGHTNESS 10                     // начальная яркость  ***если минимальная яркость будет больше максимальной, то при смене яркости будет ошибка, лампа на этой версии не защищена от такой ошибки***
#define MAX_BRIGHTNESS 255                    // начальная яркость
#define TIME_BRIGHTNESS_CORECTION 750         // время перед крекцией яркости (млс)
#define TIME_SETTINGS_CORECTION 500           // время перед крекцией настроек (млс)
#define BRIGHTNESS_INCREASE_DELAY 38          // задержка при увеличении яркости
#define WARNING_LEVEL_BRIGHTNESS_INCREASES 150// уровень предупреждения при высокой яркости (для ощущения края яркости)
#define WARNING_BRIGHTNING_UP 1               // предупреждать при высокой яркости (для ощущения края яркости)


#define BURNING_POWER_BUTTON_PRESS 300        // ожидание перед двойным нажатием кнопки питания
#define BURNING_NEXT_EFFECT_BUTTON_PRESS 300  // ожидание перед двойным нажатием кнопки следующего режима


#define GREETINGS 1                           // приветствие при включении
#define ON_AT_START 1                         // включен-ли при старте


#define AUTO_MODE_CHANGEOVER 0                // автоматическое переключение режимов при старте
#define TIME_AUTO_MODE_CHANGEOVER 30          // время автоматического переключения режимов (в секундах)


#define EEPROM_USAGE 0                        // будет-ли использоваться энергонезависимая память

// --------------------------- настройки бегущего текста ------------------------------

#define TEXT_DIRECTION 1                      // 1 - по горизонтали, 0 - по вертикали
#define MIRR_V 0                              // отразить текст по вертикали (0 / 1)
#define MIRR_H 0                              // отразить текст по горизонтали (0 / 1)


#define TEXT_HEIGHT 3                         // высота, на которой бежит текст (от низа матрицы)
#define LET_WIDTH 5                           // ширина буквы шрифта
#define LET_HEIGHT 8                          // высота буквы шрифта
#define SPACE 1                               // пробел


#define DELAYING_OUTPUT_OF_FUTURE_LINE 50     // задержка вывода бегущеё строки 
#define BRIGHTNESS_WHEN_OUTPUT_TEXT 50        // яркость при выводе текста


static const uint8_t maxDim = max(WIDTH, HEIGHT);


// эффект "огонь"
#define SPARKLES 1                            // вылетающие угольки вкл выкл
#define FIRE_PALETTE 3                        // разные типы огня2 (0 - 3).
unsigned char line[WIDTH];
int pcnt = 0;
boolean loadingFlag = true;
unsigned char matrixValue[8][16];
CRGBPalette16 gPal;
bool gReverseDirection = false;

// ----------------- ПЕРЕМЕННЫЕ ДЛЯ РЕЖИМОВ ------------------

// ----------------- ПЕРЕМЕННЫЕ ------------------
boolean poverStatus = ON_AT_START;
boolean settingsStatus = false;

boolean startSettingsVivo = true;

struct {
  byte brightness = BRIGHTNESS;
  byte speed = 1;
  byte scale = 10;
} modes[MODE_AMOUNT];

byte effectsNum = START_MODE;
byte lastEffect = 1;
byte brightness = BRIGHTNESS;

unsigned long timerButtonPover = 0;
unsigned long timerButtonNextEffect = 0;

unsigned long timerButtonPoverUnClick = 0;
unsigned long timerButtonNextEffectUnClick = 0;

boolean autoSwitchingModes = AUTO_MODE_CHANGEOVER;
unsigned long timerAutoSwitchingModes = millis() + (TIME_AUTO_MODE_CHANGEOVER * 1000);

// ------------------- ТИПЫ --------------------
uint8_t CentreX =  (WIDTH / 2) - 1;
uint8_t CentreY = (HEIGHT / 2) - 1;
CRGB leds[NUM_LEDS];

void parametersFire2(){
  if (FIRE_PALETTE == 0) gPal = HeatColors_p;
  else if (FIRE_PALETTE == 1) gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
  else if (FIRE_PALETTE == 2) gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);
  else if (FIRE_PALETTE == 3) gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);
}

// ------------------- FiFi -------------------

const char* ssid = STASSID;
const char* password = STAPSK;

WiFiServer server(80);

void setup() {
  delay(1000);  
  // Запуск Serial
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");

  // Запуск WiFi
  WiFi.softAP(ssid, password); 

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println(WiFi.SSID());

  // Запуск сервера
  initServer();

  // Запуск EEPROM
  if (EEPROM_USAGE)
    EEPROM.begin(1024);

  // Запуск FastLED
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  if (CURRENT_LIMIT > 0) 
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);

  if (EEPROM_USAGE)
    startEeprom();
    
  parametersFire2();
  clearAll();

  if (BUTTON_MENEGER){
    pinMode(BUTTON_POVER, INPUT_PULLUP);
    pinMode(BUTTON_NEXT_EFFECT, INPUT_PULLUP);
  }                   

  resetString();

  fillAll(CRGB::Azure);
  FastLED.show();
}

void initServer(){
  server.begin();
} 

uint64_t data_timer = millis() + 1000;
uint64_t data_bytesCount = 0;

uint32_t TFPS = 0;
uint64_t TFPS_timer = millis() + 1000;
void loop() {
  server_looping();
  //server_looping2();
  lamp_looping();

  if (millis() > TFPS_timer){
    //Serial.println(TFPS);
    TFPS = 0;
    TFPS_timer = millis() + 1000;
  }
}

uint64_t updateServerTimer = millis() + 500;
void server_looping(){
  //if (updateServerTimer > millis()) return;

  WiFiClient client = server.available();

  if (client){
    while (client){
      byte data_step = 0;
      byte data_type = 0;
      while (client && client.available() > 0){
        data_step++;
        uint8_t data = client.read();
        if (data_step == 1){
          data_type = data;
        } else if (data_step == 2){
          if (data_type == 0){
            brightness = data;
            FastLED.setBrightness(brightness);
          } else if (data_type == 255) {
            clearAll();
            effectsNum = data;
          }
        } else {
          client.flush();
          break;
        }
      }

      lamp_looping();
    }
  }

  
  updateServerTimer = millis() + 500;
}

void server_looping2(){
  //if (updateServerTimer > millis()) return;
  updateServerTimer = millis() + 500;

  WiFiClient client = server.available();
  if (!client) return;
  while (!client.available()) { }
  Serial.println("New client");

  String req = client.readStringUntil('\r');
  client.flush();

  req[0] = ' ';
  req.replace("HTTP/1.1", "");
  req.replace("GET /", "");
  req.replace("ET /", "");

  Serial.println(req);
}

uint64_t updateTimer = millis() + 15;
void lamp_looping(){
  if (updateTimer > millis()) return;

  if (BUTTON_MENEGER)
    buttonsMeneger();
  if (EEPROM_USAGE)
    menegerEeprom();

  if (poverStatus){
    if (autoSwitchingModes && timerAutoSwitchingModes < millis()){
      setNextEffects();
      clearAll();
      resetTimerAutoSwitchingModes();
    }
    //clearAll();
    leds[0] = 0; //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    effects();
  }
  else
    clearAll();

  refreshBrightness(false);
  FastLED.show(); TFPS++;

  updateTimer = millis() + 15;
}

void startAnimation(){
  ticker("Добрый день", CRGB::White, true);
  effectsStatysReset();
  refreshBrightness();
}
