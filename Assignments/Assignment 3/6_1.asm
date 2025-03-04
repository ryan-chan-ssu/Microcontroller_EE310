SUM     EQU     0F7H
        ORG     0H

HERE    MOVLW   0
        MOVWF   SUM
        MOVLW   25H
        ADDLW   0x34
        ADDLW   11H
        ADDLW   0C1H
        ADDLW   25
        ADDLW   D'18'
        ADDLW   B'00000110'
        MOVWF   SUM

        MOVLW   SUM
        GOTO    HERE

        END
