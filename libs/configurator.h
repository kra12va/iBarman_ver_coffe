class configurator {
  
  #define PIN_WFIF_AP_BTN 13 // Пин кнопки для принудительной настроки МК
  #define CFG_DEBUG  // Раскоментировать при отладке
  #include "libs/wifi_connection.h" // Работа с файловой системой FFat
  public:
    configurator(){

    }

    void read_config(){
      #ifdef CFG_DEBUG
        Serial.println("Функция read_confi конфигуратора запустилась");
        Serial.println("wifi_cfg_path = " + wifi_cfg_path);
        Serial.println("ui_cfg_path = " + ui_cfg_path);
      #endif

      read_wifi_cfg(wifi_cfg_path);
    }

    void init_wifi_connection() {
      #ifdef CFG_DEBUG
      Serial.println("Отработал конструктор конфигаратора.");
      #endif
      pinMode(PIN_WFIF_AP_BTN, INPUT_PULLUP);      


      if (!digitalRead(PIN_WFIF_AP_BTN)){
        Serial.println("Нажата кнопка принудительной настройки МК, создаю точку доступа ");
        wifi_AP_DEFAULT_run();
      }else{
        wifi_run(); // Подключаюсь к Вайфай сети 
      }

     
    }

    void read_wifi_cfg(String path) {
      #ifdef CFG_DEBUG
        Serial.println("Функция read_wifi_cfg конфигуратора запустилась");
        Serial.println("path = " + path);
      #endif    

      File configFile = FFat.open(path, "r");
      // Allocate a temporary JsonDocument
      // Don't forget to change the capacity to match your requirements.
      // Use arduinojson.org/v6/assistant to compute the capacity.
      StaticJsonDocument<512> doc;
      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, configFile);
      if (error){
        #ifdef CFG_DEBUG
        Serial.println(F("Failed to read file, using default configuration"));
        #endif
        wifi_cfg.ap = true;    
      }else{
      // Copy values from the JsonDocument to the Config
        wifi_cfg.ssid = doc["ssid"].as<String>();
        wifi_cfg.pwd = doc["pwd"].as<String>();
        wifi_cfg.ap = doc["ap"];

        #ifdef CFG_DEBUG
        Serial.println("Считываем конфигурацию вайфай: ");
        Serial.print("SSID:");
        Serial.println(wifi_cfg.ssid);
        Serial.print("Pasword:");
        Serial.println(wifi_cfg.pwd);
        Serial.print("ap Флаг: ");
        Serial.println(wifi_cfg.ap);
        #endif
      }
      configFile.close();
    }

    void save_wifi_cfg(String ssid, String pwd){
      #ifdef CFG_DEBUG
        Serial.println("Функция save_wifi_cfg конфигуратора запустилась");
        Serial.println("ssid = " + ssid);
        Serial.println("pwd = " + pwd);
      #endif  

      File configFile = FFat.open(wifi_cfg_path, "w");
      if (!configFile) {
        Serial.println("Failed to open config file for writing");
      }else{

        StaticJsonDocument<256> doc;
        // Set the values in the document

        
        doc["ssid"] = ssid;
        doc["pwd"] = pwd;
        doc["ap"] = "false";
        
        // Serialize JSON to file
        if (serializeJson(doc, configFile) == 0) {
          Serial.println(F("Failed to write to file"));
        }  

        configFile.close();
      }
    }

  private:
    
  // Пароль к течке доступа по умолчанию
    const char *ssid = "i-ESP32";
    const char *password = "11223344";

    // Конфигурация подключения WiFi МК
    String wifi_cfg_path = "/src/assets/base_cfg/wifi_cfg.json";
    struct Connection_wifi_cfg {
      String ssid;        // Срока, хранит значение SSID вайфай
      String pwd;         //строка, хранит значение пароля к вайфай
      boolean ap = true;  //true - режим точки доступа, false - режим подключения клиентом к вайфай
    };   
    Connection_wifi_cfg wifi_cfg;  // Конфигурация подключения WiFi МК

    // Конфигурация типов UI МК
    String ui_cfg_path = "/src/assets/base_cfg/ui_cfg.json";
    struct Connection_ui_cfg {
      boolean button;         // Взаимодействие через кнопки 
      boolean web;            // Взаимодействие через веб-сервер
      boolean mqtt;           // Взаимодействие через MQTT сервер
      boolean screen;         // Взаимодействие через экран
      boolean ap;             // Взаимодействие через приложение на телефоне Android, iOS
    };   
    Connection_ui_cfg ui_cfg;  // Конфигурация типов UI МК   

    
};