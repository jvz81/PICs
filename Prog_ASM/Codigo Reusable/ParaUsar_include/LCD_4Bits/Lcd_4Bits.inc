;********************************************************************************
;	Rutinas para el manejo de LCD en modo 4 bits para un controlador 			*
;	compatible con el HD44780U.													*
;********************************************************************************
;																				*
;*******************	Configuracion para el Hardware	*************************
;	Los 4 pines para el bus de datos pueden ubicarse en la parte alta o en la 	*
;	parte baja de cualquier puerto, esto seria (suponiendo que los pines de		*
;	datos se conectaran en el Puerto B):										*
;																				*
;	Para la parte alta:		RB4	-->	D4 (pin 11)									*
;							RB5	-->	D5 (pin 12)									*
;							RB6	-->	D6 (pin 13)									*
;							RB7	-->	D7 (pin 14)									*
;																				*
;	Para la parte baja:		RB0	-->	D4 (pin 11)									*
;							RB1	-->	D5 (pin 12)									*
;							RB2	-->	D6 (pin 13)									*
;							RB3	-->	D7 (pin 14)									*
;																				*
;	Los pines de control del LCD pueden situarse en cualquier Puerto, puede ser	*
;	diferente al puerto para los pines de datos, ademas puede usarse cualquier	*
;	pin del puerto seleccionado, pero los 3 pines de control deben estar en el	*
;	mismo puerto.																*
;																				*
;	Los pines de control del LCD son:											*
;																				*
;	RS(pin 4) Register Select,		RS = 1, para leer o escribir un dato		*
;						 			RS = 0, Para escribir un comando			*
;	R/W(pin 5)Read/Write Select,	R/W = 1, Modo de lectura					*
;						   			R/W = 0, Modo de escritura					*
;	E (pin 6) Enable signal, 		inicia la lectura o escritura de datos		*
;																				*
;	La libreria permite escoger si se utiliza el pin R/W o no, si no se 		*
;	piensa usar debe ir conectado a tierra										*
;********************************************************************************

;********************************************************************************
;							SE NECESITA 										*
;********************************************************************************
;	Declarar las siguientes variables:											*
;																				*
;	LcdChar		1 byte															*
;	LcdTemp		1 byte															*
;	LcdAddr		1 byte															*
;																				*
;********************************************************************************
;							LO QUE CONSUME										*
;********************************************************************************
;																				*
;	RAM		4		Posiciones	(Max)											*
;	ROM		111		Posiciones	(Max)											*
;	PILA	3		Posiciones	(Max)											*
;																				*
;	La cantidad de ROM usada dismimuye a menos de 86 si no se usa R/W			*
;********************************************************************************
;																				*
;********************************************************************************
;							RUTINAS												*
;********************************************************************************
;	LcdReadByte		Lee un byte desde el bus de datos							*
;																				*
;	LcdEnviarNible	Envia un nible al LCD, usa LcdReadByte						*
;																				*
;	LcdEnviarByte	Envia un byte al LCD, usa LcdReadByte y LcdEnviarByte		*
;																				*
;	IniciarLcd		Inicializa el LCD en modo 4 bits, con el cursor				*
;					encendido y parpadeando										*
;																				*
;	LcdSendChar		Envia un caracter al LCD									*
;																				*
;	LcdSendCmd		Envia un comando al LCD										*
;********************************************************************************
;																				*
;********************************************************************************
;							MODO DE USO 										*
;********************************************************************************
;	Antes de poder incluir esta libreria en el programa se debe incluir la 		*
;	libreria Retardo4M.inc si es que se va a utilizar un oscilador de 4 MHz		*
;	o en su defecto se debe incluir algunas rutinas de retardo para 1 ms, 5 ms	*
;	y 25 ms.																	*
;																				*
;	Como las rutinas estan en un archivo .inc se debe usar la directiva			*
;	#INCLUDE<>, para incluir las rutinas dentro el codigo principal. 			*
;																				*
;	Tambien se debe hacer un include al archivo MacrosLcd.inc, porque 			*
;	contiene macros para llamar a algunas de las subrutinas, 					*
;																				*
;	la primera subrutina que se debe llamar es IniciarLcd la cual por 			*
;	defecto inicializara el LCD con el cursor encendido y parpadeando			*
;	y un desplazamiento  hacia la izquierda cada vez que se escriba				*
;																				*
;	Para hacer modificaciones en la configuracion del LCD puede usarse			*
; 	algunas de las Macros del archivo MacrosLcd.inc como ser:					*
;																				*
;	LcdClear		Borra la pantalla del LCD									*
;	LcdHome			Situa el cursor en el inicio								*
;	LcdSetXY		Situa el cursor en la posicion X,Y							*
;	LcdSetDisp		Fija propiedades del display y del cursor.					*
;	LcdSetMode		Fija la direccion de movimiento del cursor y el display		*
;	LcdSH_Disp		Desplaza el display hacia la izquierda o la derecha			*
;	LcdSH_Curs		Ddesplaza el cursor hacia la izquierda o la derecha			*
;********************************************************************************
;																				*
;********************************************************************************
;	Revisiones																	*
;																				*
;	Rev 1	02/11/2010:	Correccion y mejora de los comentarios 					*
;																				*
;********************************************************************************
;********************************************************************************

