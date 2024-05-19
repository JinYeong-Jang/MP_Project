		AREA     RESET, CODE, READONLY
		ENTRY
        LDR r1, VALUE1
		LDR r2, VALUE2
		LDR r9, VALUE3
		LDR r11,VALUE4
FindIDATChunkAndCopyPaste
		LDRB r3,[r1]
		ADD r1,#1
		LDRB r4,[r1]
		ADD r1,#1
		;check ID r3,r4
		CMP r3,#73     ;check r3 'I'
		BNE CheckR4
		CMP r4,#68     ;check r4 'D'
		BNE notfound
		;find ID and paste r3 r4
		STRB r3,[r2]  
		ADD r2,#1
		STRB r4,[r2]
		ADD r2,#1
		;copy r3 r4
		LDRB r3,[r1]
		ADD r1,#1
		LDRB r4,[r1]
		ADD r1,#1
        ;check AT r3 r4
		CMP r3,#65     ;check r3 'A'
		BNE notfound
		CMP r4,#84     ;check r4 'T'
		BNE notfound
	    ;found IDAT r3,r4 paste and END Loop
		STRB r3,[r2]    
		ADD r2,#1
		STRB r4,[r2]
		ADD r2,#1
	    BAL READIDAT
CheckR4
        CMP r4,#73     ;check r4 'I'
		BNE notfound
		
       	STRB r3,[r2]
		ADD r2,#1
		STRB r4,[r2]
		ADD r2,#1
		LDRB r3,[r1] 
		ADD r1,#4
		LDRB r4,[r1]
		ADD r1,#4 
		
		CMP r3,#68     ;check r3 'D'
		BNE notfound
		CMP r4,#65     ;check r4 'A'
		BNE notfound
		
		STRB r3,[r2]
		ADD r2,#1
		STRB r4,[r2]
		ADD r2,#1
		LDRB r3,[r1]   
		ADD r1,#4
		LDRB r4,[r1]
		ADD r1,#4

		CMP r3,#84     ;check r3 'T'
		BNE notfound
		STRB r3,[r2]
		ADD r2,#1
		STRB r4,[r2]
		ADD r2,#1
        BAL READIDAT
notfound 
        STRB r3,[r2]
		ADD r2,#1
		STRB r4,[r2]
		ADD r2,#1
		BAL FindIDATChunkAndCopyPaste
READIDAT
		LDRB r3,[r1]
		ADD r1,#1
		LDRB r4,[r1]
		ADD r1,#1
		LDRB r5,[r1]
		ADD r1,#1
		LDRB r6,[r1]
		ADD r1,#1
		
		ADD r7,r3,r4
		ADD r7,r7,r5
		
		CMP r7,r9
		BPL ZEROBIT
		MOV r8,#0
		BAL WRITEBIT
ZEROBIT
		MOV r8,#1
WRITEBIT
        STRB r8,[r2]
        ADD r2,#1
		ADD r10,r10,#1
		CMP r10,r11
		BNE READIDAT


VALUE1  	DCD 	&40000000
VALUE2      DCD     &20000000
VALUE3      DCD     &17D
VALUE4      DCD     &25800
        END
