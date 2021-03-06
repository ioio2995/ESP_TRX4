#include <Arduino.h>
#include <Wire.h>
#include "bus.h"

#define i2cSdaPin 18
#define i2cSclPin 23

void initI2C(){
  if (!Wire.begin(i2cSdaPin, i2cSclPin)) {
    Serial.println("Failed to init i2c");
    while (1) {
      delay(10);
    }
  }
  Serial.println("i2c init!");
}