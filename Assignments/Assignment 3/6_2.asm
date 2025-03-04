MYVAL  	EQU  	9        ;MYVAL = 9
R0     	EQU  	0x10     ;assign RAM addresses to R0
R1     	EQU  	0x11	 ;R1
R2     	EQU  	0x12	 ;R2
R3     	EQU  	0x13	 ;R3
R4     	EQU  	0x14	 ;R4

SUM    	EQU  	15H

	ORG	0x10	 ;Program memory start from 0x10
	MOVLW  	MYVAL    ;WREG = 9
	MOVWF  	R0       ;RAM loc 0x10 has 9
	MOVWF  	R1       ;RAM loc 0x11 has 9
	MOVWF  	R2       ;RAM loc 0x12 has 9
	MOVWF  	R3       ;RAM loc 0x13 has 9
	MOVWF  	R4       ;RAM loc 0x14 has 9

	ADDWF  	R0, W    ;WREG = R0 + WREG
	ADDWF  	R1, W    ;WREG = R1 + WREG
	ADDWF  	R2, W    ;WREG = R2 + WREG
	ADDWF  	R3, W    ;WREG = R3 + WREG
	ADDWF  	R4, W    ;WREG = R4 + WREG

	MOVWF  	SUM
HERE	GOTO	HERE
	END
