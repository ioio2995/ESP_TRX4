#include <Arduino.h>
#include <Arduino_JSON.h>
#include <Wire.h> 
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <PS4Controller.h>
#include "SPIFFS.h"
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Wifi network credentials
const char* ssid = "user-iobois";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;  
unsigned long lastTimeTemperature = 0;
unsigned long lastTimeAcc = 0;
unsigned long gyroDelay = 10;
unsigned long temperatureDelay = 1000;
unsigned long accelerometerDelay = 200;

// Create a sensor object
Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
float gyroX, gyroY, gyroZ;
float accX, accY, accZ;
float temperature;

// Create a driver object
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver();

// Driver conf
int pwmFreq 333
int pulseMin 800
int pulseMax 2200
int nullRange 10
// Driver Pin

// Electronic Speed Controler
#define ESCRESOL 0.5
#define ESC 0

// Steering servo
#define SSRESOL 0.42
#define SSOFFSET -0.08
#define STEERING 1

// Transmission servo
#define TSRESOL 0.5
#define TRANSMISSION 2

//T-Lock servos
#define TLSRESOL 0.5
#define TLSOFFSET 0
#define TLOCKFRONT 3
#define TLOCKREAR 4


//Gyroscope sensor deviation
float gyroXerror = 0.07;
float gyroYerror = 0.03;
float gyroZerror = 0.01;

// Init PCA9685
void initPCA(){
  pca.setPWMFreq(pwmFreq);
  if (!pca.begin()) {
    Serial.println("Failed to find PCA9685 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("PCA9685 Found!");
}

// Init MPU6050
void initMPU(){
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
}

// Init FS
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
}

String getGyroReadings(){
  mpu.getEvent(&a, &g, &temp);

  float gyroX_temp = g.gyro.x;
  if(abs(gyroX_temp) > gyroXerror)  {
    gyroX += gyroX_temp/50.00;
  }
  
  float gyroY_temp = g.gyro.y;
  if(abs(gyroY_temp) > gyroYerror) {
    gyroY += gyroY_temp/70.00;
  }

  float gyroZ_temp = g.gyro.z;
  if(abs(gyroZ_temp) > gyroZerror) {
    gyroZ += gyroZ_temp/90.00;
  }

  readings["gyroX"] = String(gyroX);
  readings["gyroY"] = String(gyroY);
  readings["gyroZ"] = String(gyroZ);

  String jsonString = JSON.stringify(readings);
  return jsonString;
}

String getAccReadings() {
  mpu.getEvent(&a, &g, &temp);
  // Get current acceleration values
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;
  readings["accX"] = String(accX);
  readings["accY"] = String(accY);
  readings["accZ"] = String(accZ);
  String accString = JSON.stringify (readings);
  return accString;
}

String getTemperature(){
  mpu.getEvent(&a, &g, &temp);
  temperature = temp.temperature;
  return String(temperature);
}


// WIRE
#define I2C_SDA 18
#define I2C_SCL 23

unsigned long previousMillis = 0;


int r = 255;
int g = 0;
int b = 0;

int generic_value(int position, float resolution, float offset=0, int inverse=0);

void setup() {
  Serial.begin(115200);
  PS4.begin("b8:27:eb:bc:a8:67");
  Serial.println("Initialization ready!");
  Wire.begin(I2C_SDA, I2C_SCL);
  
  Serial.println("Initializing PCA9685 Board");
   // fréquence de 50 Hz pour les servomoteurs
  Serial.println("Initializing Servos position");
  pwm.setPWM(ESC, 0, generic_value(0, ESCRESOL));
  pwm.setPWM(STEERING, 0, generic_value(0, SSRESOL, SSOFFSET));
  pwm.setPWM(TRANSMISSION, 0, generic_value(128, TSRESOL));
  pwm.setPWM(TLOCKFRONT, 0, generic_value(128, TLSRESOL));
  pwm.setPWM(TLOCKREAR, 0, generic_value(-128, TLSRESOL));
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

int generic_value(int position, float resolution, float offset, int inverse)
{ 
  int pulse_wide, analog_value, pulse_min, pulse_max, pulse_range, pulse_offset;
  if ( position < NULLRANGE && position > -NULLRANGE ) 
    position = 0 ; 
  if (inverse = 1) 
    position = -position;
  pulse_range = (PULSEMAX - PULSEMIN);
  pulse_offset = (pulse_range * offset);
  pulse_min = int((PULSEMIN + pulse_offset) + ((pulse_range * resolution)/2));
  pulse_max = int((PULSEMAX + pulse_offset) - ((pulse_range * resolution)/2));
  pulse_wide   = map(position, -128, 128, pulse_min, pulse_max);
  analog_value = int(float(pulse_wide) / 1000000 * PWMFREQ * 4096);
  return analog_value ;
}