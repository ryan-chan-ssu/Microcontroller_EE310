	MOVLW 22H   ;WREG = 22H
	MOVWF 5H    ;move (copy) WREG contents to location 5H
	MOVWF 6H    ;move (copy) WREG contents to location 6H
	MOVWF 7H    ;move (copy) WREG contents to location 7H
	ADDWF 5, 0  ;add W and loc 5, result in WREG (W = 44H)
	ADDWF 6, 0  ;add W and loc 6, result in WREG (W = 66H)
	ADDWF 7, 1  ;add W and loc 7, result in location 7H
            ;now location 7 has 88H and WREG = 66H