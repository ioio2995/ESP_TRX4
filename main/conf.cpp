#include <Arduino.h>
#include "conf.h"
#include <Wire.h> 


void initI2C(){
  if (!Wire.begin(i2cSdaPin, i2cSclPin)) {
    Serial.println("Failed to init i2c");
    while (1) {
      delay(10);
    }
  }
  Serial.println("i2c init!");
}