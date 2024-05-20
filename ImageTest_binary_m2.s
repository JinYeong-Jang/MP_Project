		AREA     RESET, CODE, READONLY
		ENTRY
        LDR r1, READPOINTER
		LDR r2, WRITEPOINTER
SkipHeader
        MOV r4,#0       ;i = 0
Loop1	LDRB r3,[r1]    ;fread
		ADD r1,#1       ;ReadPointer++
		STRB r3,[r2]    ;fwrite
		ADD r2,#1       ;WritePointer++
		ADD r4,#1       ;i++
		CMP r4,#16      ;i == 16
		BNE Loop1       ;for
IHDRChunk
		LDR r5, Width   
		LDR r6, Height
		LDR r7, bpp
		LDR r8, colortype
		MOV r9, #0      ;Compression type ,filter type , interlace = 0
		ROR r5,#24      ;width right shift 6byte
		STRB r5, [r2]   ;fwrite width (LSB 2byte) 
		ADD r1,#1       ;ReadPointer++
		ADD r2,#1		;WritePointer++
		ROR r5,#24
		STRB r5, [r2]
		ADD r1,#1
		ADD r2,#1
		ROR r5,#24
		STRB r5, [r2]
		ADD r1,#1
		ADD r2,#1
		ROR r5,#24
		STRB r5, [r2]
		ADD r1,#1
		ADD r2,#1
		ROR r6,#24
		STRB r6, [r2]   ;height
		ADD r1,#1
		ADD r2,#1
		ROR r6,#24		;height right shift 6byte
		STRB r6, [r2]	;fwrite height (LSB 2byte) 
		ADD r1,#1
		ADD r2,#1
		ROR r6,#24
		STRB r6, [r2]
		ADD r1,#1
		ADD r2,#1
		ROR r6,#24
		STRB r6, [r2]	
		ADD r1,#1     
		ADD r2,#1
		STRB r7, [r2]  ;write bpp
		ADD r1,#1
		ADD r2,#1
		ROR r6,#24
		STRB r8, [r2]  ;write colortype
		ADD r1,#1     
		ADD r2,#1
		STRB r9, [r2]  ;write compression
		ADD r1,#1
		ADD r2,#1
		STRB r9, [r2]	;write filtertype
		ADD r1,#1
		ADD r2,#1
		STRB r9, [r2]	;write interlace
		ADD r1,#1
		ADD r2,#1
FindIDATChunkAndCopyPaste
		LDRB r3,[r1]    ;Read data to r3 r4
		ADD r1,#1
		LDRB r4,[r1]
		ADD r1,#1
		;check ID r3,r4
		CMP r3,#73       ;check r3 == 'I'
		BNE CheckR4      ;r3 was not 'I' but check r4 can be 'I'
		CMP r4,#68       ;check r4 == 'D'
		BNE NotFound
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
		CMP r3,#65     ;check r3 == 'A'
		BNE NotFound
		CMP r4,#84     ;check r4 == 'T'
		BNE NotFound
	    ;found IDAT r3,r4 paste and END Loop
		STRB r3,[r2]    
		ADD r2,#1
		STRB r4,[r2]
		ADD r2,#1
	    BAL RGBtoBinary
CheckR4
        CMP r4,#73     ;check r4 == 'I'
		BNE NotFound
		
       	STRB r3,[r2]
		ADD r2,#1
		STRB r4,[r2]
		ADD r2,#1
		LDRB r3,[r1] 
		ADD r1,#1
		LDRB r4,[r1]
		ADD r1,#1 
		
		CMP r3,#68     ;check r3 == 'D'
		BNE NotFound
		CMP r4,#65     ;check r4 == 'A'
		BNE NotFound
		
		STRB r3,[r2]
		ADD r2,#1
		STRB r4,[r2]
		ADD r2,#1
		LDRB r4,[r1]   
		ADD r1,#1

		CMP r4,#84     ;check r3 == 'T'
		BNE LOOP2
		STRB r4,[r2]
		ADD r2,#1
        BAL RGBtoBinary
NotFound 
        STRB r3,[r2]
		ADD r2,#1
LOOP2	STRB r4,[r2]
		ADD r2,#1
		BAL FindIDATChunkAndCopyPaste
RGBtoBinary
		LDR r10, CheckValue     ; const r10 = 381
		LDR r11,Pixels          ; const r11 = 960 x 160 = 153600
		MOV r13,#0              ; i = 0
		MOV r3,r1               ; RedPointer	 = r1	
		ADD r4,r1,r11           ; GreenPointer = RedPointer + 153600
		ADD r5,r4,r11           ; BlackPointer = RedPointer + 153600 * 2
LOOP3	LDRB r6,[r3]            ; r6 = Red
		ADD r3,#1               ; RedPointer++
		LDRB r7,[r4]			; r7 = Green
		ADD r4,#1				; GreenPointer++
		LDRB r8,[r5]			; r8 = Black
		ADD r5,#1				; BlackPointer++

		ADD r9,r6,r7            ; R+G
		ADD r9,r9,r8            ; R+G+B
		CMP r9,r10              ; if 3(R+G+B) > 381
		BPL ONEBIT
		MOV r12,#0              ; Then BIT 0
		BAL WRITEBIT
ONEBIT
		MOV r12,#1             	; else BIT 1
WRITEBIT
        STRB r12,[r2]         	; WRITE 0 or 1
        ADD r2,#1             	; WritePointer++
		ADD r13,r13,#1          ; i++
		CMP r10,r11          	; if i == 153600 then go to IENDChunk
		BNE LOOP3               ; else go to LOOP3
IENDChunk
        MOV r3,#0
        LDR r4,IEND
		LDR r5,IEND2
		STRB r3,[r2]   ;0000
		ADD r2,#1
		STRB r3,[r2]
		ADD r2,#1
		STRB r3,[r2]
		ADD r2,#1
		STRB r3,[r2]
		ADD r2,#1
		STRB r4,[r2]
	    ADD r2,#1
		
		ROR r4,#8      ;IEND
		STRB r4,[r2]
	    ADD r2,#1
		ROR r4,#8
		STRB r4,[r2]
	    ADD r2,#1
		ROR r5,#8	
		STRB r4,[r2]
	    ADD r2,#1	
			
	    ADD r2,#1      ;IEND2
		ROR r5,#8
		STRB r5,[r2]
	    ADD r2,#1
		ROR r5,#8
		STRB r5,[r2]
	    ADD r2,#1
		ROR r5,#8	
		STRB r5,[r2]		
READPOINTER  	  DCD 	  &40000000
WRITEPOINTER      DCD     &40000000
CheckValue        DCD     381
Pixels            DCD     153600
Width             DCD     960
Height            DCD     160
bpp               DCD     1
colortype         DCD     2
IEND              DCD     &444E4549
IEND2             DCD     &826042AE
        END