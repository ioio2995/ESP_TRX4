#ifndef DRIVE_H
#define DRIVE_H

// Include Adafruit library
#include <Adafruit_PWMServoDriver.h>

// Functions
void initPCA();
void calibESC();
void initServo();
void control_trx4();
int convert_value(
    int position, 
    float resolution, 
    float offset=0, 
    int inverse=0
    );

#endif