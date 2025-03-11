//-----------------------------
// Title: Temperature Control
//-----------------------------
// Purpose: The purpose of this program is to measures and adjust temperature automatically
// Dependencies: myConfigFile.inc
// Compiler: pic-as, 3.0
// Author: Ryan Chan
// OUTPUTS: PORTD.1 and PORTD.2 
// INPUTS: refTemp and measTemp 
// Versions:
//  	V1.0: 3/8/25 - First version 
//-----------------------------

;---------------------
; Initialization - make sure the path is correct
;---------------------
#include "myConfigFile.inc"
#include <xc.inc>

;----------------
; PROGRAM INPUTS
;----------------
;The DEFINE directive is used to create macros or symbolic names for values.
;It is more flexible and can be used to define complex expressions or sequences of instructions.
;It is processed by the preprocessor before the assembly begins.
#define  measuredTempInput 	-5 ; this is the input value
#define  refTempInput 		15 ; this is the input value

;---------------------
; Definitions
;---------------------
#define SWITCH    LATD,2  
#define LED0      PORTD,0
#define LED1	  PORTD,1
    
;---------------------
; Program Constants
;---------------------
;The EQU (Equals) directive is used to assign a constant value to a symbolic name or label.
;It is simpler and is typically used for straightforward assignments.
;It directly substitutes the defined value into the code during the assembly process.
REG10   equ     10h   // in HEX
REG11   equ     11h
REG01   equ     1h
   
; Define Registers 
refTemp      equ  0x20  ; Register storing reference temperature
measuredTemp equ  0x21  ; Register storing measured temperature
contReg      equ  0x22  ; Register storing control flag
	
refTemp_L    equ  0x60  ; Least significant digit of refTemp
refTemp_M    equ  0x61  ; Middle digit of refTemp
refTemp_H    equ  0x62  ; Most significant digit of refTemp

measuredTemp_L   equ  0x70  ; Least significant digit of measuredTemp
measuredTemp_M   equ  0x71  ; Middle digit of measuredTemp
measuredTemp_H   equ  0x72  ; Most significant digit of measuredTemp
     
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld ; Do not change
    
    ORG     0               ; Reset vector
    GOTO    START
    
    ORG	    0x20	    ; Begin assembly
    
START:    
    MOVLW   0
    MOVWF   TRISD,0	    ; Set PORTD to output
    MOVLW   refTempInput
    MOVWF   refTemp	    ; Store refTemp
    MOVLW   measuredTempInput	
    MOVWF   measuredTemp    ; Store measuredTemp

CHECK_NEGATIVE:
    BTFSC   measuredTemp, 7 ; Skip if bit 7 is 0 (measuredTemp is positive)
    NEGF    measuredTemp    ; If bit 7 is 1, perform two's complement negation
    GOTO    COMPARE	
    
COMPARE:
    MOVF    measuredTemp, W ; Load measuredTemp into WREG
    CPFSEQ  refTemp	    ; Skip next if measuredTemp == refTemp
    GOTO    CHECK_GREATER   ; Check if measuredTemp > refTemp
    GOTO    LED_OFF	    ; if measuredTemp == refTemp
   
CHECK_GREATER:
    CPFSLT  refTemp	    ; Skip next if measuredTemp > refTemp
    GOTO    LED_COOL	    ; if measuredTemp < refTemp
    GOTO    LED_HOT	    ; if measuredTemp > refTemp
    
LED_OFF:    
    MOVLW   0x00	    ; Set contReg = 0
    MOVWF   contReg
    BCF	    PORTD,1	    ; Turn off heating
    BCF	    PORTD,2	    ; Turn off cooling
    GOTO    HEX2DEC
    
LED_HOT:    
    MOVLW   0x02            ; Set contReg = 2
    MOVWF   contReg
    BSF	    PORTD,1	    ; Turn on heating
    BCF	    PORTD,2	    ; Turn off cooling
    GOTO    HEX2DEC
    
LED_COOL:
    MOVLW   0x01            ; Set contReg = 1 
    MOVWF   contReg
    BCF	    PORTD,1	    ; Turn off heating
    BSF	    PORTD,2	    ; Turn on cooling
    GOTO    HEX2DEC
    
HEX2DEC:
    CLRF    refTemp_L
    CLRF    refTemp_M
    CLRF    refTemp_H
    MOVLW   refTempInput
    MOVWF   REG10	    ; Store numerator
    MOVLW   100   
D1:			    ; D1 converts hundreds to DEC
    INCF    REG11, F	    
    SUBWF   REG10, F
    BC      D1
    DECF    REG11, F
    ADDWF   REG10, F
    MOVFF   REG11, refTemp_L
    CLRF    REG11
    MOVLW   10
D2:			    ; D2 converts tens to DEC
    INCF    REG11, F	    
    SUBWF   REG10, F
    BC      D2
    DECF    REG11, F
    ADDWF   REG10, F
    MOVFF   REG11, refTemp_M
    CLRF    REG11
    MOVFF   REG10, refTemp_H
    
    CLRF    measuredTemp_L
    CLRF    measuredTemp_M
    CLRF    measuredTemp_H
    MOVLW   measuredTempInput
    MOVWF   REG10	    ; Store numerator
    MOVLW   100   
D3:			    ; D3 converts hundreds to DEC
    INCF    REG11, F	    
    SUBWF   REG10, F
    BC      D3
    DECF    REG11, F
    ADDWF   REG10, F
    MOVFF   REG11, measuredTemp_L
    CLRF    REG11
    MOVLW   10
D4:			    ; D4 converts tens to DEC
    INCF    REG11, F	    
    SUBWF   REG10, F
    BC      D4
    DECF    REG11, F
    ADDWF   REG10, F
    MOVFF   REG11, measuredTemp_M
    CLRF    REG11
    MOVFF   REG10, measuredTemp_H
    GOTO    END_PROGRAM
    
END_PROGRAM:
    MOVLW   measuredTempInput	
    MOVWF   measuredTemp    ; Reset measuredTemp
    GOTO    END_PROGRAM	    ; Stay forever
    END
