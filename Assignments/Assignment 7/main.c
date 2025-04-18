//------------------------------------------
// Title: Secret Box with Touchless Switches
//------------------------------------------
// Purpose: Design a safebox with two touchless switches (photoresistors) that allow
//          a user to enter a secret code to unlock a motor or trigger a buzzer.
//          The system displays input feedback and result messages on an LCD screen.
//          A SYS_LED remains on when the system is powered, and an emergency interrupt
//          plays a melody via a buzzer.
//
// Dependencies: 
// - stdbool.h - Boolean support
// - xc.h - XC8 compiler standard
// - config.h - MCU configuration bits
// - initialization.h - I/O setup and variable initialization
// - functions.h - Input, password logic, and ISR
// - lcd.h - LCD macros and function declarations
//
// Compiler: XC8 v3.00
// Author: Ryan Chan
//
// Inputs:
// - Two photoresistors (PR1, PR2) as touchless switches (RA0, RA1)
// - Button 1 (RB1) for confirming input
// - Button 2 (RB0) as emergency interrupt
//
// Outputs:
// - LCD display (RD0?RD7 data, RC2 = RS, RC3 = EN)
// - DC motor (RA5) via transistor and relay
// - Buzzer (RB2) for wrong code or melody on interrupt
// - SYS_LED (RC4) stays on while powered
//
// Hardware: PIC18F47K42 microcontroller, 16x2 LCD, 2x photoresistors, 2x push buttons, 
// 1x DC motor, 1x buzzer, 1x relay module with NPN driver, 1x SYS_LED
// Versions: First version

// main.c
#include <stdbool.h>
#include <xc.h>
#include "config.h"
#include "initialization.h"
#include "functions.h"

int count = 0;
int password = 0;
int secretPassword = 21;
bool correct = false;
 
void main(void)  
{ 
    initialization();
        
    while(1) {
        updateLCD();

        while(correct == false)
            passwordOneInput();

        __delay_ms(250);
        password = count * 10;
        correct = false;
        count = 0;
        updateLCD();

        while(correct == false)
            passwordTwoInput();

        __delay_ms(250);
        password += count;
        passwordCheck();
        break;
    }
    LCD_Clear();
    LCD_String_xy(1, 0, "Start Over");
    __delay_ms(1500);
    Sleep();
}
