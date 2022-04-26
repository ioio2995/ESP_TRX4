#include <Arduino.h>
#include "global.h"
#include "controller.h"
#include "lcd.h"
#include "drive.h"
#include "bus.h"
#include "network.h"
#include "ota.h"

void setup()
{
  Serial.begin(115200);
  initI2C();
  initLCD();
  initPCA();
  //initWiFi();
  //initOTA();
  initPS4();
  initServo();
  Car_LCD();
  delay(10);
}

void loop()
{
  //handleOTA();
  Vbat_LCD();
  infoDualshock();
  control_esc();
  control_steering();
  control_trx4();
}