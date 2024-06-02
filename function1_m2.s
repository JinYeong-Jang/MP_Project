			AREA     RESET, CODE, READONLY
			ENTRY
			MOV       R11,#0x40000036   ; r11 : StartAddress + 36 (HeaderSize)
			LDR       R7, =0x40200000   ; r7  : WriteRGBAPointer   		#0x40200000
			ADD       R9,R7,#0x00200000 ; r9  : WriteRRGGBBPointer 		#0x40400000
			ADD       R10,R7,#0x00400000; r10 : WriteRGBCompPointer 	#0x40600000
			MOV       R4,#0x00000000    ; r4  : i = 0
			B         JUMP1             
JUMP2		LDRB      R0,[R11,R4]       ; Read [0x40000036 + i] 
			STRB      R0,[R7,R4]        ; Write[0x40200000 + i]
			ADD       R4,R4,#0x00000001 ; i++
JUMP1		CMP       R4,#0x00096000    ; while(i == 614400)
			BCC       JUMP2
			MOV       R4,#0x00000000    ; i = 0
			MOV       R5,#0x00000000    ; j = 0
			B         JUMP3
JUMP4		LDRB      R0,[R7,R4]        ; Read  Red   [0x40200000 + i]
			STRB      R0,[R9,R5]        ; Write Red   [0x40400000 + i]
			ADD       R0,R4,#0x00000001 ; r0 = i + 1 
			LDRB      R0,[R7,R0]        ; Read Green  [0x40200000 + r0]
			ADD       R1,R5,#153600 	; r1 = j + 153600
			STRB      R0,[R9,R1]		; Write Green [0x40200000 + r1]
			ADD       R0,R4,#0x00000002 ; r0 = i + 2
			LDRB      R0,[R7,R0]		; Read Blue   [0x40200000 + r0]
			ADD       R1,R5,#307200 	; r1 = j + 307200
			STRB      R0,[R9,R1]        ; Write Blue  [0x40200000 + r1]
			ADD       R4,R4,#0x00000004 ; i = i + 4 
			ADD       R5,R5,#0x00000001 ; j = j + 1
JUMP3		CMP       R4,#614400   	    ; while ( i == 614400 )
			BCC       JUMP4
			LDR       R6,=0xE0E0E0E0    ; r6 = 0xE0E0E0E0 masking Value
			LDR       R8,=0x40400000    ; r8 = 0x40400000
			MOV       R4,#0x00000000    ; i = 0
			MOV       R5,#0x00000000    ; j = 0
			B         JUMP5
JUMP7		MOV       R1,R5             ; r1 = i 
			ADD       R5,R5,#0x00000001 ; i++
			ADD       R0,R10,R1,LSL #2  ; r0 = #0x40600000 + i * 4
			MOV       R1,R4             ; r1 = j
			ADD       R4,R4,#0x00000001 ; j++
			LDR       R2,[R0]           ; r2 = Read Empty Byte [0x40600000 + i * 4]
			LDR       R1,[R8,R1,LSL #2] ; r1 = Read Red Value 4byte [0x40400000 + i * 4]
			AND       R1,R1,R6			; r1 = r1 and r6 -> masking all Red   Values [7:5]
			ADD       R1,R1,R2          ; r1 = r1 + r2   -> ADD r1[masked Red values] and r2[Empty Values]
			STR       R1,[R0]           ; Write masked Red values
JUMP5		CMP       R5,#0x00009600    ; while (i == 38400)
			BCC       JUMP7
			MOV       R5,#0x00000000    ; i = 0 (Be careful j is not 0)
			B         JUMP6
JUMP10		MOV       R1,R5             ; r1 = i
			ADD       R5,R5,#0x00000001 ; i++
			ADD       R0,R10,R1,LSL #2  ; r0 = #0x40400000 + i * 4
			MOV       R1,R4             ; r1 = j
			ADD       R4,R4,#0x00000001 ; j++
			LDR       R2,[R0]           ; r2 = Read Masked Red Value 4byte [0x40600000 + i * 4]
			LDR       R1,[R8,R1,LSL #2] ; r1 = Read Green Value 4byte [0x40400000 + j * 4]
			AND       R1,R1,R6			; r1 = r1 and r6 -> masking all Green Values [7:5]
			ADD       R1,R2,R1,LSR #3   ; r1 = r2 + r1 -> ADD r1[masked Green Values >> 3] and r2[masked Red Values]
			STR       R1,[R0]           ; Write masked Red and Green Values
JUMP6		CMP       R5,#0x00009600    ; while (i == 38400)
			BCC       JUMP10
			LDR       R0,=0x20202020    ; r0 = 0x20202020 
			SUB       R6,R6,R0			; r6 = r6 - r0 -> 0xE0E0E0E0 - 0x20202020 = 0xC0C0C0C0
			MOV       R5,#0x00000000    ; i = 0
			B         JUMP9
JUMP8		MOV       R1,R5             ; r1 = i
			ADD       R5,R5,#0x00000001 ; i++
			ADD       R0,R10,R1,LSL #2  ; r0 = #0x40400000 + i * 4
			MOV       R1,R4				; r1 = j
			ADD       R4,R4,#0x00000001 ; j++
			LDR       R2,[R0]			; r2 = Read Masked Red Green Value 4byte [0x40600000 + i * 4]
			LDR       R1,[R8,R1,LSL #2] ; r1 = Read Blue Value 4byte [0x40400000 + j * 4]
			AND       R1,R1,R6			; r1 = r1 and r6 -> masking Blue Values [7:6]
			ADD       R1,R2,R1,LSR #6	; r1 = r2 + r1 -> ADD r1[masked Blue Values >> 6] and r2[masked Red Green Values]
			STR       R1,[R0]			; Write Compressed RGB Values
JUMP9		CMP       R5,#0x00009600	; while (i == 38400)
			BCC       JUMP8
			END