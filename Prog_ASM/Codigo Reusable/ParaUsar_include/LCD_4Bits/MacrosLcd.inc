
	Disp_ON			EQU		0x01
	Disp_OFF		EQU		0x00
	Cursor_ON		EQU		0x01
	Cursor_OFF		EQU		0x00
	Cursor_BLINK	EQU		0x01
	Cursor_NBLINK	EQU		0x00
	RIGHT			EQU		0x01
	LEFT			EQU		0x00
	Curs_LEFT		EQU		0x00
	Curs_RIGHT		EQU		0x01
	Disp_FIX		EQU		0x00
	Disp_MOVE		EQU		0x01

;*******************	LcdClear	***************************
;	Borra la pantalla del LCD
;**************************************************************
LcdClear	MACRO	Valor

	movlw		B'00000001'
	call		LcdSendCmd

			ENDM
;**************************************************************

;*******************	LcdHome		***************************
;	Situa el cursor en el inicio
;**************************************************************
LcdHome		MACRO	Valor

	movlw		B'00000010'
	call		LcdSendCmd

			ENDM
;**************************************************************

;*******************	LcdSetXY	***************************
;	Situa el cursor en X,Y
;**************************************************************
LcdSetXY	MACRO	X,Y
	VARIABLE	Aux

	IF		X == .2
	Aux = Y | B'11000000'
	ELSE
	Aux = Y | B'10000000'
	ENDIF

	movlw		Aux
	call		LcdSendCmd

			ENDM
;**************************************************************

;*******************	LcdSetDisp	***********************
;	Fija las propiedades del display y del cursor, las opciones son
;	las siguientes:
;
;	Disp_ON			Para encender el display
;	Disp_OFF		Para que el display este apagado
;	Cursor_ON		El cursor esta encendido
;	Cursor_OFF		El cursor esta apagado
;	Cursor_BLINK	El cursor esta parpadeando
;	Cursor_NBLINK	El cursor no esta parpadeando
;**************************************************************
LcdSetDisp	MACRO	Pa,Pb,Pc

	VARIABLE	Aux =B'00001000'
	VARIABLE	Aux1=0x00
	Aux1 = Pa*.4
	Aux = Aux | Aux1
	Aux1 = Pb*.2
	Aux = Aux | Aux1
	Aux1 = Pc*.1
	Aux = Aux | Aux1
	movlw		Aux
	call		LcdSendCmd
			ENDM
;**************************************************************

;*******************	LcdSH_Disp	***************************
;	Desplaza el display hacia la izquierda o la derecha, 
;	utiliza los parametros LEFT or RIGHT para fijar el lado.
;**************************************************************
LcdSH_Disp	MACRO	Dir
	
	VARIABLE	Aux = 0x00
	Aux = Dir*.4					;Ubicamos el valor pasado
									;en la posicion del bit 2 
	Aux = B'00011000' | Aux
	movlw		Aux
	call		LcdSendCmd
		ENDM
;**************************************************************

;*******************	LcdSetMode	***********************
;	Fija la direccion de movimiento del cursor y el display
;	las opciones son las siguientes:
;
;	Curs_LEFT		El cursor se mueve a la izquierda
;	Curs_RIGHT		El cursor se mueve a la derecha
;	Disp_FIX		Para que el display se quede quieto
;	Disp_MOVE		Para que el display se mueva hacia el lado
;					contrario del Cursor
;**************************************************************
LcdSetMode	MACRO	Pa,Pb
	VARIABLE	Aux = 0x00
	
	Aux = Pa*.2 + Pb
	Aux = Aux | B'00000100'
	movlw		Aux
	call		LcdSendCmd
			ENDM
;**************************************************************


;*******************	LcdSH_Curs	**************************
;	Ddesplaza el cursor hacia la izquierda o la derecha,
;	utiliza los parametros LEFT or RIGHT para fijar el lado.
;**************************************************************
LcdSH_Curs	MACRO	Dir
	
	VARIABLE	Aux = 0x00
	Aux = Dir*.4					;Ubicamos el valor pasado
									;en la posicion del bit 2 
	Aux = B'00010000' | Aux
	movlw		Aux
	call		LcdSendCmd
		ENDM
;*************************************************************

;*************************************************************
;	Macros para manejar la linea RW
;*************************************************************
BorrarRW	MACRO

	bcf		LcdPortCtr,LcdRW		
			ENDM

SetRW		MACRO

	bsf		LcdPortCtr,LcdRW	
			ENDM
;*************************************************************

;*************************************************************
;	Macros para manejar la linea RS
;*************************************************************
BorrarRS	MACRO

	bcf		LcdPortCtr,LcdRS	
			ENDM

SetRS		MACRO

	bsf		LcdPortCtr,LcdRS
			ENDM
;*************************************************************

;*************************************************************
;	Macros para manejar la linea E
;*************************************************************
BorrarEn	MACRO

	bcf		LcdPortCtr,LcdEn	
			ENDM

SetEn		MACRO

	bsf		LcdPortCtr,LcdEn
			ENDM
;*************************************************************