		AREA     RESET, CODE, READONLY
		ENTRY
        LDR r1, READPOINTER  ; r1 : Read Start Address + SkipHeader
		LDR r2, WRITEPOINTER ; r2 : Write Start Address
		LDR r10,CheckValue   ; 381
		LDR r11,IMAGESIZE    ; i = 960x160
		MOV r6,#1 			 ; WHITE
		MOV r7,#0 			 ; BLACK
LOOP	LDRB r3,[r1],#1      ; LDR R and ReadPointer++
		LDRB r4,[r1],#1      ; LDR G and ReadPointer++
		LDRB r5,[r1],#2      ; LDR B and ReadPointer = ReadPointer + 2(Skip Alpha Value)
		ADD r3,r3,r4		 ; ADD Red and Green Value
		ADD r3,r3,r5		 ; ADD Red + Green Value and Blue Value
		CMP r3,r10           ; Compare r6(GrayScale) and 381
		STRBPL r6,[r2],#1    ; STRB WHITE to [r2] and WritePointer++
		STRBMI r7,[r2],#1    ; STRB BLACK to [r2] and WritePointer++
		SUBS r11,r11,#1      ; i--
		BNE LOOP             ; While(i > 0 ) Loop
READPOINTER  	  DCD 	  &40000036
WRITEPOINTER      DCD     &40200000
CheckValue        DCD     381
IMAGESIZE         DCD     153600
        END
				