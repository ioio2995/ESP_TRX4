#include <Arduino.h>

#include "network.h"

unsigned long previousMillis = 0;


int r = 255;
int g = 0;
int b = 0;

int generic_value(int position, float resolution, float offset=0, int inverse=0);

void setup() {
  Serial.begin(115200);

  Serial.println("Initializing PCA9685 Board");
   // fréquence de 50 Hz pour les servomoteurs
  Serial.println("Initializing Servos position");
  pwm.setPWM(escPin, 0, generic_value(0, escResol));
  pwm.setPWM(steeringPin, 0, generic_value(0, steeringResol, steeringOffset));
  pwm.setPWM(transmissionPin, 0, generic_value(128, transmissionResol));
  pwm.setPWM(tlockfrontPin, 0, generic_value(128, tlockResol));
  pwm.setPWM(tlockrearPin, 0, generic_value(-128, tlockResol));
  delay(10);
}

void loop() {
  if (PS4.isConnected()){
    PS4.setLed(137, 178, 250);
    
    // Sends data set in the above three instructions to the controller
    PS4.sendToController();

    if (PS4.Options()) Serial.printf("Battery Level : %d\n", PS4.Battery());
    if ( PS4.LStickX() ) 
    pwm.setPWM(STEERING, 0, generic_value(PS4.LStickX(), SSRESOL, SSOFFSET,1));
    if (PS4.LStickY()) {
    float  escpuis = ESCRESOL
    if ( PS4.L1() ) escpuis = 0.75;
    if ( PS4.R1() ) escpuis = 0.25;
    if ( PS4.L2Value() > 10 ) {
        float resol = ((PS4.L2Value() * escpuis) / 256);
    }
    pwm.setPWM(ESC, 0, generic_value(PS4.LStickY(), resol));
    }
    if (PS4.Triangle()) pwm.setPWM(TLOCKFRONT, 0, generic_value(-128, TLSRESOL));
    if (PS4.Circle()) pwm.setPWM(TLOCKFRONT, 0, generic_value(128, TLSRESOL));
    if (PS4.Cross()) pwm.setPWM(TLOCKREAR, 0, generic_value(128, TLSRESOL));
    if (PS4.Square()) pwm.setPWM(TLOCKREAR, 0, generic_value(-128, TLSRESOL));
    // Don't send data to the controller immediately, will cause buffer overflow 
    delay(10);
  }
}