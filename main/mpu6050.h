#ifndef MPU6050_H
#define MPU6050_H

float gyroX, gyroY, gyroZ;
float accX, accY, accZ;
float temperature;


//Gyroscope sensor deviation
float gyroXerror = 0.07;
float gyroYerror = 0.03;
float gyroZerror = 0.01;



void initMPU();


#endif