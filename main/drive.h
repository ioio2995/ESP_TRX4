#ifndef DRIVE_H
#define DRIVE_H

// Driver conf
int pwmFreq     = 333;
int pulseMin    = 800;
int pulseMax    = 2200;

// Controller conf
int nullRange   = 10;

// Functions
void initPCA();


#endif