;*******************************	DEFINICIONES	*****************************
;	En esta seccion se definen los puertos a usarse y otras condiciones para el	*
;	ensamblado condicional														*
;********************************************************************************
FALSE			EQU		0x00
TRUE			EQU		0x01



DataPORT_Up		EQU		FALSE		;Si DataPORT_Up es TRUE se usaran los bits 
									;4 al 7 del PORT elegido

UseRW			EQU		TRUE		;Si UseRW es TRUE se usa la subrutina
									;LcdReadByte, en otro caso el pin RW del
									;LCD debe ir a cero.


LcdTris			EQU		TRISA		;TRIS para los pines de datos
LcdTrisCtr		EQU		TRISA		;TRIS para los pines de control

LcdPort			EQU		PORTA		;PORT para los pines de datos
LcdPortCtr		EQU		PORTA		;TRIS para los pines de control

LcdEn			EQU		.7			;Se indica el numero de bit del puerto a usar
LcdRW			EQU		.6
LcdRS			EQU		.4						
;********************************************************************************


;***************************	LcdReadByte		*********************************
;	LcdReadByte		Permite leer un valor desde el bus de datos del	LCD, el		*
;					 valor leido es devuelto mediante el registro W,			*
;	internamente se usa la variable LcdTemp, se utiliza  una directiva de 		*
;	pre procesado para que esta subrutina solo se ensamble en caso que UseRW 	*
;	sea TRUE.																	*
;********************************************************************************
	IF	UseRW
LcdReadByte
							;Ponemos en cero el puerto del LCD
	IF	DataPORT_Up
	movlw	0xF0			;para poner en cero solo la parte alta del puerto
	ELSE
	movlw	0x0F			;para poner en cero solo la parte baja del puerto
	ENDIF
	andwf	LcdPort,F
	BorrarRS
	BorrarRW
	BorrarEn

	bsf		STATUS,RP0		;Banco 1

	IF	DataPORT_Up
	movlw	0xF0			;para poner como entrada solo la parte alta del puerto
	ELSE
	movlw	0x0F			;para poner como entrada solo la parte baja del puerto
	ENDIF			

	iorwf	LcdTris,F		;Configuramos como entrada la parte de datos
	bcf		STATUS,RP0		;Banco 0 

	SetRW					;Pasamos al modo de lectura
	SetEn					;Habilitamos LCD
							;leemos la parte alta
	IF	DataPORT_Up
	movf	LcdPort,W		;Aqui leemos la parte alta directamente
	ELSE
	swapf	LcdPort,W		;hacemos swap para que el valor leido quede en la parte alta
	ENDIF

	andlw	0xF0			;no nos interesa los bits inferiores
	movwf	LcdTemp			;la guardamos en LCDTemp (parte alta)

	BorrarEn				;Deshabilitamos LCD
	SetEn					;Volvemos a habilitar el LCD
							;leemos la parte baja
	IF	DataPORT_Up
	swapf	LcdPort,W		;leemos la parte baja haciendo swapf
	ELSE
	movf	LcdPort,W
	ENDIF
	andlw	0x0F			;Solo nos interesa la parte baja
	iorwf	LcdTemp,F		;y lo guardamos	en LcdTemp

	BorrarEn				;Deshabilitamos LCD

	bsf		STATUS,RP0		;Banco 1

	IF	DataPORT_Up
	movlw	0x0F			;para poner como salida solo la parte alta del puerto
	ELSE
	movlw	0xF0			;para poner como salida solo la parte baja del puerto
	ENDIF			

	andwf	LcdTris,F		;Configuramos como salida la parte de datos

	bcf		STATUS,RP0		;Banco 0 
	movf	LcdTemp,W		;devolvemos el valor leido en W
	return

	ENDIF
