;********************************************************************************
;	Rutinas de retardo para uso de cristal de 4MHz.	Estas rutinas pueden usarse	*
;	para los PIC de gama media													*
;	Version		v1.00															*
;********************************************************************************
;							SE NECESITA 										*
;********************************************************************************
;	Declarar las siguientes variables:											*
;																				*
;	Cant_ms		2 bytes															*
;	CantSeg		1 byte															*
;																				*
;********************************************************************************
;							LO QUE CONSUME										*
;********************************************************************************
;																				*
;	RAM		3		Posiciones	(Max)											*
;	ROM		23		Posiciones	(Max)											*
;	PILA	3		Posiciones	(Max)											*
;********************************************************************************

;********************************************************************************
;								RUTINAS											*
;********************************************************************************
;	Retardo_ms		genera una retardo de 1 - 65535 ms							*
;	RetardoSeg		Genera una retardo de 1 - 255 segundos						*
;********************************************************************************
;																				*
;********************************************************************************
;								MODO DE USO 									*
;********************************************************************************
;	Como las rutinas estan en un archivo .inc se debe usar la directiva			*
;	#INCLUDE<>, para incluir las rutinas dentro el codigo principal. 			*
;																				*
;	Para realizar la llamada a las subrutinas se pueden utilizar las macros		*
;	creadas en el archivo MacrosRetardo.inc, por ejemplo:						*
;																				*
;	Delay_ms	.1500			;Generara una demora de 1500 ms					*
;	Delay_seg	.60				;Genera una demora de 60 segundos				*
;********************************************************************************
;																				*
;********************************************************************************
;	Revisiones																	*
;																				*
;	Rev 1	02/11/2010:	Correccion y mejora de los comentarios 					*
;																				*
;********************************************************************************



;***************************	RetardoSeg	*************************************
;	RetardoSeg	La subrutina genera una espera en segundos de acuerdo al valor	*
;				de CantSeg (1s - 255s), Si se recibe el valor de cero generara	*
;	un retardo de 255 seg														*
;********************************************************************************

RetardoSeg		
	movlw		0xE8			;cargamos las variables usadas para Retardo_ms 
	movwf		Cant_ms			;con 1000 (0x03E8)		
	movlw		0x03
	movwf		Cant_ms+1
	call		Retardo_ms		;generamos la espera de 1000 ms 
	decfsz		CantSeg			;CantSeg --,CantSeg == 0??
	goto		RetardoSeg		;no => seguimos generando retardos de 1 seg
	return						;si => retornamos
;********************************************************************************

;*****************************	Retardo_ms	*************************************
;	Retardo_ms	La subrutina genera una espera en mili segundos  de acuerdo		*
;				al valor en Cant_ms+1 y Cant_ms (1ms - 65535ms), si se recibe	*
;	el valor cero , en realidad se genera un retardo de 65536 ms.				*
;********************************************************************************
Retardo_ms

Ciclo1ms
	movlw		0xF8
Ciclo4us
	addlw		0xFF			;Se decrementa W
	btfss		STATUS,Z		;W==0??
	goto		Ciclo4us		;no => aun no paso 1 ms
								;si => ya paso 1 ms y podemos controlar
								;si ya se  termino la espera
	movf		Cant_ms,F		;
	btfsc		STATUS,Z		;Cant_ms == 0??
	decf		Cant_ms+1,F		;si => Cant_ms+1 -- (parte alta)
	decf		Cant_ms,F		;no => Cant_ms -- (parte baja)
	movf		Cant_ms+1,F		;
	btfss		STATUS,Z		;Cant_ms+1 == 0??
	goto		Ciclo1ms		;no => se sigue en el ciclo
	movf		Cant_ms,F		;si => revisamos Cant_ms
	btfsc		STATUS,Z		;Cant_ms == 0??
	return						;si => retornamos (Parte alta y baja en cero)
	goto		Ciclo1ms		;no => continuamos en el ciclo
;********************************************************************************


	