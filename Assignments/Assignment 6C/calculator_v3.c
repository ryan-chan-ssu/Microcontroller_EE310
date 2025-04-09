/*
 * ---------------------
 * Title: Calculator Design
 * ---------------------
 * Program Details:
 *  This program implements a basic calculator on the PIC18F46K42 using a 4x4 keypad for input,
 *  and two 7-segment displays for output. Arithmetic operations are selected using A–D keys.
 *  'A' Addition
 *  'B' Subtraction
 *  'C' Multiplication
 *  'D' Division
 *  '#' Confirms the operation and outputs result
 *  '*' Resets the system
 *
 * Inputs: 4x4 Keypad
 * Outputs: Two 7-Segment displays
 *          PORTB for the first digit
 *          PORTD for the second digit
 *
 * Setup: C – MPLAB X Simulator
 * Date: April 6, 2025
 * Compiler: XC8, v3.00
 * Author: Ryan Chan
 *
 * Versions:
 *      V1.0: Initial implementation
 *
 * Useful links:
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4
 *      Instruction List (Farid): http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf
 */

// CONFIG1L
#pragma config FEXTOSC = LP
#pragma config RSTOSC = EXTOSC

// CONFIG1H
#pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
#pragma config PR1WAY = ON      // PRLOCKED One-Way Set Enable bit (PRLOCK bit can be cleared and set only once)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)

// CONFIG2L
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCK bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (ULPBOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG2H
#pragma config BORV = VBOR_2P45 // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 2.45V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config DEBUG = OFF      // Debugger Enable bit (Background debugger disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG3H
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4L
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG4H
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not write-protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)

// CONFIG5L
#pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "C:/Program Files/Microchip/xc8/v3.00/pic/include/proc/pic18f47k42.h"

#define _XTAL_FREQ 4000000

// Define rows and columns for keypad
#define R0 LATAbits.LATA0
#define R1 LATAbits.LATA1
#define R2 LATAbits.LATA2
#define R3 LATAbits.LATA3
#define C0 PORTCbits.RC4
#define C1 PORTCbits.RC5
#define C2 PORTCbits.RC6
#define C3 PORTCbits.RC7

// 7-segment display encodings for 0-9 and E for error
#define SEGMENT_OFF 0b00000000
#define SEGMENT_0   0b00111111
#define SEGMENT_1   0b00000110
#define SEGMENT_2   0b01011011
#define SEGMENT_3   0b01001111
#define SEGMENT_4   0b01100110
#define SEGMENT_5   0b01101101
#define SEGMENT_6   0b01111101
#define SEGMENT_7   0b00000111
#define SEGMENT_8   0b01111111
#define SEGMENT_9   0b01101111
#define SEGMENT_E   0b01111001

// convert number to 7-segment pattern
unsigned char getSegment(char key) {
    switch (key) {
        case 0: return SEGMENT_0;
        case 1: return SEGMENT_1;
        case 2: return SEGMENT_2;
        case 3: return SEGMENT_3;
        case 4: return SEGMENT_4;
        case 5: return SEGMENT_5;
        case 6: return SEGMENT_6;
        case 7: return SEGMENT_7;
        case 8: return SEGMENT_8;
        case 9: return SEGMENT_9;
        case 'E': return SEGMENT_E;
        default: return SEGMENT_OFF;
    }
}

// scan keypad and return pressed key
char getKeypadKey() {
    const char keymap[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };

    while (1) {
        for (char row = 0; row < 4; row++) {
            R0 = R1 = R2 = R3 = 1;
            switch (row) {
                case 0: R0 = 0; break;
                case 1: R1 = 0; break;
                case 2: R2 = 0; break;
                case 3: R3 = 0; break;
            }
            __delay_ms(5);
            if (C0 == 0) { while (C0 == 0); return keymap[row][0]; }
            if (C1 == 0) { while (C1 == 0); return keymap[row][1]; }
            if (C2 == 0) { while (C2 == 0); return keymap[row][2]; }
            if (C3 == 0) { while (C3 == 0); return keymap[row][3]; }
        }
    }
}

