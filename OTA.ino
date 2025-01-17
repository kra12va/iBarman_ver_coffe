 //инициируем прошивку по воздуху
void ElegantOTA_init(){
  //инициируем прошивку по воздуху
  ElegantOTA.begin(&HTTP);    // Start ElegantOTA
  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);  
  Serial.println("Инициализация прошивки по воздуху - OK");
}

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
    delay(1000);
    ESP.restart();
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}