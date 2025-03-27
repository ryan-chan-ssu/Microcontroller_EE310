;---------------------------------------------------------------
; Title: Keypad-Controlled 7-Segment Counter
;---------------------------------------------------------------
; Program Details:
; The purpose of this program is to implement a system
; that responds to the following keypad inputs:
;   - Key '1': Increment the counter
;   - Key '2': Decrement the counter
;   - Key '3': Reset the counter to 0
;
; Inputs: Keypad (connected to PORTB)
; Outputs: 7-Segment Display (connected to PORTD)
; Setup: Curiosity PIC board with battery
;
; Date: Mar 25, 2025
; File Dependencies / Libraries:
;   - AssemblyConfig.inc in Header Folder
;
; Compiler: pic-as 3.0
; Author: Ryan Chan
; Versions:
;   - v1.0: First Iteration
;   - v2.0: Fixed Logic
;---------------------------------------------------------------
    
;---------------------
; Initialization
;---------------------
#include "myConfigFile.inc"
#include <xc.inc>

;---------------------
; Program Variables
;---------------------
COUNTER     equ 0x20      ; 0?0xF value
BUTTON	    equ 0x21      ; 1=up, 2=down, 3=reset
Outer_delay equ 0x22
Inner_delay equ 0x23

;---------------------
; Start Program
;---------------------
    PSECT absdata,abs,ovrld

    ORG     0x00
    GOTO    _initialization

    ORG     0x20

_initialization:
    CLRF    COUNTER
    CLRF    BUTTON
    RCALL   _setupPorts
    GOTO    _main

_setupPorts:
    BANKSEL PORTB
    CLRF PORTB
    BANKSEL LATB
    CLRF LATB
    BANKSEL ANSELB
    CLRF ANSELB
    BANKSEL TRISB
    MOVLW 0b11111000	; Set RB0-RB2 as outputs and RB3-RB7 as inputs
    MOVWF TRISB

    BANKSEL PORTD
    CLRF PORTD
    BANKSEL LATD
    CLRF LATD
    BANKSEL ANSELD
    CLRF ANSELD
    BANKSEL TRISD
    MOVLW 0x00
    MOVWF TRISD

;---------------------
; Main Loop
;---------------------
_main:
    CALL _displayDigit      ; Display "0" at the start
_loop:
    CALL _displayDigit
    RCALL _scanKeypad

    MOVF BUTTON, W
    XORLW 0x01              ; Is key #1 held?
    BTFSC STATUS, 2
    GOTO _increment_loop

    MOVF BUTTON, W
    XORLW 0x02              ; Is key #2 held?
    BTFSC STATUS, 2
    GOTO _decrement_loop

    MOVF BUTTON, W
    XORLW 0x03              ; Is key #3 held?
    BTFSC STATUS, 2
    GOTO _reset

    GOTO _loop              ; If no valid key, scan again

_increment_loop:
    CALL _displayDigit
    CALL _delay

    INCF COUNTER, F
    MOVF COUNTER, W
    XORLW 0x10              ; If COUNTER > 0x0F, go to 0
    BTFSS STATUS, 2
    GOTO _inc_hold_check
    CLRF COUNTER

_inc_hold_check:
    RCALL _scanKeypad
    MOVF BUTTON, W
    XORLW 0x01
    BTFSC STATUS, 2
    GOTO _increment_loop     ; Continue if still holding #1
    GOTO _loop

_decrement_loop:
    CALL _displayDigit
    CALL _delay

    DECF COUNTER, F           ; Decrease COUNTER
    MOVF COUNTER, W
    XORLW 0xFF                ; Is COUNTER now ready to wrap around?
    BTFSC STATUS, 2
    MOVLW 0x0F                ; If yes, fix to 0x0F
    BTFSC STATUS, 2
    MOVWF COUNTER

_dec_hold_check:
    RCALL _scanKeypad
    MOVF BUTTON, W
    XORLW 0x02
    BTFSC STATUS, 2
    GOTO _decrement_loop     ; Continue if still holding #2
    GOTO _loop

_reset:
    CLRF COUNTER
    CALL _displayDigit
    GOTO _loop

_displayDigit:
    LFSR 0, _table	    ; Load address of 7-Segment Table into FSR0
    MOVF FSR0H, W           ; Copy FSR0 into TBLPTR
    MOVWF TBLPTRH
    MOVF FSR0L, W
    MOVWF TBLPTRL

    BANKSEL COUNTER         ; Load digit index from COUNTER and offset TBLPTRL
    MOVF COUNTER, W
    ADDWF TBLPTRL, F
    TBLRD*                  ; Table read: load byte at TBLPTR into TABLAT
    MOVF TABLAT, W
    MOVWF LATD		    ; Output to 7-segment
    RETURN

;---------------------
; 7-Segment Table
;---------------------
    ORG	  0x200
_table:
    DB  0x3F  ; 0
    DB  0x06  ; 1
    DB  0x5B  ; 2
    DB  0x4F  ; 3
    DB  0x66  ; 4
    DB  0x6D  ; 5
    DB  0x7D  ; 6
    DB  0x07  ; 7
    DB  0x7F  ; 8
    DB  0x6F  ; 9
    DB  0x77  ; A
    DB  0x7C  ; B
    DB  0x39  ; C
    DB  0x5E  ; D
    DB  0x79  ; E
    DB  0x71  ; F
    
;---------------------
; Delay
;---------------------
_delay:
    MOVLW 255
    MOVWF Outer_delay
_outer_loop:
    MOVLW 255
    MOVWF Inner_delay
_inner_loop:
    NOP
    NOP
    DECFSZ Inner_delay, F
    GOTO _inner_loop
    DECFSZ Outer_delay, F
    GOTO _outer_loop
    RETURN
    
;---------------------
; Keypad Scanner
;---------------------
_scanKeypad:
    CLRF BUTTON

    BCF LATB, 0             ; Set Column 1 LOW
    BCF LATB, 1             ; Set Column 2 LOW
    BCF LATB, 2             ; Set Column 3 LOW
    NOP

    BSF LATB, 0             ; Set Column 1 HIGH
    BTFSC PORTB, 3          ; Check if Row 1 is HIGH
    MOVLW 0x01	            ; If so, load "1" into WREG
    BCF LATB, 0             ; Set Column 1 LOW

    BSF LATB, 1             ; Set Column 2 HIGH
    BTFSC PORTB, 3          ; Check if Row 1 is HIGH
    MOVLW 0x02	            ; If so, load "2" into WREG
    BCF LATB, 1             ; Set Column 2 LOW

    BSF LATB, 2             ; Set Column 3 HIGH
    BTFSC PORTB, 3          ; Check if Row 1 is HIGH
    MOVLW 0x03	            ; If so, load "3" into WREG
    BCF LATB, 2             ; Set Column 3 LOW

    MOVWF BUTTON       ; Store result in BUTTON register
                       ; If no key was pressed, WREG still holds 0
    RETURN    

    END