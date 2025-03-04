DREG    EQU  0x0A       ; Register for delay
BLINK   EQU  0x06       ; Register for blink counter

        ORG  	0       ; Start program at address 0
BACK    MOVLW   0x05    ; Load blink count (5)
        MOVWF   BLINK   ; Store in BLINK
        MOVLW   0x00    ; Set TRISC as output
        MOVWF   TRISC

        MOVLW   0x01    ; Turn LED ON
        MOVWF   PORTC
        CALL    DELAY   ; Call delay function

        MOVLW   0x00    ; Turn LED OFF
        MOVWF   PORTC
        CALL    DELAY   ; Call delay function

        DECF    BLINK,1 ; Decrement blink counter
        BZ      HERE    ; If BLINK is 0, stop blinking
        GOTO    BACK    ; Repeat loop

DELAY   MOVLW   0x05    ; Load delay count
        MOVWF   DREG    ; Store in DREG

AGAIN   NOP             ; No operation (waste clock cycles)
        NOP
        DECFSZ  DREG,1  ; Decrement DREG, skip next if zero
        BNZ     AGAIN   ; Loop until DREG = 0
        RETURN          ; Return to main program

HERE    END             ; End program
