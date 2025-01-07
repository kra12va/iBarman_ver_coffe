/*

*/

#define DEBUG  // Раскоментировать при отладке
#include "Arduino.h"
#include <WiFi.h>
#include <WiFiAP.h>
#include <ArduinoJson.h>
#include "FS.h"
#include "FFat.h"
#include <TimerMs.h>

#include <WebServer.h>
WebServer HTTP(80);

#include <SimpleFTPServer.h>
FtpServer ftpSrv;  //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial

#include <ElegantOTA.h>  // прошивка по воздуху
unsigned long ota_progress_millis = 0; // для обновления по воздуху

#include "libs/configurator.h"
#include "libs/file_system.h" // Работа с файловой системой FFat


//            КОД ДЛЯ КОНКРЕТНЫХ ПРОЕКТОВ  
#include "bot_libs/GyverHX711.h"

// (период, мс), (0 не запущен / 1 запущен), (режим: 0 период / 1 таймер)
TimerMs listen_sensor_tmr(500, 1, 1);
TimerMs listen_bnt_tmr(200, 1, 1);

#define HX_SCK 16
#define HX_DT 17
GyverHX711 hx(HX_DT, HX_SCK, HX_GAIN64_A);

#define PIN_BTN_SCALES 18 //пин проверки веса (весы) Кнопка 2
#define PIN_BTN_TARE 19 //Калибровка весов (Тарирование) Кнопка 3

int drink_pin[] = {32, 27, 14, 15, 22, 23}; //указываем пины для налива напитков 
#include <bot_libs/bot_action.h>
bot_action bot(drink_pin, 6); //указываем пины для налива напитков 

int recipe[10][10];

bool flag_drink_go = false;
bool flag_run = false;
int i = 0;
int step = -1;
int mode = -1;

struct Weight {
  int delta = 3; // дельта погрешности весов
  int container_min = 3;        // Масса стакана должна быть больше
  int container;        // Масса стакана должна быть больше
  int curent;
  bool container_flag = false;
  int liquid_max = 190; //максимальный вес жидкости в стакане
  int liquid_curent = 0; // текущее количество жидкости в стакане
};

Weight weight;

//      КОНЕЦ КОД ДЛЯ КОНКРЕТНЫХ ПРОЕКТОВ  

configurator MK;


void setup() {

  Serial.begin(115200);
  
  FFat_file_system_init();    // Инициируем файловую систему
  MK.read_config();
  MK.init_wifi_connection();

  // Обработка HTTP-запросов
  HTTP_init();  
  FTP_server_init();
  ElegantOTA_init();

//            КОД ДЛЯ КОНКРЕТНЫХ ПРОЕКТОВ
  pinMode(PIN_BTN_SCALES, INPUT_PULLUP);
  pinMode(PIN_BTN_TARE, INPUT_PULLUP);  
  listen_sensor_tmr.setPeriodMode();
  listen_bnt_tmr.setTimerMode();
  listen_bnt_tmr.start();  
  bot.print_pins();

  delay(2000);
  hx.tare();    // калибровка нуля  

//      КОНЕЦ КОД ДЛЯ КОНКРЕТНЫХ ПРОЕКТОВ  
}

void loop() {
  HTTP.handleClient();
  ftpSrv.handleFTP();  //make sure in loop you call handleFTP()!!

//            КОД ДЛЯ КОНКРЕТНЫХ ПРОЕКТОВ  
  if (!digitalRead(PIN_BTN_TARE)) {
   if (listen_bnt_tmr.tick()){
     listen_bnt_tmr.start();
     Serial.println("Нажата кнопка 3 PIN_BTN_TARE, Калибровка весов "); 
     hx.tare();
    }
  };  

  if (hx.available()) {
    weight.curent = (int)abs(hx.read())/923;
  }    

  if (weight.curent > weight.container_min) {
    weight.container_flag = true;
  }  
  if (weight.curent <= weight.container_min) {
    weight.container_flag = false;
  }    

  if (!digitalRead(PIN_BTN_SCALES)) {
    if (listen_bnt_tmr.tick()){
      listen_bnt_tmr.start();
     
      Serial.println("Нажата кнопка 2 PIN_BTN_SCALES, вес: " + String(weight.curent));

      if (!weight.container_flag) {
        Serial.println("Стакана нет: ");
      }  

      if (weight.container_flag) {
        Serial.println("Стакана установлен: ");
      } 
    }
  }
     
    //switch (mode) {
    //case 0:
    //  break;  
    //}

  switch (step) {
    case 0:
      // Наливаю... взвешиваю...
      //Serial.println("Жду команды: "); 
      if(weight.curent > (weight.container + weight.liquid_curent)){
        digitalWrite(bot.drink_number(recipe[i][0]), 0);
        Serial.println("Закрываю краник...");
        Serial.println("Текущий вес жидкости и стакана" + String(weight.curent));
        step = 1;
        i++;
      }
      break;    
    case 1:
      if (recipe[i][0] < 0){
        flag_run = false;
        i = 0; 
        step = 3;
        break;
      }    
      // Начинаю наливать

        digitalWrite(bot.drink_number(recipe[i][0]), 1);
        Serial.println("Наливаю: "); 
        Serial.println("Напток: " + String(recipe[i][0]) + "Вес: " + String(recipe[i][1])); 
        weight.liquid_curent = weight.liquid_curent + recipe[i][1];
        step = 0;


      break;
    case 2:
      // 
      Serial.println("Прекращаю наливать: ");   
      flag_run = false;
      i++;
      step = 1 ;   

      break;      

    case 3:
      // 
      if (flag_run == false){
        flag_run = true;  
        Serial.println("Заберите стакан: ");     
      }
      
      if (weight.curent <= (weight.container_min + weight.delta)) {
        weight.container_flag = false;
        weight.liquid_curent = 0;   
        step = 4 ;           
      }        
      break;  

    case 4:
      // 
      Serial.println("Сделайте новый заказ: ");  
      step = -1 ;   
      flag_run = false;  
      flag_drink_go = false;    
      break;      
    }
//      КОНЕЦ КОД ДЛЯ КОНКРЕТНЫХ ПРОЕКТОВ  


}

