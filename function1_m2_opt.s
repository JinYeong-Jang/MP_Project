			AREA     RESET, CODE, READONLY
			ENTRY
			MOV       R6,#0x40000036    ; r6  : ReadPointer+HeaderSize  #0x40000036
			LDR       R7, =0x40200000   ; r7  : WriteRGBAPointer   		#0x40200000
			ADD       R8,R7,#0x00200000 ; r8  : WriteRedPointer 		#0x40400000
			ADD       R9,R8,#153600		; r9  : WriteGreenPointer 		#0x40425800
			ADD       R10,R9,#153600 	; r10 : WriteBluePointer 		#0x4044B000
			ADD       R11,R7,#0x00400000; r11 : WriteRGBCompPointer 	#0x40600000
			LDR       R7, =0x40200036   ; r7  : WriteRGBAPointer   		#0x40200036
			MOV       R4,#153600        ; r4  : i = 153600 	
			LDRH      R0,[R6],#2        ; Read [0x40000036 + i] 
			STRH      R0,[R7],#2        ; Write[0x40200000 + i]
JUMP1		LDR       R0,[R6],#4        ; Read [0x40000036 + i] 
			STR       R0,[R7],#4        ; Write[0x40200000 + i]
		    SUBS      R4,#1             ; i--
		    BNE       JUMP1             ; while(i > 0)
			MOV       R4,#153600  		; i = 153600
			LDR       R7, =0x40200036   ; r7  : ReadRGBApointer 		#0x40200036
JUMP2		LDRB      R0,[R7],#1        ; Read  Red   Value and ReadRGBApointer++
			STRB      R0,[R8],#1     	; Write Red   Value and WriteRedPointer++ 		
			LDRB      R0,[R7],#1        ; Read  Green Value and ReadRGBApointer++		
			STRB      R0,[R9],#1 		; Write Green Value and WriteGreenPointer++ 		
			LDRB      R0,[R7],#2		; Read  Blue  Value and ReadRGBApointer + 2 ,Skip Alpha Chunk
			STRB      R0,[R10],#1       ; Write Blue  Value and WriteBluePointer++ 
	     	SUBS      R4,R4,#1			; i--
			BNE       JUMP2             ; while(i > 0
			LDR       R5,=0xE0E0E0E0    ; r5 = 0xE0E0E0E0 masking [7:5]
			LDR   	  R6,=0xC0C0C0C0    ; r6 = 0xC0C0C0C0 masking [7:6]
			LDR       R7, =0x40200000   ; r7  : WriteRGBAPointer   		#0x40200000
			ADD       R8,R7,#0x00200000 ; r8  : WriteRedPointer 		#0x40400000
			ADD       R9,R8,#153600		; r9  : WriteGreenPointer 		#0x40425800
			ADD       R10,R9,#153600 	; r10 : WriteBluePointer 		#0x4044B000
			MOV       R4,#38400 		; i = 38400
JUMP3		LDR       R1,[R8],#4	    ; r1 = Read Red   Value 4byte and ReadRedPointer + 4
			AND       R1,R1,R5			; r1 = r1 and r5 -> masking all Red   Values [7:5]
			LDR       R2,[R9],#4	    ; r1 = Read Green Value 4byte and ReadBluePointer + 4
			AND       R2,R2,R5			; r2 = r1 and r5 -> masking all Green Values [7:5]
			ADD       R1,R1,R2,LSR #3   ; r1 = r1 + r2 -> ADD r1[masked Red Values] and r2[masked Green Values >> 3]
			LDR       R2,[R10],#4	    ; r2 = Read Blue  Value 4byte and ReadGreenPointer + 4
			AND       R2,R2,R6			; r1 = r1 and r6 -> masking all Blue Values [7:6]
			ADD       R1,R1,R2,LSR #6	; r1 = r1 + r2 -> ADD r1[masked Red Green Values] and r2[masked Blue Values >> 6]
			STR       R1,[R11],#4		; Write Compressed RGB Values
		    SUBS      R4,R4,#1          ; i--
			BNE       JUMP3             ; while(i > 0
			END
				