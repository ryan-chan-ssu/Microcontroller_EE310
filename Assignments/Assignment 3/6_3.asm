INPUT1_L 	EQU 0x23  	; Lower byte of INPUT1
INPUT1_U 	EQU 0x01  	; Upper byte of INPUT1
INPUT2_L 	EQU 0x34  	; Lower byte of INPUT2
INPUT2_U 	EQU 0x02  	; Upper byte of INPUT2

RESULT_L 	EQU 0x10  	; Lower byte of result
RESULT_U 	EQU 0x11  	; Upper byte of result

		MOVLW INPUT1_L  ; Load lower byte of INPUT1 into WREG
		ADDLW INPUT2_L  ; Add lower byte of INPUT2
		MOVWF RESULT_L  ; Store result in 0x10

		MOVLW INPUT1_U  ; Load upper byte of INPUT1 into WREG
		ADDLW INPUT2_U  ; Add upper byte of INPUT2
		MOVWF RESULT_U  ; Store result in 0x11

		END
