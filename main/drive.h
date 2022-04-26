#ifndef DRIVE_H
#define DRIVE_H

// Include Adafruit library
#include <Adafruit_PWMServoDriver.h>

// Functions
void initPCA();
void calibESC();
void initServo();
void control_esc();
void control_steering();
void control_trx4();
float get_voltage();
int convert_value(
    int position, 
    float resolution, 
    float offset=0, 
    int inverse=0
    );

#endif