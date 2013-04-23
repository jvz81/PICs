;************************************************************************************
;	Este archivo agrupa diversas Macros para los PIC16F. Tenemos macros para 		*
;	realizar condicionales _if_else y algunas para operaciones basicas de 16 bits	*
;************************************************************************************


;************************************************************************************
;	_if_else_Reg1_gt_Reg2	Reg1,Reg2,label											*
;	Esta macro realiza una comparacion entre Reg1 y Reg2. Si Reg1 > Reg2 entonces	*
;	realiza el codigo que esta a continuacion de la macro, caso contrario (else)	*
;	se salta a la etiqueta label.													*
;************************************************************************************

MACRO	_if_else_Reg1_gt_Reg2	Reg1,Reg2,label
	movf		Reg1,W
	subwf		Reg2,W				;Reg2 - Reg1
	btfsc		STATUS,C
	goto		label				;C=1, W<=f (Reg1 <= Reg2)
									;C=0, W>f (Reg1 > Reg2)
ENDM
;************************************************************************************

;************************************************************************************
;	_if_else_Reg1_lt_Reg2	Reg1,Reg2,label											*
;	Esta macro realiza una comparacion entre Reg1 y Reg2. Si Reg1 < Reg2 entonces	*
;	realiza el codigo que esta a continuacion de la macro, caso contrario (else)	*
;	se salta a la etiqueta label.													*
;************************************************************************************
MACRO	_if_else_Reg1_lt_Reg2	Reg1,Reg2,label
	movf		Reg2,W
	subwf		Reg1,W				;Reg1 - Reg2
	btfsc		STATUS,C
	goto		label				;C=1, W<=f (Reg2 <= Reg1)
									;C=0, W>f (Reg1 < Reg2)
ENDM
;************************************************************************************

;************************************************************************************
;	_if_else_Reg1_gte_Reg2	Reg1,Reg2,label											*
;	Esta macro realiza una comparacion entre Reg1 y Reg2. Si Reg1 >= Reg2 entonces	*
;	realiza el codigo que esta a continuacion de la macro, caso contrario (else)	*
;	se salta a la etiqueta label.													*
;************************************************************************************
MACRO	_if_else_Reg1_gte_Reg2	Reg1,Reg2,label
	movf		Reg2,W
	subwf		Reg1,W				;Reg1 - Reg2
	btfss		STATUS,C
	goto		label				;C=0, W>f (Reg2 > Reg1)
									;C=1, W<=f (Reg1 >= Reg2)
ENDM
;************************************************************************************

;************************************************************************************
;	_if_else_Reg1_lte_Reg2	Reg1,Reg2,label											*
;	Esta macro realiza una comparacion entre Reg1 y Reg2. Si Reg1 <= Reg2 entonces	*
;	realiza el codigo que esta a continuacion de la macro, caso contrario (else)	*
;	se salta a la etiqueta label.													*
;************************************************************************************
MACRO	_if_else_Reg1_lte_Reg2	Reg1,Reg2,label
	movf		Reg1,W
	subwf		Reg2,W				;Reg2 - Reg1
	btfss		STATUS,C
	goto		label				;C=0, W>f (Reg1 > Reg2)
									;C=1, W<=f (Reg1 <= Reg2)
ENDM
;************************************************************************************

;************************************************************************************
;	_if_else_Reg1_eq_Reg2	Reg1,Reg2,label											*
;	Esta macro realiza una comparacion entre Reg1 y Reg2. Si Reg1 == Reg2 entonces	*
;	realiza el codigo que esta a continuacion de la macro, caso contrario (else)	*
;	se salta a la etiqueta label.													*
;************************************************************************************
MACRO	_if_else_Reg1_eq_Reg2	Reg1,Reg2,label
	movf		Reg1,W
	xorwf		Reg2,W				;Comparamos Reg1 y Reg2
	btfss		STATUS,Z
	goto		label				;Z=0, W!=f (Reg1 != Reg2)
									;Z=1, W =f (Reg1  = Reg2)
ENDM
;************************************************************************************

;************************************************************************************
;	_if_else_Reg1_neq_Reg2	Reg1,Reg2,label											*
;	Esta macro realiza una comparacion entre Reg1 y Reg2. Si Reg1 != Reg2 entonces	*
;	realiza el codigo que esta a continuacion de la macro, caso contrario (else)	*
;	se salta a la etiqueta label.													*
;************************************************************************************
MACRO	_if_else_Reg1_neq_Reg2	Reg1,Reg2,label
	movf		Reg1,W
	xorwf		Reg2,W				;Comparamos Reg1 y Reg2
	btfsc		STATUS,Z
	goto		label				;Z=1, W =f (Reg1  = Reg2)
									;Z=0, W!=f (Reg1 != Reg2)
