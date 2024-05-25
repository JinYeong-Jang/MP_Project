			AREA     RESET, CODE, READONLY
			ENTRY
			MOV       R6,#0x40000036    ; r11 : ReadPointer + (HeaderSize)
			LDR       R7, =0x40200000   ; r7  : WriteRGBAPointer   		#0x40200000
			ADD       R8,R7,#0x00200000 ; r8  : WriteRedPointer 		#0x40400000
			ADD       R9,R8,#153600		; r9  : WriteGreenPointer 		#0x40425800
			ADD       R10,R9,#153600 	; r10 : WriteBluePointer 		#0x4044B000
			ADD       R11,R7,#0x00400000; r11 : WriteRGBCompPointer 	#0x40600000
			MOV       R4,#0x00096000    ; r4  : i = 96000            
JUMP1		LDRB      R0,[R6],#1       ; Read [0x40000036 + i] 
			STRB      R0,[R7],#1        ; Write[0x40200000 + i]
			SUBS      R4,#1             ; i--
		    BPL       JUMP1
			MOV       R4,#153600  		; i = 153600
			LDR       R7, =0x40200000
			BL  	  JUMP3             ; Branch JUMP3,Prevent One more Trial
JUMP2		LDRB      R0,[R7],#1        ; Read  Red   [0x40200000 + i]
			STRB      R0,[R8],#1     	; Write Red   [0x40400000 + i]
			LDRB      R0,[R7],#1        ; Read Green  [0x40200000 + r0]
			STRB      R0,[R9],#1 		; Write Green [0x40200000 + r1]
			LDRB      R0,[R7],#2		; Read Blue   [0x40200000 + r0]
			STRB      R0,[R10],#1        ; Write Blue  [0x40200000 + r1]
JUMP3		SUBS      R4,R4,#1
			BPL       JUMP2
			LDR       R5,=0xE0E0E0E0    ; r5 = 0xE0E0E0E0 masking [7:5]
			LDR   	  R6,=0xC0C0C0C0    ; r6 = 0xC0C0C0C0 masking [7:6]
			LDR       R7, =0x40200000   ; r7  : WriteRGBAPointer   		#0x40200000
			ADD       R8,R7,#0x00200000 ; r8  : WriteRedPointer 		#0x40400000
			ADD       R9,R8,#153600		; r9  : WriteGreenPointer 		#0x40425800
			ADD       R10,R9,#153600 	; r10 : WriteBluePointer 		#0x4044B000
			MOV       R4,#38400
JUMP4		LDR       R1,[R8],#4	    ; r1 = Read Red Value 4byte [0x40400000 + i * 4]
			AND       R1,R1,R5			; r1 = r1 and r5 -> masking all Red   Values [7:5]
			LDR       R2,[R9],#4	    ; r1 = Read Green Value 4byte [0x40400000 + j * 4]
			AND       R2,R2,R5			; r1 = r1 and r6 -> masking all Green Values [7:5]
			ADD       R1,R1,R2,LSR #3   ; r1 = r2 + r1 -> ADD r2[masked Green Values >> 3] and r1[masked Red Values]
			LDR       R2,[R10],#4	    ; r1 = Read Blue Value 4byte [0x40400000 + j * 4]
			AND       R2,R2,R6			; r1 = r1 and r6 -> masking Blue Values [7:6]
			ADD       R1,R1,R2,LSR #6	; r1 = r2 + r1 -> ADD r1[masked Blue Values >> 6] and r2[masked Red Green Values]
			STR       R1,[R11],#4		; Write Compressed RGB Values
			SUBS      R4,R4,#1       
			BPL       JUMP4
			END