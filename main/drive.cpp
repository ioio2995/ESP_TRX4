#include <Arduino.h>
#include "conf.h"
#include "drive.h"

// Include Adafruit library and P4Controller
#include <Adafruit_PWMServoDriver.h>
#include <PS4Controller.h>

// Create a driver object
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver();


// Init PS4 Controler
void initPS4(){
  if (!PS4.begin(ps4Mac)) {
    Serial.println("Failed to find PS4 Controler");
    while (1) {
      delay(10);
    }
  }
  Serial.println("PS4 Controler Found!");
}

// Init PCA9685
void initPCA(){
  pca.begin();
  pca.setPWMFreq(pwmFreq);
  Serial.println("PCA9685 Found!");
}

//Init Servo Position
void initServo(){
  pca.setPWM(escPin, 0, generic_value(0, escResol));
  pca.setPWM(steeringPin, 0, generic_value(0, steeringResol, steeringOffset));
  pca.setPWM(transmissionPin, 0, generic_value(128, transmissionResol));
  pca.setPWM(tlockfrontPin, 0, generic_value(128, tlockResol));
  pca.setPWM(tlockrearPin, 0, generic_value(-128, tlockResol));
  Serial.println("Initializing Servos position finish");
}


int generic_value(int position, float resolution, float offset, int inverse){ 
  int pulse_wide, analog_value, pulse_min, pulse_max, pulse_range, pulse_offset;
  if ( position < nullRange && position > -nullRange ) 
    position = 0 ; 
  if (inverse = 1) 
    position = -position;
  pulse_range = (pulseMax - pulseMin);
  pulse_offset = (pulse_range * offset);
  pulse_min = int((pulseMin + pulse_offset) + ((pulse_range * resolution)/2));
  pulse_max = int((pulseMax + pulse_offset) - ((pulse_range * resolution)/2));
  pulse_wide   = map(position, -128, 128, pulse_min, pulse_max);
  analog_value = int(float(pulse_wide) / 1000000 * pwmFreq * 4096);
  return analog_value ;
}
