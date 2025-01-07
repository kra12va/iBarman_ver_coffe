    void wifi_AP_DEFAULT_run() {
      #ifdef CFG_DEBUG
      Serial.println("Функция wifi_AP_DEFAULT_run конфигуратора отработала...");
      #endif
      // You can remove the password parameter if you want the AP to be open.
      // a valid password must have more than 7 characters
      if (!WiFi.softAP(ssid, password)) {
        log_e("Soft AP creation failed.");
        while (1);
      }
      IPAddress myIP = WiFi.softAPIP();
      #ifdef CFG_DEBUG
      Serial.print("AP IP адрес: ");
      Serial.println(myIP);
      Serial.println("Server запущен");      
      #endif
    } 

    void wifi_AP_run() {
      #ifdef CFG_DEBUG
      Serial.println("Функция wifi_AP_run конфигуратора отработала...");
      #endif
      // You can remove the password parameter if you want the AP to be open.
      // a valid password must have more than 7 characters
      if (!WiFi.softAP(wifi_cfg.ssid, wifi_cfg.pwd)) {
        log_e("Soft AP creation failed.");
        while (1);
      }
      IPAddress myIP = WiFi.softAPIP();
      #ifdef CFG_DEBUG
      Serial.print("AP IP адрес: ");
      Serial.println(myIP);
      Serial.println("Server запущен");      
      #endif
    }  


    void wifi_run() {

      int i=0;
      WiFi.begin(wifi_cfg.ssid, wifi_cfg.pwd);
      Serial.println("Подключаюсь к вайфай сети...");
      while ((WiFi.status() != WL_CONNECTED) && (i < 20)) {
        delay(500);
        Serial.print(".");
        i++;
      }
      if (i < 20){
        Serial.println("Подключились к вайфай сети: ");
        Serial.println(wifi_cfg.ssid);
        Serial.println("IP адрес устройства: ");
        Serial.println(WiFi.localIP());
      }else{
        Serial.println();
        Serial.println("Не удалось подключитсья к вайфай");
      }      
    }  