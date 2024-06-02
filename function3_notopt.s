		AREA     RESET, CODE, READONLY
		ENTRY
        LDR r1, READPOINTER  ; r1 : Read Start Address
		LDR r2, WRITEPOINTER ; r2 : Write Start Address
		LDR r10,CheckValue   ; 381
		LDR r11,IMAGESIZE       ; Pixels = 960x640 
		MOV r9,#0            ; i = 0
LOOP	LDRB r3,[r1]         ; LDR R
		ADD r1,#1            ; ReadPointer++
		LDRB r4,[r1]         ; LDR G
		ADD r1,#1			 ; ReadPointer++
		LDRB r5,[r1]         ; LDR B
		ADD r1,#1			 ; ReadPointer++
		LDRB r6,[r1]         ; LDR A
		ADD r1,#1			 ; ReadPointer++
		ADD r7,r3,r4         ; R+G
		ADD r7,r7,r5         ; R+G+B
		CMP r7,r10           ; 3(R+G+B) > 381
		BPL ONEBIT
		MOV r8,#0            ;BIT 0
		BAL WRITEBIT
ONEBIT
		MOV r8,#1            ;BIT 1
WRITEBIT
        STRB r8,[r2]         ;WRITE 0 or 1
        ADD r2,#1            ;WritePointer++
		ADD r9,r9,#1         ;i++
		CMP r9,r11           ;i == 614400
		BNE LOOP		
READPOINTER  	  DCD 	  &40000036
WRITEPOINTER      DCD     &40200000
CheckValue        DCD     381
IMAGESIZE         DCD     153600
        END
