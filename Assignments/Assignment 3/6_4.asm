INPUT1_L EQU   0x83  ; Lower byte of INPUT1
INPUT1_U EQU   0x2   ; Upper byte of INPUT1
INPUT2_L EQU   0x94  ; Lower byte of INPUT2
INPUT2_U EQU   0x1   ; Upper byte of INPUT2

RESULT_L EQU   0x10  ; Lower byte of result
RESULT_U EQU   0x11  ; Upper byte of result

	 MOVLW INPUT1_L     ; Load lower byte of INPUT1 into WREG
	 ADDLW INPUT2_L     ; Add lower byte of INPUT2
	 MOVWF RESULT_L     ; Store result in 0x10

	 BTFSS STATUS, C    ; If carry is set, skip next instruction
	 CLRF WREG          ; If no carry, load 0 into WREG
	 BTFSC STATUS, C    ; If carry is set, skip next instruction
	 MOVLW 0x01         ; If carry, load 1 into WREG

	 ADDLW INPUT1_U	    ; Add upper byte of INPUT1 + carry
	 ADDLW INPUT2_U     ; Add upper byte of INPUT2
	 MOVWF RESULT_U     ; Store result in 0x11

	 END
