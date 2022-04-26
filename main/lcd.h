#ifndef LCD_H
#define LCD_H

//LCD Init
void initLCD();

void Tlock_LCD(
    int position,
    bool status
    );
void Trans_LCD(
    int speed
    );
void Car_LCD();
void Vbat_LCD ();
void print_LCD(
    const char * text,
    int column = 0, 
    int line = 0
    );
#endif