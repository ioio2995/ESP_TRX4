#include <Arduino.h>
#include "controller.h"
#include "drive.h"
#include "global.h"
#include "lcd.h"

// Create a driver object
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver();

// Driver conf
const int pulseMin = 800;
const int pulseMax = 2200;
const int pwmFreq = 100;
const int OscillatorFrequency = 24000000; //26100000;

// Servo Pin on PCA-9685
const int escPin = 0;
const int steeringPin = 1;
const int transmissionPin = 2;
const int tlockfrontPin = 3;
const int tlockrearPin = 4;
// Servo Resolution
float escResol = 0.75;
float steeringResol = 0.42;
float transmissionResol = 0.5;
float tlockResol = 0.5;
// Servo Offset
float steeringOffset = 0 ; // -0.08;

// Controller range for limit pressision
int nullRange = 10;

// Convert Contoller value to PWM Value
int convert_value(int position, float resolution, float offset, int inverse)
{
  int pulse_wide, analog_value, pulse_min, pulse_max, pulse_range, pulse_offset;
  if (position < nullRange && position > -nullRange)
    position = 0;
  if (inverse = 1)
    position = -position;
  pulse_range = (pulseMax - pulseMin);
  pulse_offset = (pulse_range * offset);
  pulse_min = int((pulseMin + pulse_offset) + ((pulse_range * resolution) / 2));
  pulse_max = int((pulseMax + pulse_offset) - ((pulse_range * resolution) / 2));
  pulse_wide = map(position, -128, 128, pulse_min, pulse_max);
  analog_value = int(float(pulse_wide) / 1000000 * pwmFreq * 4096);
  return analog_value;
}

// Init PCA9685
void initPCA()
{
  pca.begin();
  pca.setOscillatorFrequency(OscillatorFrequency);
  pca.setPWMFreq(pwmFreq);
  Serial.println("PCA9685 Found!");
}

//Init Servo Position
void initServo()
{
  pca.setPWM(escPin, 0, convert_value(0, escResol));
  pca.setPWM(steeringPin, 0, convert_value(0, steeringResol, steeringOffset));
  pca.setPWM(transmissionPin, 0, convert_value(-128, transmissionResol));
  Trans_LCD(1);
  pca.setPWM(tlockfrontPin, 0, convert_value(128, tlockResol));
  Tlock_LCD(1,true);
  pca.setPWM(tlockrearPin, 0, convert_value(-128, tlockResol));
  Tlock_LCD(0,true);
  Serial.println("Initializing Servos position finish");
}

//ESC Calibrate
void calibESC()
{
  pca.setPWM(escPin, 0, convert_value(0, 0));
  delay(1000);
  pca.setPWM(escPin, 0, convert_value(128, 0));
  delay(1000);
  pca.setPWM(escPin, 0, convert_value(-128, 0));
  delay(1000);
  pca.setPWM(escPin, 0, convert_value(0, 0));
}

//ESC control
void control_esc()
{
  int escPwm = convert_value(0, escResol);
  if (PS4.isConnected())
  {
    // Speed control
    if (PS4.event.analog_move.button.l2 || PS4.event.analog_move.button.r2 )
    {
      float escResolMod = escResol;
      int escPower;
      if (PS4.data.button.l1)
        escResolMod = 0.50;
      if (PS4.data.button.r1)
        escResolMod = 0.87;
      if ( PS4.data.button.l2 &&  ! PS4.data.button.r2 ) 
      {
        escPower = (((float)PS4.data.analog.button.l2 /255.0 ) *128.0);
        escPwm = convert_value(escPower, escResolMod);
      }
      if ( PS4.data.button.r2 &&  ! PS4.data.button.l2 ) 
      {
        escPower = -(((float)PS4.data.analog.button.r2 /255.0 ) *128.0);
        escPwm = convert_value(escPower, escResolMod);
      }
    }
  }
  pca.setPWM(escPin, 0, escPwm);
}

//Steering control
void control_steering()
{
  int steeringPwm = convert_value(0, steeringResol, steeringOffset, 1);
  if (PS4.isConnected())
  {
    // Steering control
    if (PS4.event.analog_move.stick.lx)
      steeringPwm = convert_value(PS4.data.analog.stick.lx, steeringResol, steeringOffset, 1);
  }
  pca.setPWM(steeringPin, 0, steeringPwm);
}

//TRX4 control
void control_trx4()
{
  if (PS4.isConnected())
  {
    // Transmission control
    if (PS4.data.button.up)
    {
      pca.setPWM(transmissionPin, 0, convert_value(-128, transmissionResol));
      Trans_LCD(1);
    }
    if (PS4.data.button.down)
    {
      pca.setPWM(transmissionPin, 0, convert_value(128, transmissionResol));
      Trans_LCD(2);
    }
    // T-Lock control
    if (PS4.data.button.triangle)
    {
      pca.setPWM(tlockfrontPin, 0, convert_value(-128, tlockResol));
      Tlock_LCD(0,false);
      pca.setPWM(tlockrearPin, 0, convert_value(128, tlockResol));
      Tlock_LCD(1,false);
    } 
    if (PS4.data.button.cross)
    {
      pca.setPWM(tlockfrontPin, 0, convert_value(128, tlockResol));
      Tlock_LCD(0,true);
      pca.setPWM(tlockrearPin, 0, convert_value(-128, tlockResol));
      Tlock_LCD(1,true);
    }  
  }
}