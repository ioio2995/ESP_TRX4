#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "global.h"
#include "lcd.h"

const int rowsLCD = 4;
const int columnsLCD = 20;
LiquidCrystal_I2C lcd(0x27, rowsLCD, columnsLCD);

byte Tlock_on[8] = {
    0b00100,
    0b00100,
    0b01110,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00100
};
byte Tlock_off[8] = {
    0b00100,
    0b00100,
    0b01110,
    0b10001,
    0b10001,
    0b01110,
    0b00100,
    0b00100
};

 byte Wheel_Right[8] = {
        B00000,
        B00000,
        B00000,
        B00000,
        B11111,
        B11111,
        B11111,
        B00100
    };
    byte Wheel_Letf[8] = {
        B00100,
        B11111,
        B11111,
        B11111,
        B00000,
        B00000,
        B00000,
        B00000
    };
    byte Transmission[8] = {
        B00000,
        B00000,
        B00000,
        B11111,
        B11111,
        B00000,
        B00000,
        B00000
    };
    byte Speed[8] = {
        B00000,
        B01000,
        B01010,
        B11111,
        B11111,
        B01010,
        B01000,
        B00000
    };

// Init LCD Screen
void initLCD()
{
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, Wheel_Right);
    lcd.createChar(1, Wheel_Letf);
    lcd.createChar(2, Transmission);
    lcd.createChar(3, Tlock_on); // create a new custom character
    lcd.createChar(4, Tlock_off); // create a new custom character
    lcd.createChar(5, Speed);
}

void Tlock_LCD(int position, bool status)
{
    if (position == 1 )
    {
        if (status == true)
        {
            lcd.setCursor(14, 1); // move cursor to (2, 0)
            lcd.write((byte)3);  // print the custom char at (2, 0)  
        }
        else
        {
            lcd.setCursor(14, 1); // move cursor to (2, 0)
            lcd.write((byte)4);  // print the custom char at (2, 0) 
        }
    }

    if (position == 0)
    {
        if (status == true)
        {
            lcd.setCursor(18, 1); // move cursor to (2, 0)
            lcd.write((byte)3);  // print the custom char at (2, 0)  
        }
        else
        {
            lcd.setCursor(18, 1); // move cursor to (2, 0)
            lcd.write((byte)4);  // print the custom char at (2, 0) 
        }
    }
}

void Trans_LCD(int speed)
{
    if (speed == 1 )
    {
        lcd.setCursor(16, 2); // move cursor to (2, 0)
        lcd.print("L");
    }
    if (speed == 2 )
    {
        lcd.setCursor(16, 2); // move cursor to (2, 0)
        lcd.print("H");
    }  
}

void Car_LCD()
{
    lcd.clear();
    lcd.setCursor(14, 0);
    lcd.write((byte)0);
    lcd.setCursor(18, 0);
    lcd.write((byte)0);
    lcd.setCursor(15, 1); 
    lcd.write((byte)2);
    lcd.setCursor(16, 1);
    lcd.write((byte)5);
    lcd.setCursor(17, 1);
    lcd.write((byte)2);
    lcd.setCursor(14, 2);
    lcd.write((byte)1);
    lcd.setCursor(18, 2);
    lcd.write((byte)1);  
}

void Vbat_LCD ()
{
    lcd.setCursor(0, 0);
    lcd.print( "Vbat:");
    lcd.setCursor(6, 0);
    lcd.print(v_batt());
    lcd.setCursor(12, 0);
    lcd.print("V");
}

void print_LCD(const char * text, int column, int line)
{
  lcd.setCursor(column, line);
  for (byte i = 0; i < strlen(text); i++)
  {
    lcd.setCursor(i, line);
    lcd.print(" ");
  }
  lcd.setCursor(column, line);
  lcd.print(text);
}