;********************************************************************************
	

;***************************	LcdEnviarNible		*****************************
;	LcdEnviarNible	Envia un nible recibido en W al bus de datos del LCD, 		*
;					recibe el dato en el formato B'xxxxdddd', y utiliza LcdTemp	*
;	se trata de no afectar los bits del PORT que  podrian estar usandose para 	*
;	otra cosa 																	*	
;********************************************************************************
LcdEnviarNible		
	movwf	LcdTemp			;
	IF	DataPORT_Up
	swapf	LcdTemp,W		;si usamos la parte alta del PORT debemos 
							;intercambiar
	andlw	0xF0			;mascara para tomar solo la parte baja del dato
	ELSE
	andlw	0x0F			;Tomamos la parte baja del dato	directamente
	ENDIF

	movwf	LcdTemp			;
	movf	LcdPort,W		;Leemos LcdPort para cuidar los bits restantes
	IF	DataPORT_Up
	andlw	0x0F			;la parte baja podria usarse en otra cosa
	ELSE
	andlw	0xF0			;la parte alta podria usarse en otra cosa
	ENDIF
	iorwf	LcdTemp,W		;actualiza el dato que se colocara al bus con
	movwf	LcdPort			;los bits bajos anteriores, y actualizamos 
							;el puerto del LCD

	SetEn					;Habilitamos LCD
	BorrarEn				;Deshabilitamos LCD
	return
;********************************************************************************

;***********************	LcdEnviarByte	*************************************
;	LcdEnviarByte	Envia un byte recibido en W al bus de datos, envia un nible	*
;					a la vez, pero antes se revisa el bit busy esperando a que 	*
;	este se ponga en 1 o se realiza una espera de 2 ms para garantizar que el	*
;	LCD este desocupado, para elegir entre las	dos opciones se usa UseRW, en 	*
;	la subrutina tambien se determina si se	enviara un dato o un comando, la 	*
;	informacion de que enviar la recibe en el bit 0 de LcdAddr, si este bit 	*
;	esta en 1 enviamos un dato y si esta en cero enviamos un comando			*
;********************************************************************************
LcdEnviarByte
	movwf	LcdChar				;Guardamos el dato recibido para enviar 

	BorrarRS					;Por defecto enviamos un comando
LcdSendByte_P0
	IF	UseRW
	call	LcdReadByte			;Leemos el bus de datos y guardamos el 
	movwf	LcdTemp				;valor en LcdTemp
	btfsc	LcdTemp,7			;Bit Busy == 1? (LCD ocupado?)
	goto	LcdSendByte_P0		;si => continuamos esperando
	ELSE
	Delay_ms	.1				;Espera que el LCD este desocupado , se
								;podria bajar a 55 us
	ENDIF

	btfss	LcdAddr,0			;No =>, LcdAddr == 1?(Enviaremos un dato?)
	goto	LcdSendByteCmd		;No => vamos a enviar comando

	SetRS						;para enviar un dato
	goto	LcdSendByte_P1		;Vamos a enviar el byte
LcdSendByteCmd
	BorrarRS					;para poder enviar un comando

LcdSendByte_P1		
	IF	UseRW
	BorrarRW					;Modo de escritura
	ENDIF
	BorrarEn					;Deshabilitamos LCD
	swapf	LcdChar,W			;Enviamos primero la parte alta
	call	LcdEnviarNible
	goto	$+1					;Espera para que el LCD procese el anterior
	goto	$+1					;nible
	movf	LcdChar,W			;Luego enviamos la parte baja

	call	LcdEnviarNible
	BorrarRS
	return
;********************************************************************************

;*****************************	IniciarLcd	*************************************
;	IniciarLcd	Realiza el procedimiento necesario para configurar el LCD en su	*
;				modo de 4 bits, ademas configura los parametros basicos para el	*
;	despliegue. 																*
;********************************************************************************

