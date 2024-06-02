		AREA     RESET, CODE, READONLY
		ENTRY
        LDR r1, READPOINTER  ; r1 : Read Start Address
		LDR r2, WRITEPOINTER ; r2 : Write Start Address
		LDR r10,CheckValue   ; 381
		LDR r11,IMAGESIZE       ; Pixels = 960x640 
		MOV r6,#0
		MOV r7,#1
LOOP	LDRB r3,[r1],#1      ; LDR R and ReadPointer++
		LDRB r4,[r1],#1      ; LDR G and ReadPointer++
		LDRB r5,[r1],#1      ; LDR B and ReadPointer++
		ADD r3,r3,r4		 ; ADD Red and Green Value
		ADD r3,r3,r5		 ; ADD Red + Green Value and Blue Value
		CMP r3,r10           ; Compare r6(GrayScale) and 127
		STRBPL r7,[r2],#1    ; STR WHITE to [r2] and WritePointer++
		STRBMI r6,[r2],#1    ; STR BLACK to [r2] and WritePointer++
		SUBS r11,r11,#1      ; Pixels--
		BNE LOOP             ; While(Pixels > 0 ) Loop
READPOINTER  	  DCD 	  &40000036
WRITEPOINTER      DCD     &40200000
CheckValue        DCD     381
IMAGESIZE            DCD     153600
        END
				