// output 2 digits to 7-segment displays
void displayDigits(char high, char low) {
    LATB = getSegment(high);
    LATD = getSegment(low);
}

// turn off displays
void clearDisplays() {
    LATB = SEGMENT_OFF;
    LATD = SEGMENT_OFF;
    LATDbits.LATD0 = 0;
}

// main calculator logic
void main(void) {
    // I/O setup
    TRISB = 0x00; LATB = SEGMENT_OFF; ANSELB = 0x00;
    TRISD = 0x00; LATD = SEGMENT_OFF; ANSELD = 0x00;
    TRISAbits.TRISA0 = 0; TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0; TRISAbits.TRISA3 = 0;
    TRISCbits.TRISC4 = 1; TRISCbits.TRISC5 = 1;
    TRISCbits.TRISC6 = 1; TRISCbits.TRISC7 = 1;
    ANSELA = 0x00; ANSELC = 0x00; WPUC |= 0xF0;

    char x_high = 0, x_low = 0, y_high = 0, y_low = 0;
    unsigned char x_input_REG = 0;
    unsigned char y_input_REG = 0;
    char operation = 'A';
    unsigned char Operation_Reg = 0;

    while (1) {
        // first number, 'tens' digit
        while (1) {
            x_high = getKeypadKey();
            if (x_high == '*') { clearDisplays(); continue; }
            if (x_high >= '0' && x_high <= '9') break;
        }
        LATB = getSegment(x_high - '0');
        
        // first number, 'ones' digit
        while (1) {
            x_low = getKeypadKey();
            if (x_low == '*') { clearDisplays(); goto restart; }
            if (x_low >= '0' && x_low <= '9') break;
        }
        LATD = getSegment(x_low - '0');
        x_input_REG = (x_high - '0') * 10 + (x_low - '0');

        // operation
        while (1) {
            operation = getKeypadKey();
            if (operation == '*') { clearDisplays(); goto restart; }
            if (operation == 'A' || operation == 'B' || operation == 'C' || operation == 'D') {
                Operation_Reg = operation;
                break;
            }
        }

        clearDisplays();
        
        // second number, 'tens' digit
        while (1) {
            y_high = getKeypadKey();
            if (y_high == '*') { clearDisplays(); goto restart; }
            if (y_high >= '0' && y_high <= '9') break;
        }
        LATB = getSegment(y_high - '0');
        
        // second number, 'ones' digit
        while (1) {
            y_low = getKeypadKey();
            if (y_low == '*') { clearDisplays(); goto restart; }
            if (y_low >= '0' && y_low <= '9') break;
        }
        LATD = getSegment(y_low - '0');
        y_input_REG = (y_high - '0') * 10 + (y_low - '0');

        // wait for #
        while (1) {
            char confirm = getKeypadKey();
            if (confirm == '*') { clearDisplays(); goto restart; }
            if (confirm == '#') break;
        }

        // calculation
        int result = 0;
        if (Operation_Reg == 'A') result = x_input_REG + y_input_REG;
        else if (Operation_Reg == 'B') result = x_input_REG - y_input_REG;
        else if (Operation_Reg == 'C') result = x_input_REG * y_input_REG;
        else if (Operation_Reg == 'D') {
            if (y_input_REG == 0) { LATB = SEGMENT_E; LATD = SEGMENT_E; goto waitClear; }
            result = x_input_REG / y_input_REG;
        }

        // handle output
        if (result > 99 || result < -99) {
            LATB = SEGMENT_E;
            LATD = SEGMENT_E;
        } else {
            unsigned char absResult = (result < 0) ? -result : result;
            LATB = getSegment(absResult / 10);
            LATD = getSegment(absResult % 10);
            if (result < 0) LATD |= 0b10000000;  // Set DP bit for negative sign
        }

        // wait for reset
        waitClear:
        while (getKeypadKey() != '*');
        clearDisplays();

        restart: ;
    }
}
