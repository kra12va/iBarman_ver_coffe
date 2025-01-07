void HTTP_init(){
  HTTP.onNotFound([](){                                                 // Описываем действия при событии "Не найдено"
  if(!handleFileRead(HTTP.uri()))                                       // Если функция handleFileRead (описана ниже) возвращает значение false в ответ на поиск файла в файловой системе
      HTTP.send(404, "text/plain", "Страница не найдена");                        // возвращаем на запрос текстовое сообщение "File isn't found" с кодом 404 (не найдено)
  });  
    
  HTTP.on("/wifi_save", handle_wifi_save);     //
  HTTP.on("/wifiscan", handle_wifiscan);

  //          ФУНКЦИИ ДЛЯ КОНКРЕТНЫХ ПРОЕКТОВ
  HTTP.on("/make_cocktail", handle_make_cocktail);

  //      КОНЕЦ    ФУНКЦИИ ДЛЯ КОНКРЕТНЫХ ПРОЕКТОВ


  HTTP.begin();

  #ifdef DEBUG
  Serial.println("HTTP server запущен");  
  #endif
}

//          ФУНКЦИИ ДЛЯ КОНКРЕТНЫХ ПРОЕКТОВ

void handle_make_cocktail() {
   make_cocktail(HTTP.arg("cocktail"));
}


//       КОНЕЦ   ФУНКЦИИ ДЛЯ КОНКРЕТНЫХ ПРОЕКТОВ

void handle_wifi_save() {

  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
  #ifdef DEBUG
  Serial.println("Получены параметры: ");
  Serial.println("SSID: " + HTTP.arg("ssid"));
  Serial.println("PWD: " + HTTP.arg("pwd"));
  #endif

  MK.save_wifi_cfg(HTTP.arg("ssid"), HTTP.arg("pwd"));
  
  FFat.end();
  delay(1000);
  #ifdef DEBUG
  Serial.println("Перезагружаем ESP...");
  #endif
  ESP.restart();
  
}

void handle_wifiscan() {
  StaticJsonDocument<1024> jsonDocument;
  String json_str;
  
  String ssid;
  int32_t rssi;
  uint8_t encryptionType;
  uint8_t* bssid;
  int32_t channel;
  bool hidden;
  int scanResult;


    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    #ifdef DEBUG
    Serial.println("Сканируем вайфай сети");
    #endif
    if (n == 0) {
        #ifdef DEBUG
        Serial.println("Вайфай Сети НЕ найдены");
        #endif
    } else {
        #ifdef DEBUG
        Serial.println("Вайфай Сети найдены");
        #endif
        for (int i = 0; i < n; ++i) {
            //WiFi.SSID(i).c_str();
            jsonDocument[i]["ssid"] = WiFi.SSID(i);
            jsonDocument[i]["level"] = WiFi.RSSI(i);
            jsonDocument[i]["mac_adress"] = WiFi.BSSIDstr(i);
        }
        serializeJsonPretty(jsonDocument, json_str);
    }

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();

  HTTP.send(200, "text/plain", json_str);   // отправляем ответ о выполнении
}



bool handleFileRead(String path){                                       // Функция работы с файловой системой
  if(path.endsWith("/")) path += "index.html";                          // Если устройство вызывается по корневому адресу, то должен вызываться файл index.html (добавляем его в конец адреса)
  String contentType = getContentType(path);                            // С помощью функции getContentType (описана ниже) определяем по типу файла (в адресе обращения) какой заголовок необходимо возвращать по его вызову
  if(FFat.exists(path)){                                              // Если в файловой системе существует файл по адресу обращения
    File file = FFat.open(path, "r");                                 //  Открываем файл для чтения
    size_t sent = HTTP.streamFile(file, contentType);                   //  Выводим содержимое файла по HTTP, указывая заголовок типа содержимого contentType
    file.close();                                                       //  Закрываем файл
    return true;                                                        //  Завершаем выполнение функции, возвращая результатом ее исполнения true (истина)
  }
  return false;                                                         // Завершаем выполнение функции, возвращая результатом ее исполнения false (если не обработалось предыдущее условие)
}



String getContentType(String filename){                                 // Функция, возвращающая необходимый заголовок типа содержимого в зависимости от расширения файла
  if (filename.endsWith(".html")) return "text/html";                   // Если файл заканчивается на ".html", то возвращаем заголовок "text/html" и завершаем выполнение функции
  else if (filename.endsWith(".css")) return "text/css";                // Если файл заканчивается на ".css", то возвращаем заголовок "text/css" и завершаем выполнение функции
  else if (filename.endsWith(".js")) return "application/javascript";   // Если файл заканчивается на ".js", то возвращаем заголовок "application/javascript" и завершаем выполнение функции
  else if (filename.endsWith(".png")) return "image/png";               // Если файл заканчивается на ".png", то возвращаем заголовок "image/png" и завершаем выполнение функции
  else if (filename.endsWith(".jpg")) return "image/jpeg";              // Если файл заканчивается на ".jpg", то возвращаем заголовок "image/jpg" и завершаем выполнение функции
  else if (filename.endsWith(".gif")) return "image/gif";               // Если файл заканчивается на ".gif", то возвращаем заголовок "image/gif" и завершаем выполнение функции
  else if (filename.endsWith(".ico")) return "image/x-icon";            // Если файл заканчивается на ".ico", то возвращаем заголовок "image/x-icon" и завершаем выполнение функции
  return "text/plain";                                                  // Если ни один из типов файла не совпал, то считаем что содержимое файла текстовое, отдаем соответствующий заголовок и завершаем выполнение функции
}
