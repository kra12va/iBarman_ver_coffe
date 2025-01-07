void FFat_file_system_init(){
  #ifdef CFG_DEBUG
  Serial.print(F("Инициализируем файловую система FAT..."));
  #endif
  if (FFat.begin(true)){
      Serial.println(F("Успешно"));
  }else{
      Serial.println(F("Ошибка"));
      while (true) { delay(1000); };
  }
}    




/*

boolean save_wifi_config(String ssid, String pwd) {
  File configFile = FFat.open(wifi_cfg_path, "w");
  if (!configFile) {
  //Serial.println(“Failed to open config file for writing”);
  return false;
  }
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
  return true;
  
}

*/

