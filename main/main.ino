#include <Arduino.h>
#include "global.h"
#include "controller.h"
#include "drive.h"
#include "bus.h"
#include "network.h"
#include "ota.h"

void setup()
{
  Serial.begin(115200);
  initI2C();
  initPCA();
  initWiFi();
  initOTA();
  initPS4();
  initServo();
  delay(10);
}

void loop()
{
  handleOTA();
  infoDualshock();
  control_trx4();
}