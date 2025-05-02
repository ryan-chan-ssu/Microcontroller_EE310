//------------------------------------------
// Title: Light-Sensing LCD Display with PWM Control and Interrupt Mode
//------------------------------------------
// Purpose:
// - Read light intensity using a photoresistor and display the lux value on an LCD
// - Provide PWM control via a button that increases duty cycle by 10% per press
// - Trigger a 10-second interrupt mode with LED blinking using a second button
//
// Dependencies: 
// - stdbool.h – Boolean support
// - xc.h – XC8 compiler standard
// - config.h – MCU configuration bits
// - lcd.h – LCD macros and function declarations
// - pwm.h – PWM initialization and control
//
// Compiler: XC8 v3.00
// Author: Ryan Chan
//
// Inputs:
// - Light sensor (photoresistor) on RA0 (analog input)
// - PWM control button on RB0
// - Interrupt trigger button on RB1
//
// Outputs:
// - LCD display (RD0–RD7 data lines, RC5 = RS, RC6 = EN)
// - Interrupt status LED on RB2 (blinks during 10-second countdown)
// - PWM output signal on RB3
//
// Hardware:
// - PIC18F47K42 microcontroller
// - 16x2 LCD display
// - Two push buttons (one for PWM, one for interrupt)
// - One photoresistor with analog input
// - LEDs for visual feedback
// - External pull-down resistors for button inputs
//
// Version: 2.0


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "config.h"
#include "lcd.h"
#include "pwm.h"

int digital;
float voltage;
float lux;
char data[16];
volatile unsigned char interrupt_flag = 0;

uint8_t duty_percent = 10;
uint16_t duty_raw = 0;

#define RED_LED LATBbits.LATB2

void ADC_Init(void);
void Interrupt_Init(void);

void __interrupt(irq(7), base(0x4008)) IOC_ISR(void) {
    if (IOCBFbits.IOCBF1) {
        interrupt_flag = 1;
        IOCBFbits.IOCBF1 = 0;
    }
}

void main(void) {
    ADC_Init();
    LCD_Init();
    Interrupt_Init();

    PWM_Output_D8_Enable();
    TMR2_Initialize();
    PWM2_Initialize();
    TMR2_Start();
    
    static bool prev_btn_state = 0;

    while (1) {
        if (interrupt_flag) {
            LCD_Clear();
            LCD_String_xy(1, 0, "INTERRUPTED");

            for (int i = 10; i > 0; i--) {  // 20 × 500ms = 10 seconds
                char countdown[16];
                sprintf(countdown, "Countdown:%2d s", i);
                LCD_String_xy(2, 0, countdown);
                
                RED_LED = 1;
                __delay_ms(500);
                
                RED_LED = 0;
                __delay_ms(500);
            }
            interrupt_flag = 0;
            //LCD_Clear();  // Clear LCD after wait (optional)
        } else {
            ADCON0bits.GO = 1;             // Start ADC conversion
            while (ADCON0bits.GO);         // Wait until done

            digital = (ADRESH << 8) | ADRESL;           // 10-bit result
            voltage = digital * (Vref / 4096.0);        // Convert to voltage
             lux = (1.0 - (voltage / Vref)) * 2250.0;    // Estimate lux

            LCD_Clear();
            sprintf(data, "Light: %.0f Lux", lux);
            LCD_String_xy(1, 0, data);
//            LCD_String_xy(1, 0, "Welcome Ryan C");
//            LCD_String_xy(2,0,"EE310@SSU");

            sprintf(data, "PWM: %u%%", duty_percent);
            LCD_String_xy(2, 0, data);

            __delay_ms(800);  // LCD update delay
        }
        
        // PWM Button on RB0: increase 10% per press
        bool curr_btn_state = PORTBbits.RB0;
        if (curr_btn_state && !prev_btn_state) {
            LATBbits.LATB3 = 1;
            duty_percent += 10;
            if (duty_percent > 100) duty_percent = 0;
            duty_raw = (uint16_t)((duty_percent * 1023UL) / 100);
            PWM2_LoadDutyValue(duty_raw);
        }
        prev_btn_state = curr_btn_state;
        LATBbits.LATB3 = 0;
    }
}

void ADC_Init(void) {
    TRISAbits.TRISA0 = 1;         // RA0/AN0 input
    ANSELAbits.ANSELA0 = 1;       // Analog enabled

    ADPCH = 0x00;                 // Select AN0
    ADCON0bits.FM = 1;            // Right justified
    ADCON0bits.CS = 1;            // Internal oscillator
    ADCLK = 0x00;
    ADPREL = 0x00;
    ADPREH = 0x00;
    ADACQ = 0x00;
    ADRESL = 0;
    ADRESH = 0;
    ADCON0bits.ON = 1;            // Enable ADC
}

void Interrupt_Init(void) {
    TRISBbits.TRISB1 = 1;         // RB1 as input
    ANSELBbits.ANSELB1 = 0;       // Digital mode
    IOCBPbits.IOCBP1 = 1;         // Interrupt on rising edge
    IOCBNbits.IOCBN1 = 0;
    IOCBFbits.IOCBF1 = 0;         // Clear flag

    PIE0bits.IOCIE = 1;           // Enable IOC interrupt
    PIR0bits.IOCIF = 0;
    INTCON0bits.GIE = 1;          // Global interrupt enable
    INTCON0bits.IPEN = 0;         // Disable priority

    TRISBbits.TRISB2 = 0;         // RB2 as output (RED LED)
    LATBbits.LATB2 = 0;
    
    TRISBbits.TRISB3 = 0;         // RB3 as output for PWM LED
    LATBbits.LATB3 = 0;           // Start OFF

    TRISBbits.TRISB0 = 1;         // RB0 as input for PWM button
    ANSELBbits.ANSELB0 = 0;       // Digital mode
    WPUBbits.WPUB0 = 0;           // Disable internal pull-up (using pull-down externally)
    INLVLBbits.INLVLB0 = 1;       // TTL input level
}
