#ifndef CONF_H
#define CONF_H
// Wifi network credentials
const char* ssid = "";
const char* password = "";

// PS4 Controler MAC address must be unicast
char* ps4Mac = "b8:27:eb:bc:a8:67";

// I2C Pin
int i2cSdaPin = 18; 
int i2cSclPin = 23;

// Servo Configuration 
// Servo Pin on PCA-9685
int escPin = 0;
int steeringPin = 1;
int transmissionPin = 2;
int tlockfrontPin = 3;
int tlockrearPin = 4;
// Servo Resolution
float escResol = 0.5;
float steeringResol = 0.42;
float transmissionResol = 0.5;
float tlockResol = 0.5;
// Servo Offset
float steeringOffset = -0.08;

// Timer variables
unsigned long lastTime = 0;  
unsigned long lastTimeTemperature = 0;
unsigned long lastTimeAcc = 0;
unsigned long gyroDelay = 10;
unsigned long temperatureDelay = 1000;
unsigned long accelerometerDelay = 200;

#endif