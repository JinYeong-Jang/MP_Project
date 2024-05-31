		AREA     RESET, CODE, READONLY
		ENTRY
        LDR r1, READPOINTER  ; r1 : Read Start Address
		LDR r2, WRITEPOINTER ; r2 : Write Start Address
		LDR r11,IMAGESIZE    ; i = 960x160
LOOP	LDRB r3,[r1],#1      ; LDR R and ReadPointer++
		LDRB r4,[r1],#1      ; LDR G and ReadPointer++
		LDRB r5,[r1],#2      ; LDR B and ReadPointer = ReadPointer + 2(Skip Alpha Value) 
		CMP r3,r4            ; Compare R and G
	    MOVMI r6,r4          ;( MI : R < G ) r6(MAX) <- G
		MOVMI r7,r3          ;( MI : R < G ) r7(MIN) <- R 
		MOVPL r6,r3          ;( PL : R > G ) r6(MAX) <- R 
		MOVPL r7,r4          ;( PL : R > G ) r7(MIN) <- G
		CMP r6,r5            ; Compare r6(MAX) and B
		MOVMI r6,r5          ;( MI : Min(R,G) < Max(R,G) < Blue ) r6(MAX) <- B
		BMI Jump             ; Branch to Jump LABEL 
		CMP r7,r5            ; Compare r7(MIN) and B 
		BMI Jump             ;( MI : Min(R,G) < Blue < Max(R,G) ) thus didn't change r6,r7)
		MOV r7,r5            ;( PL : Blue < Min(R,G) < Max(R,G) ) r7(MIN) <- B 
Jump	ADD r6,r6,r7         ; r6 = Max(R,G,B) + Min(R,G,B)
		LSR r6,#1            ; r6 / 2
		STRB r6,[r2],#1      ; STR r6 to [r2] and WritePointer++
		SUBS r11,r11,#1      ; i--
		BNE LOOP             ; While(Pixels > 0 ) Loop
READPOINTER  	  DCD 	  &40000036
WRITEPOINTER      DCD     &40400000
IMAGESIZE         DCD     153600
        END
			
