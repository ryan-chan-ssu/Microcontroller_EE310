#include <stdbool.h> 
#include "lcd.h"

extern bool correct;
extern int count;
extern int password;
extern int secretPassword;

void initialization(void) {
    PORTA = 0;  LATA = 0;  ANSELA = 0;
    TRISA = 0b00000111;  WPUA = 0b00000100;

    PORTB = 0;  LATB = 0;  ANSELB = 0;
    TRISB = 0b00000011;  WPUB = 0;

    PORTC = 0;  LATC = 0;  ANSELC = 0;
    TRISCbits.TRISC2 = 0;  // RS
    TRISCbits.TRISC3 = 0;  // EN
    TRISCbits.TRISC4 = 0;  // SYS_LED
    LATCbits.LATC4 = 1;    // Turn on SYS_LED

    PORTD = 0;  LATD = 0;  ANSELD = 0;
    TRISD = 0x00;          // LCD Data D0?D7

    INTCON0bits.INT0EDG = 1;
    INTCON0bits.IPEN = 1;
    INTCON0bits.GIEH = 1;
    INTCON0bits.GIEL = 1;
    IPR1bits.INT0IP = 1;
    PIE1bits.INT0IE = 1;
    PIR1bits.INT0IF = 0;

    IVTBASEU = 0x00;
    IVTBASEH = 0x30;
    IVTBASEL = 0x08;

    LCD_Init();
    LCD_Clear();
}