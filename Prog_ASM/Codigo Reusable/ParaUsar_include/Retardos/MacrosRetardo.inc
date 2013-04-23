;***********************	Delay_ms	***********************
;	Esta macro recibe, mediante el parametro Valor, un numero
;	entre 1 y 65535, para hacer la llamada a Retardo_ms
;**************************************************************

	Delay_ms	MACRO	Valor

	movlw		HIGH Valor		;Tomamos la parte alta de Valor
	movwf		Cant_ms+1		;y lo cargamos la parte alta de Cant_ms
	movlw		LOW Valor		;Tomamos la parte baja de Valor
	movwf		Cant_ms			;y lo cargamos la parte alta de Cant_ms
	call		Retardo_ms		;llamamos a la rutina de retardo	

				ENDM
;**************************************************************


;***********************	Delay_seg	***********************
;	Esta macro recibe, mediante el parametro Valor, un numero
;	entre 1 y 255, para hacer la llamada a RetardoSeg
;**************************************************************

	Delay_seg	MACRO	Valor

	movlw		Valor
	movwf		CantSeg
	call		RetardoSeg

				ENDM
;**************************************************************