#include <Arduino.h>
#include <Wire.h>
#include "ota.h"
#include <ArduinoOTA.h>

// INITIALISATION Arduino OTA
// **************************
void initOTA() {
  /* Port par defaut = 8266 */
  // ArduinoOTA.setPort(8266);

  /* Hostname, par defaut = esp8266-[ChipID] */
  ArduinoOTA.setHostname("TRX4");

  /* Pas d'authentication par defaut */
  // ArduinoOTA.setPassword("admin");

  /* Le mot de passe peut également être défini avec sa valeur md5 */
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  
  // code à exécuter au démarrage de la mise à jour
  ArduinoOTA.onStart([]() {
    Serial.println("\nStart updating ");
  });
  
  // code à exécuter à la fin de la mise à jour
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  
  // code à exécuter pendant la mise à jour
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress : %u%%\r", (progress / (total / 100)));
  });
  
  // code à exécuter en cas d'erreur de la mise à jour
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Erreur[%u] : ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready");
}

void handleOTA(){
ArduinoOTA.handle();
}