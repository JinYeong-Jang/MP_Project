		AREA     RESET, CODE, READONLY
		ENTRY
		LDR r0,Filter			; r0 : masking bits &00FF00FF
		LDR r11,CheckValue      ; 017d0000
		MOV r12,#1				; WHITE
		MOV r13,#0				; BLACK
		LDR r14,Pixels          ; i = 960x640 / 2
		LDR r1,READPOINTER      ; r1 : Read Red   Value Address
		ADD r2,r1,r14,LSL #1    ; r2 : Read Green Value Address
		ADD r3,r2,r14,LSL #1    ; r3 : Read Blue  Value Address
		LDR r4,WRITEPOINTER		; r4 : Write Start Address
LOOP    LDRH r5,[r1],#2         ; Read Red   Value HalfWord (r5 :_ _ R2 R1)
		AND r8,r0,r5,LSL #8     ; Mask   R1 value from r5(LSL #8  -> r5 : _ R2 R1 _ ) -> (r8 : _ R2 _ _ )
		LSL r5,#24		     	; Remove R2 value from r5(LSL #24 -> r5 : _ R2 R1 _ ) -> (r8 : R1 _ _ _ )
		ADD r8,r8,r5,LSR #24	; ADD r8 and LSR #24 r5 to r8       (r8 : _ R2 _  R1)
		LDRH r6,[r2],#2		    ; Read Green Value HalfWord (r6 :_ _ R2 R1)
		AND r9,r0,r6,LSL #8     ; Mask   G1 value from r6(LSL #8  -> r6 : _ G2 G1 _ ) -> (r9 : _ G2 _ _ )
		LSL r6,#24			    ; Remove G2 value from r6(LSL #24 -> r6 : _ G2 G1 _ ) -> (r9 : G1 _ _ _ )
		ADD r9,r9,r6,LSR #24	; ADD r8 and LSR #24 r6 to r9       (r9 : _ G2 _  G1)
		LDRH r7,[r3],#2			; Read Blue Value  HalfWord (r7 :_ _ G2 G1)
		AND r10,r0,r7,LSL #8    ; Mask   B1 value from r7(LSL #8  -> r7 : _ B2 B1 _ ) -> (r10 : _ B2 _ _ )
		LSL r7,#24			    ; Remove B2 value from r7(LSL #24 -> r7 : _ B2 B1 _ ) -> (r10 : B1 _ _ _ )
		ADD r10,r10,r7,LSR #24	; ADD r8 and LSR #24 r7 to r10      (r10 : _ R2 _ R1)
		ADD r8,r8,r9            ; ADD r8 and r9 to r8        (r8 : _ R2+G2    _ R1+G1    )
		ADD r8,r8,r10			; ADD r8 and r10 to r8       (r8 : _ R2+G2+B2 _ R1+G1+B1 )
		MOV r7,r8,LSL #16		; MOV r7 from LSL #16 r8     (r7 : _ R1+G1+B1 _    _     )
		CMP r11,r7,LSR #16		; Compare 381 and LSL #16 r7 (r7 : _    _     _ R1+G1+B1 )
		STRBPL r13,[r4],#1	    ; If 381 >= R+G+B Write BLACK
		STRBMI r12,[r4],#1	    ; If 381 < R+G+B Write WHITE
		CMP r11,r8,LSR #16      ; Compare 381 and LSL #16 r8 (r8 : _    _     _ R2+G2+B2 )
		STRBPL r13,[r4],#1		; If 381 >= R+G+B Write BLACK
		STRBMI r12,[r4],#1		; If 381 < R+G+B Write WHITE
		SUBS r14,#1       	    ; i--
		BNE LOOP                ; While( i >= 0 ) LOOP
READPOINTER  	  DCD 	  &40000000
WRITEPOINTER      DCD     &40200000
CheckValue        DCD     381
Pixels            DCD     76800
Filter            DCD     &00FF00FF
        END