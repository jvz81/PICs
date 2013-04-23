;MACROS PARA HACER COMPARACIONES

;Salta a ir_a si A es mayor a B 
;ir_a debe estar en la misma pagina en la que se llama
;a la macro
	Jmp_AgtB	MACRO	D0,D1,ir_a

	movf		D1+1,W
	movwf		ACCbHI
	movf		D1,W
	movwf		ACCbLO
	movf		D0+1,W
	movwf		ACCaHI
	movf		D0,W
	movwf		ACCaLO
	pagesel		D_sub
	call		D_sub
	pagesel		ir_a
	btfss		carry,.0		;carry == 0(D0>D1)?
	goto		ir_a			;Si => vamos a
								;No => continuamos
				ENDM

;Salta a ir_a si A es menor a B
;ir_a debe estar en la misma pagina en la que se llama
;a la macro
	Jmp_AltB	MACRO	D0,D1,ir_a

	movf		D0+1,W
	movwf		ACCbHI
	movf		D0,W
	movwf		ACCbLO
	movf		D1+1,W
	movwf		ACCaHI
	movf		D1,W
	movwf		ACCaLO
	pagesel		D_sub
	call		D_sub
	pagesel		ir_a
	btfss		carry,.0		;carry == 0(D0 < D1)?
	goto		ir_a			;Si => vamos a
								;No => continuamos
				ENDM





Load_ACCa		MACRO	Const

	movlw		HIGH Const
	movwf		ACCaHI
	movlw		LOW Const
	movwf		ACCaLO

				ENDM

Load_ACCb		MACRO	Const

	movlw		HIGH Const
	movwf		ACCbHI
	movlw		LOW Const
	movwf		ACCbLO

				ENDM





Add16_Vars		MACRO	Var2,Var1

	movf		Var1+1,W
	movwf		ACCaHI
	movf		Var1,W
	movwf		ACCaLO
	
	movf		Var2+1,W
	movwf		ACCbHI
	movf		Var2,W
	movwf		ACCbLO

	call		D_add

	movf		ACCbHI,W
	movwf		Var2+1
	movf		ACCbLO,W
	movwf		Var2

				ENDM