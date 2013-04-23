
;	Rutina5 de retardo de 5 ms para un cristal de 4MHz
;	Puede usarse  en los PIC de gama media
;	Version		v0.90
;*********************************************************************
;							SE NECESITA 
;*********************************************************************
;	Declara las siguiente variable:
;
;	Cant_ms		1 byte

;*********************************************************************
;							LO QUE CONSUME
;*********************************************************************
;
;	RAM		1		Posiciones	(Max)
;	ROM		11		Posiciones	(Max)
;	PILA	1		Posiciones	(Max)
;*********************************************************************

;*********************************************************************
;							RUTINAS
;*********************************************************************
;	DELAY_5MS		genera una retardo de 5 ms
;*********************************************************************

;*********************************************************************
;							MODO DE USO 
;*********************************************************************
;	Como las rutinas estan en un archivo .inc se debe usar la directiva
;	#INCLUDE<>, para incluir la rutina dentro el codigo principal. 
;
;	La subrutina se llama usando un call	DELAY_5MS
;*********************************************************************


;*************************	DELAY_5MS	********************************
;	DELAY_5MS	La subrutina genera una espera de 5  mili segundos  para
;				un oscilador de 4 MHz. primero inicia la variable Cant_ms 
;	con 5 y luego va realizando retardos de 1 ms mientras  Cant_ms sea 
;	mayor a 0.
;***********************************************************************
DELAY_5MS
	movlw		.5
	movwf		Cant_ms			;Cant_ms = 5
DELAY_5MS_P0
	movlw		0xF8
DELAY_5MS_P1
	addlw		0xFF			;Se decrementa W
	btfss		STATUS,Z		;W==0??
	goto		DELAY_5MS_P1	;no => aun no paso 1 ms
								;si => ya paso 1 ms y revisamos Cant_ms
	movf		Cant_ms,F		;
	btfsc		STATUS,Z		;Cant_ms == 0??
	return						;si => termino la rutina
	decf		Cant_ms,F		;no => Cant_ms -- 
	goto		DELAY_5MS_P0	;y continuamos esperando
	
;***********************************************************************