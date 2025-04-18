#include <stdbool.h> 
#include <stdio.h>
#include "lcd.h"
 
extern int count;
extern int password;
extern int secretPassword;
extern bool correct;

void updateLCD(void) {
    char buffer[16];
    sprintf(buffer, "Input: %d", count);
    LCD_Clear();
    LCD_String_xy(1, 0, buffer);
}
 
void passwordOneInput(void) //gets input 
{ 
    while(correct==false){ 
        if(PORTAbits.RA0==1){ 
            count++; 
            updateLCD(); 
            LCD_String_xy(2, 0, "First Digit");
            __delay_ms(500); 
        } 
        if(PORTBbits.RB1==1){ //leaves when button pressed  
            correct=true; 
        } 
        LCD_String_xy(2, 0, "First Digit");
    } 
    return; 
} 

void passwordTwoInput(void) //gets input 
{ 
    while(correct==false){ 
        if(PORTAbits.RA1==1){ 
            count++; 
            updateLCD();
            LCD_String_xy(2, 0, "Second Digit");
            __delay_ms(500); 
        } 
        if(PORTBbits.RB1==1){ //leaves when button pressed  
            correct=true; 
        } 
        LCD_String_xy(2, 0, "Second Digit");
    } 
    return; 
} 
 
void passwordCheck(void) {
    LCD_Clear();
    if(password == secretPassword) {
        LCD_String_xy(1, 0, "Access Granted");
        PORTAbits.RA5 = 1; // Turn on motor via relay
        __delay_ms(5000);
        PORTAbits.RA5 = 0;
    } else {
        LCD_String_xy(1, 0, "Access Denied");
        PORTBbits.RB2 = 1; // Turn on buzzer
        __delay_ms(2000);
        PORTBbits.RB2 = 0;
    }   
}
 
void __interrupt(irq(IRQ_INT0),base(0x3008)) INT0_ISR(void){ //interrupt  
    if(PIR1bits.INT0IF == 1){ //turns on buzzer 
        PORTBbits.RB2 = 1; __delay_ms(300); PORTBbits.RB2 = 0; __delay_ms(100); // -
        PORTBbits.RB2 = 1; __delay_ms(100); PORTBbits.RB2 = 0; __delay_ms(100); // .
        PORTBbits.RB2 = 1; __delay_ms(100); PORTBbits.RB2 = 0; __delay_ms(100); // .
        PORTBbits.RB2 = 1; __delay_ms(300); PORTBbits.RB2 = 0; __delay_ms(100); // -
        PORTBbits.RB2 = 1; __delay_ms(300); PORTBbits.RB2 = 0; __delay_ms(400); // -
        PORTBbits.RB2 = 1; __delay_ms(300); PORTBbits.RB2 = 0; __delay_ms(100); // -
        PORTBbits.RB2 = 1; __delay_ms(300); PORTBbits.RB2 = 0; __delay_ms(100); // -
    } 
    PIR1bits.INT0IF = 0;                 
    return; 
}