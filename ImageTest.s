		AREA     RESET, CODE, READONLY
		ENTRY
        LDR r1, READPOINTER
		LDR r2, WRITEPOINTER
		BL SkipHeader
SUB1 	MOV r4, #4
L1		ROR r5,r6        
		STRB r5,[r2],#1   
		SUBS r4,#1
		BNE L1
		MOV pc,r14
SkipHeader
        MOV r4,#16       ;i = 16
Loop1	LDRB r3,[r1],#1    ;fread ;ReadPointer++
		STRB r3,[r2],#1    ;fwrite
		SUBS r4,#1
		BNE Loop1       ;for
IHDRChunk 
		LDR r7, bpp
		LDR r8, colortype
		MOV r9, #0      ;Compression type ,filter type , interlace = 0
		MOV r6, #24     ;rotate value
		LDR r5, Width  
		STMFD r13!,{r14}
		BL SUB1
		LDR r5, Height		
		STMFD r13!,{r14}
		BL SUB1
		STRB r7,[r2],#1  ;write bpp
		STRB r8,[r2],#1  ;write colortype 
		STRB r9,[r2],#1  ;write compression
		STRB r9,[r2],#1  ;write filtertype
		STRB r9,[r2],#1	 ;write interlace
		ADD r1,#13       ;IHDR length
FindIDATChunkAndCopyPaste
		LDRB r3,[r1],#1    ;Read data to r3 r4
		LDRB r4,[r1],#1
		;check ID r3,r4
		CMP r3,#73       ;check r3 == 'I'
		BNE CheckR4      ;r3 was not 'I' but check r4 can be 'I'
		CMP r4,#68       ;check r4 == 'D'
		BNE NotFound
		;find ID and paste r3 r4
		STRB r3,[r2],#1  
		STRB r4,[r2],#1
		;copy r3 r4
		LDRB r3,[r1],#1
		LDRB r4,[r1],#1
        ;check AT r3 r4
		CMP r3,#65     ;check r3 == 'A'
		BNE NotFound
		CMP r4,#84     ;check r4 == 'T'
		BNE NotFound
	    ;found IDAT r3,r4 paste and END Loop
		STRB r3,[r2],#1  
		STRB r4,[r2],#1
	    BAL RGBAtoBinary
CheckR4
        CMP r4,#73     ;check r4 == 'I'
		BNE NotFound
		
       	STRB r3,[r2],#1
		STRB r4,[r2],#1
		LDRB r3,[r1],#1
		LDRB r4,[r1],#1
		
		CMP r3,#68     ;check r3 == 'D'
		BNE NotFound
		CMP r4,#65     ;check r4 == 'A'
		BNE NotFound
		
		STRB r3,[r2],#1
		STRB r4,[r2],#1
		LDRB r4,[r1],#1

		CMP r4,#84     ;check r3 == 'T'
		BNE LOOP2
		STRB r4,[r2],#1
        BAL RGBAtoBinary
NotFound 
        STRB r3,[r2],#1
LOOP2	STRB r4,[r2],#1
		BAL FindIDATChunkAndCopyPaste
RGBAtoBinary
		LDR r9, CheckValue
		LDR r11,Pixels
		MOV r10,#153600
LOOP3	LDRB r3,[r1],#1      ;LDR R
		LDRB r4,[r1],#1      ;LDR G
		LDRB r5,[r1],#2      ;LDR B
		ADD r7,r3,r4         ;R+G
		ADD r7,r7,r5         ;R+G+B
		CMP r7,r9            ;3(R+G+B) > 381
		BPL ONEBIT
		MOV r8,#0            ;BIT 0
		BAL WRITEBIT
ONEBIT
		MOV r8,#1            ;BIT 1
WRITEBIT
        STRB r8,[r2],#1      ;WRITE 0 or 1
		SUBS r10,#1
		BNE LOOP3
IENDChunk
        MOV r3,#0
		STRB r3,[r2],#1      ;0000
		STRB r3,[r2],#1
		STRB r3,[r2],#1
		STRB r3,[r2],#1
		
		MOV r6,#8		;rotate value
  		LDR r5, IEND  
		STMFD r13!,{r14}
		BL SUB1
			
		LDR r5, IEND2 
		STMFD r13!,{r14}
		BL SUB1			
READPOINTER  	  DCD 	  &40000000
WRITEPOINTER      DCD     &40000000
CheckValue        DCD     381
Pixels            DCD     153600
Width             DCD     960
Height            DCD     160
bpp               DCD     1
colortype         DCD     2
IEND              DCD     &4E454944
IEND2             DCD     &6042AE82
        END