IniciarLcd

	bsf		STATUS,RP0			;Banco 1

	IF	DataPORT_Up
	movlw	0x0F				;para poner como salida solo la parte alta del puerto
	ELSE
	movlw	0xF0				;para poner como salida solo la parte baja del puerto
	ENDIF			
	andwf	LcdTris,F			;Configuramos como salida la parte de datos

	bcf		LcdTrisCtr,LcdEn	;Los pines del puerto de control como salida
	bcf		LcdTrisCtr,LcdRS	;Los pines del puerto de control como salida
	IF	UseRW
	bcf		LcdTrisCtr,LcdRW	;Los pines del puerto de control como salida
	ENDIF

	bcf		STATUS,RP0			;Banco 0 
	clrf	LcdTemp				;Iniciamos las variables
	
	IF	DataPORT_Up
	movlw	0x0F				;para poner en cero solo la parte alta del puerto
	ELSE
	movlw	0xF0				;para poner en cero solo la parte baja del puerto
	ENDIF

	andwf	LcdPort,F			;afectamos solo 4 bits del puerto
	
	Delay_ms	.25				;Espera > 20ms despues que VDD llego a 
								;4,5 V
	IF	UseRW
	BorrarRW					;estas dos lineas en cero
	ENDIF

	BorrarRS					;Los siguientes comandos se envian con 
	BorrarEn
							
	movlw	0x03			
	call	LcdEnviarNible		;Enviamos 0x03 a DB7 DB6 DB5 DB4

	Delay_ms	.5				;Espera > a 4.1 ms
	movlw	0x03				;Enviamos 0x03 a DB7 DB6 DB5 DB4
	call	LcdEnviarNible

	Delay_ms	.1				;Espera > a 100 us
	movlw	0x03				;Enviamos 0x03 a DB7 DB6 DB5 DB4
	call	LcdEnviarNible

;	Delay_ms	.5				;Ultima espera (en teoria innecesaria)
	movlw	0x02				;Enviamos 0x02 a DB7 DB6 DB5 DB4
	call	LcdEnviarNible
								;A partir de aqui configuramos las
								;caracteristicas de despliegue enviando 
								;comandos
								
	movlw	B'00101000'			;Configuramos el registro Function Set
	clrf	LcdAddr				;| 0 | 0 | 1 | DL | N | F | x | x |
	call	LcdEnviarByte		;DL = 0, Interfaz 4 bits; N = 0 se usa 1 linea
								;	= 1, Interfaz 8 bits; N = 1 se usan 2 lineas
								;F  = 0, Fuente de 5*8 puntos
								;F  = 1, Funete de 5*11 puntos
								
	movlw	b'00001111'			;Configuramos el registro Display ON/OFF control
	clrf	LcdAddr				;| 0 | 0 | 0 | 0 | 1 | D | C | B |
	call	LcdEnviarByte		;D = 0, Display apagado(los datos se quedan en DDRAM)
								;D = 1, Display encendido
								;C = 0, Cursor apagado pero su posicion se mantiene
								;C = 1, Cursor encendido
								;B = 0, Parpadeo del cursor apagado
								;B = 1, Parpadeo del cursor encendido

	movlw	B'00000001'			;Enviamos el comando de limpiar la pantalla
	clrf	LcdAddr
	call	LcdEnviarByte
	
	movlw	B'00000110'			;Configuramos el registro Entry mode set
	clrf	LcdAddr				;| 0 | 0 | 0 | 0 | 0 | 1 | I/D | SH |
	call	LcdEnviarByte		;I/D = 0, El cursor se mueve a la izquierda
								;I/D = 1, El cursor se mueve a la derecha
								;SH  = 0, Cuando se realiza una operacion sobre DDRAM
								;o CGRAM el display no se mueve
								;SH  = 1, Cuando se escribe en DDRAM todo el display se
								;desplaza de acuerdo al bit I/D

	return
;********************************************************************************

;****************************	LcdSendChar		*********************************
;	LcdSendChar		Envia un caracter (dato) al LCD , recibe el dato en W y 	*
;					pone en 1 el bit cero de LcdAddr, para luego llamar a		*
;	LcdEnviarByte																*
;********************************************************************************
LcdSendChar

	bsf		LcdAddr,.0			;Indicamos que se enviara un Dato
	call	LcdEnviarByte
	return
;********************************************************************************

;****************************	LcdSendCmd		*********************************
;	LcdSendCmd		Envia un comando al LCD , recibe el dato en W y pone en 	*
;					cero el bit cero de LcdAddr, para luego llamar a			*
;	LcdEnviarByte																*
;********************************************************************************
LcdSendCmd

	bcf		LcdAddr,.0			;Indicamos que se enviara un comando
	call	LcdEnviarByte
	return
;********************************************************************************