ENDM
;************************************************************************************

;************************************************************************************
;	_if_else_Reg_gt_kk	Reg1,Const,label											*
;	Esta macro realiza una comparacion entre un registro (Reg1) y una constante (kk)*
;	Si Reg1 > kk entonces realiza el codigo que esta a continuacion de la macro,	*
;	caso contrario (else) se salta a la etiqueta label.								*
;************************************************************************************
MACRO	_if_else_Reg1_gt_kk		Reg1,Const,label
	movf		Reg1,W
	sublw		Const				;Const - W
	btfsc		STATUS,C
	goto		label				;C=1, W<=K (Reg1 <= kk)
									;C=0, W>K (Reg1 > kk)
ENDM
;************************************************************************************


;************************************************************************************
;	_if_else_Reg_lt_kk	Reg1,Const,label											*
;	Esta macro realiza una comparacion entre un registro (Reg1) y una constante (kk)*
;	Si Reg1 < kk entonces realiza el codigo que esta a continuacion de la macro,	*
;	caso contrario (else) se salta a la etiqueta label.								*
;************************************************************************************
MACRO	_if_else_Reg1_lt_kk		Reg1,Const,label
	movlw		Const
	subwf		Reg1				;Reg1 - W (Reg1 - Const)
	btfsc		STATUS,C
	goto		label				;C=1, W<=f (Reg1 >= kk)
									;C=0, W>f (Reg1 < kk)
ENDM
;************************************************************************************

;************************************************************************************
;	_if_else_Reg_gte_kk		Reg1,Const,label										*
;	Esta macro realiza una comparacion entre un registro (Reg1) y una constante (kk)*
;	Si Reg1 >= kk entonces realiza el codigo que esta a continuacion de la macro,	*
;	caso contrario (else) se salta a la etiqueta label.								*
;************************************************************************************
MACRO	_if_else_Reg1_gte_kk		Reg1,Const,label
	movlw		Const
	subwf		Reg1				;Reg1 - W (Reg1 - Const)
	btfss		STATUS,C
	goto		label				;C=0, W>f (Reg1 < kk)
									;C=1, W<=f (Reg1 >= kk)
ENDM
;************************************************************************************

;************************************************************************************
;	_if_else_Reg_lte_kk		Reg1,Const,label										*
;	Esta macro realiza una comparacion entre un registro (Reg1) y una constante (kk)*
;	Si Reg1 <= kk entonces realiza el codigo que esta a continuacion de la macro,	*
;	caso contrario (else) se salta a la etiqueta label.								*
;************************************************************************************
MACRO	_if_else_Reg1_lte_kk		Reg1,Const,label
	movf		Reg1,W
	sublw		Const				;Const - W
	btfss		STATUS,C
	goto		label				;C=0, W>K (Reg1 > kk)
									;C=1, W<=K (Reg1 <= kk)
ENDM
;************************************************************************************

;************************************************************************************
;	_if_else_Reg_eq_kk		Reg1,Const,label										*
;	Esta macro realiza una comparacion entre un registro (Reg1) y una constante (kk)*
;	Si Reg1 == kk entonces realiza el codigo que esta a continuacion de la macro,	*
;	caso contrario (else) se salta a la etiqueta label.								*
;************************************************************************************
MACRO	_if_else_Reg1_eq_kk		Reg1,Const,label
	movlw		Const
	xorlw		Const,W				;Se compara const con W
	btfss		STATUS,Z
	goto		label				;Z=0, W!=K (Reg1 != kk)
									;Z=1, W==K (Reg1 == kk)
ENDM
;************************************************************************************

;************************************************************************************
;	_if_else_Reg_neq_kk		Reg1,Const,label										*
;	Esta macro realiza una comparacion entre un registro (Reg1) y una constante (kk)*
;	Si Reg1 != kk entonces realiza el codigo que esta a continuacion de la macro,	*
;	caso contrario (else) se salta a la etiqueta label.								*
;************************************************************************************
MACRO	_if_else_Reg1_neq_kk		Reg1,Const,label
	movlw		Const
	xorlw		Const,W				;Se compara const con W
	btfsc		STATUS,Z
	goto		label				;Z=1, W==K (Reg1 == kk)
									;Z=0, W!=K (Reg1 != kk)
ENDM
;************************************************************************************

;************************************************************************************
;	Inc16Bits	Reg
;************************************************************************************

MACRO	Inc16Bits	Reg
	incf		Reg,F
	btfsc		STATUS,Z
	incf		Reg+1
	
ENDM

;************************************************************************************
;	Dec16Bits	Reg
;************************************************************************************
MACRO	Dec16Bits	Reg
	movf		Reg,W
	btfsc		STATUS,Z
	decf		Reg+1,F
	decf		Reg
	
ENDM
