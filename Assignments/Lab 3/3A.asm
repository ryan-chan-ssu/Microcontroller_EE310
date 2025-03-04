	;;PART A1
	;ORG 	 0x00
	;MOVLW	 0x0
	;MOVFF   W,STATUS
	;MOvLW   0X88
	;ADDLW   0X88

	;;PART A2
	;MOVLW   0X0
	;MOVFF   W,STATUS
	;MOVLW   0X80
	;SUBLW   0X1

	;;PART B1
	;MOVLW   0X0
	;MOVFF   W,STATUS
	;MOvLW   0X88
	;ADDLW   0X88
	;MOVWF	30H
	;MOVFF	STATUS,20H ;Sets OV, DC, C
			   ;;0x88 + 0x88 = 1000 1000 + 1000 1000 = 1 0001 0000

	;;PART B2
	;MOVLW   0X0
	;MOVFF   W,STATUS
	;MOvLW   0X7F
	;ADDLW   0X7F
	;MOVWF	31H
	;MOVFF	STATUS,21H ;Sets N, OV, DC
			   ;;0x7F + 0x7F = 0111 1111 + 0111 1111 = 1111 1110

	;;PART B3
	;MOVLW   0X0
	;MOVFF   W,STATUS
	;MOvLW   0X50
	;ADDLW   0XF0
	;MOVWF	32H
	;MOVFF	STATUS,22H ;Sets C
			   ;;0x50 + 0xF0 = 0101 0000 + 1111 0000 = 1 0100 0000

	;;PART B4
	;MOVLW   0X0
	;MOVFF   W,STATUS
	;MOvLW   0X50
	;SUBLW   0XF0
	;MOVWF	33H
	;MOVFF	STATUS,23H ;Sets N, DC, C
			   ;;0xF0 - 0x50 = 1111 0000 - 0101 0000 = 1010 0000

	;;PART B5
	;MOVLW   0X0
	;MOVFF   W,STATUS
	;MOvLW   0XF0
	;SUBLW   0X50
	;MOVWF	34H
	;MOVFF	STATUS,24H ;Sets DC
			   ;;0x50 - 0xF0 = 0101 0000 - 1111 0000 = 0110 0000 (2's complement)

	;;PART B6
	;MOVLW   0X0
	;MOVFF   W,STATUS
	;MOvLW   0XFF
	;SUBLW   0X1
	;MOVWF	35H
	;MOVFF	STATUS,25H ;None of the flags are set
			   ;;0x1 - 0xFF = 0000 0001 - 1111 1111 = 0000 0010 (2's complement)

	;PART C
	MOVLW  0x60	;WREG = 0x60
	ADDLW  0x2	;WREG = 0x62
	ADDWF  0x1,1	;REG[0x1] = REG[0x1] + 0x62
	MOVLW  0x5	;WREG = 0x5
	ADDWF  0x1,0	;WREG = REG[0x1] + 0x5 = 0x67

	MOVFF  0x1,0x2	;REG[0x2] = REG[0x1]
	INCF   0x1,F	;REG[0x1] = REG[0x1] + 0x1 = 0x63
	DECF   0x2,F	;REG[0x2] = REG[0x2] - 0x1 = 0x61
	;STATUS: sets DC, C

	MOVLW  0xFF	;WREG = 0xFF
	MOVWF  0x4	;REG[0x4] = 0xFF
	;STATUS: sets Z
	CLRF   0x4	;REG[0x4] = 0x00
	SETF   0x3	;REG[0x3] = 0xFF

	MOVLW  0x1	;WREG = 0x1
	MOVWF  0x5	;REG[0x5] = 0x1
	NEGF   0x5	;REG[0x5] = 0xFF (Two's complement)
	;STATUS: sets N, resets Z, DC, C

	MOVLW  0x1	;WREG = 0x1
	MOVWF  0x6	;REG[0x5] = 0x1
	SWAPF  0x6,1	;REG[0x6] = 0x10

	MOVLW  0x1	;WREG = 0x1
	MOVWF  0x7	;REG[0x7] = 0x1
	COMF   0x7,1	;REG[0x7] = 0xFE (One's complement)
