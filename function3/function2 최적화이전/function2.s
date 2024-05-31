		AREA     RESET, CODE, READONLY
		ENTRY
		MOV       R5,#0x40000036		;r5 : ReadPointer  0x40000036
		LDR       R6,=0x40400000		;r6 : WritePointer 0x40400000
		MOV       R4,#0x00000000		;r4 : i = 0
		B         JUMP1
JUMP10  LDRB      R0,[R5,R4,LSL #2]		;r0 : Read Red Value
		MOV       R1,#0x00000001		
		ADD       R1,R1,R4,LSL #2       
		LDRB      R1,[R5,R1]            ;r1 : Read Green Value 
		CMP       R0,R1					;Compare Red and Green
		BGE       JUMP2					;If Red > Green then Branch JUMP2
		LDRB      R0,[R5,R4,LSL #2]     ;r0 : Read Red Value
		MOV       R1,#0x00000002        
		ADD       R1,R1,R4,LSL #2       
		LDRB      R1,[R5,R1]            ;r1 : Read Blue Value
		CMP       R0,R1					;Compare Red and Blue
		BGE       JUMP3					;if Red > Blue then Branch JUMP3
		LDRB      R0,[R5,R4,LSL #2]		;r0 : Read Red Value  (MIN : RED)
		B         JUMP4
JUMP3   MOV       R0,#0x00000002		;JUMP3 (Red > Blue)
		ADD       R0,R0,R4,LSL #2
		LDRB      R0,[R5,R0]			;r0 : Read Blue Value (MIN : BLUE)
		B         JUMP4					;JUMP4
JUMP2   MOV       R0,#0x00000001        ;JUMP2 (Red > Green)
		ADD       R0,R0,R4,LSL #2       
		LDRB      R0,[R5,R0]            ;r0 = Reed Green Value 
		MOV       R1,#0x00000002		
		ADD       R1,R1,R4,LSL #2		
		LDRB      R1,[R5,R1]			;r1 = Reed Blue Value
		CMP       R0,R1					;Compare Green and Blue
		BGE       JUMP5					;IF Green > Blue then Branch JUMP5
		MOV       R0,#0x00000001
		ADD       R0,R0,R4,LSL #2
		LDRB      R0,[R5,R0]			;r0 : Read Green Value (MIN : GREEN)
		B         JUMP4
JUMP5   MOV       R0,#0x00000002        
		ADD       R0,R0,R4,LSL #2		
		LDRB      R0,[R5,R0]			;r0 = Blue Value  (MIN : BLUE)
JUMP4   MOV       R7,R0					;r7 = Min Value
		LDRB      R0,[R5,R4,LSL #2]     ;r0 = Red Value   
		MOV       R1,#0x00000001		
		ADD       R1,R1,R4,LSL #2       
		LDRB      R1,[R5,R1]            ;r1 = Green Value
		CMP       R0,R1					;Compare Red and Green Value
		BLE       JUMP6					;If Red < Green then JUMP4
		LDRB      R0,[R5,R4,LSL #2]     ;r0 = Red Value 
		MOV       R1,#0x00000002		
		ADD       R1,R1,R4,LSL #2		
		LDRB      R1,[R5,R1]			;r1 = Blue Value
		CMP       R0,R1					;Compare Red and Blue 
		BLE       JUMP7					;if Red < Blue then JUMP7
		LDRB      R0,[R5,R4,LSL #2]     ;r0 = Red Value (MAX : RED)
		B         JUMP8
JUMP7   MOV       R0,#0x00000002
		ADD       R0,R0,R4,LSL #2		
		LDRB      R0,[R5,R0]			;r0 : Blue Value (MAX : BLUE)
		B         JUMP8			
JUMP6   MOV       R0,#0x00000001
		ADD       R0,R0,R4,LSL #2
		LDRB      R0,[R5,R0]            ;r0 : Green Value
		MOV       R1,#0x00000002
		ADD       R1,R1,R4,LSL #2
		LDRB      R1,[R5,R1]            ;r1 : Blue Value
		CMP       R0,R1                 ;Compare Green and Blue
		BLE       JUMP9					;if Green < Blue then JUMP9
		MOV       R0,#0x00000001
		ADD       R0,R0,R4,LSL #2
		LDRB      R0,[R5,R0]            ;r0 : Green Value (MAX : Green)
		B         JUMP8
JUMP9   MOV       R0,#0x00000002
		ADD       R0,R0,R4,LSL #2
		LDRB      R0,[R5,R0]            ;r0 : Blue Value (MAX : RED)
JUMP8   MOV       R8,R0					;r8 = MaxValue
		ADD       R0,R7,R8				;r0 = MinValue + MaxValue
		ADD       R1,R0,R0,LSR #31		;r1 = r0 + (r0 / 2^31)
		MOV       R1,R1,LSL #23         ;r1 = r1 * 2^23
		MOV       R2,R1,LSR #24         ;r2 = r1 / 2^24 
		MOV       R1,R4					;r1 = i
		ADD       R4,R4,#0x00000001     ;i = i + 1
		STRB      R2,[R6,R1]			;Write [R2+i]
JUMP1   LDR       R0,=153600
		CMP       R4,R0
		BCC       JUMP10
		END