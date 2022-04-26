#include <Arduino.h>
#include "controller.h"
#include "global.h"
#include "lcd.h"

//Controler PS4
// Color RGB LED PS4
int rgbR = 137;
int rgbG = 178;
int rgbB = 250;

// Init PS4 Controler
void initPS4()
{
    PS4.begin(ps4Mac);
    Serial.println("");
    Serial.print("Connecting to PS4 Controller...");
    print_LCD("Waiting Controller..");
    byte i=0;
    while (!PS4.isConnected()) {
        Serial.print(".");
        print_LCD(".",i,1);
        if ( i == 3) {
            i=0;
            print_LCD("      ",i,1);
        } 
        i=++i;
        delay(1000);
    }
    PS4.setLed(rgbR, rgbG, rgbB);
    PS4.sendToController();
    Serial.println("PS4 Controler Found!");
    print_LCD("PS4 Controler Found!");
}

// Send Info PS4 Controler
void infoDualshock()
{
    if (PS4.isConnected())
    {
        if (PS4.data.button.options)
        {
            if (PS4.data.status.charging)
                Serial.println("The controller is charging");
            if (PS4.data.status.audio)
                Serial.println("The controller has headphones attached");
            if (PS4.data.status.mic)
                Serial.println("The controller has a mic attached");
            Serial.print("Battey = ");
            Serial.print(PS4.data.status.battery, DEC);
            Serial.println(" / 16");
        }
    }
}
