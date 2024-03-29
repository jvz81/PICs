;Rutina para un LCD de 16x2 para LCD Hitachi HD44780
;Esta rutina es para el 16F87x.
;Comunicaci�n 8 Bit, RS, RW y E.
;
;Configuraci�n del hardware:
;RB0 a DB0; RB1 a DB1;... RB7 a DB7
;RA0 a RS; RA1 a R/W; RA2 a E
;
;*************
;SE NECESITA:
;*************
;
;* Rutina de temporizaci�n de 5ms llamada DELAY_5MS. Lo debe definir el usuario para no modificar constantemente la rutina
;                                                    as� se puede usar con 8Mhz, 4Mhz, etc.
;* RAM: cursor (ver MOV_DESP)
;*      curdisp (ver CUR_DISP)
;*      poeslcd (ver PO_ES_LCD)
;
;***************
;LO QUE CONSUME
;***************
;- 3 pociciones de la RAM
;- 2 pociciones de la pila m�s uno desde el programa principal, Total 3 Stack.
;
;*********
;RUTINAS:
;*********
;
;LCD_PORT, para configurar los puerto de comunicaci�n con el LCD. Ver encabezado de la rutina.
;
;LCD_INI, inicializa el LCD para bus de 8 bits, las 2 l�neas de LCD y resoluci�n 5x7. Se ejecuta autom�ticamente, pero se dispone de ella
;         por si el usuario lo necesita.
;
;B_DISPLAY, Borra el display y la memoria del LCD.
;
;MOV_DESP, configura al cursor, tanto el movimiento y desplazamiento. B'000001MS': M 1 Incremento 0 decremento del cursor.
;          S 1 Desplaza la visualizaci�n (�til para cuando se exede los 16 caracteres por l�nea) 0 modo normal.
;          antes de llamar a esta rutina, cargar en la pocici�n CURSOR la configuraci�n deseada. Predeterminado B'00000110'
;
;CUR_DISP, Se configura si se prende o apaga al cursor y al display, y si el cursor parpadea o no.
;          B'00001DCB' D 1 Display encendido 0 Display apagado. C 1 cursor encendido 0 cursor apagado. B 1 Cursor parpadea 0 cursor no parpadea.
;          antes de llamar a esta rutina, cargar en la pocici�n CURDISP la configuraci�n deseada. Predeterminado B'00001100'
;
;PO_ES_LCD, Pocici�n para escribir en LCD. La primer L�nea comienza desde H'80' hasta H'8F'
;           y la segunda l�nea desde H'0C' hasta H'CF' �til para subir o bajar el cursor y elegir desde d�nde se empieza a escribir.
;           antes de llamar a esta rutina, carga en la pocici�n POESLCD la configuraci�n deseada. Predeterminado B'10000000' (H'80') y B'00001100' (H'0C')
;
;LCD_DATO, Env�a los datos a mostrar en la pantalla del LCD.
;
;PRE_LCD, configura al LCD en forma predeterminada con los valores B'00000110' B'00001100' B'10000000'; Tambi�n borra el Display. Al utilizar esta rutina
;         se ahorran muchos pasos.
;
;***********************************************************
;MODO DE USO
;***********************************************************
;
;Alternativa 1
;*************
;Primero: ejecutar la rutina LCD_PORT
;Segundo: solo si se necesita, ejecutar la rutina B_DISPLAY es recomendable. (ver en rutinas)
;Tercero: cargar en la variable CURSOR la configuraci�n deseada (ver en RUTINAS) y luego ejecutar la rutina MOV_DESP
;Cuarto: cargar en la variable CURDISP la configuraci�n deseada (ver en RUTINAS) y luego ejecutar la rutina CUR_DIP
;Quinto: cargar en la variable POESLCD la configuraci�n deseada (ver en RUTINAS) y luego ejecutar la rutina PO_ES_LCD
;Sexto: el LCD ya est� configurado. Se debe cargar en el registro W el c�digo ASCII a mostrar en el LCD y luego ejecutar la rutina LCD_DATO.
;S�ptimo: Cuando se cargaron las 16 pociciones de memoria y/o se quiere bajar o subir de l�nea repetir el quinto paso y luego el sexto.
;
;Alternativa 2.
;**************
;Primero: ejecutar PRE_LCD
;segundo: el LCD ya est� configurado. Se debe cargar en el registro W el c�digo ASCII a mostrar en el LCD y luego ejecutar la rutina LCD_DATO.
;tercero: Cuando se cargaron las 16 pociciones de memoria y/o se quiere bajar o subir de l�nea repetir el quinto paso y luego el sexto 
;         de la alternativa 1.
;
;***********************************************************
;NOTA:
;***********************************************************
;
;Importante: Todas las rutinas se deben ejecutar con la instrucci�n CALL y las variables se deben escribir en min�scula como est� en
;            el apartado senecesita.
;            Tambi�n es recomendable desactivar las interrupciones antes de ejecutar cualquier rutina.
;
;Esta rutina se puede utilizar tranquilamente en un PIC 16F84, para ello, se deben anular las l�neas MOVLW  H'06' y MOVWF ADCON1
;BCF  STATUS,RP1 que se encuentra en la rutina LCD_PORT
;
;*****************************************************************************
;Configura los puertos A y B para la comunicaci�n con el LCD
;Pone al PORTB como salida y a los BITs o, 1, 2, 5 del puerto A como salidas y
;a los BITs 3 y 4 como entradas.
;*****************************************************************************
;
;*****************************************************************************
;RENOMBRAMOS PUERTOS
;*****************************************************************************
;
RA		EQU	H'05'
RB		EQU	H'06'
;
;
LCD_PORT
	BSF		STATUS,RP0	;Cambio de banco a 1
	BCF		STATUS,RP1	;Idem anterior.
    CLRF    TRISB           ;RB se programa como salida
    MOVLW   b'00011000'     ;RA<4:3> se programan como entradas
    MOVWF   TRISA           ;RA<2:0> se programan como salidas
	MOVLW	H'06'		;Cargo el valor 6 al W y lo paso
	MOVWF	ADCON1		;al registro para hacer a porta salidas digitales.
    BCF     STATUS,RP0      ;Selecciona el banco 0 de datos
    BCF     RA,0            ;Desactiva RS del modulo LCD
    BCF     RA,2            ;Desactiva E del modulo LCD
;
LCD_INI		
	MOVLW   b'00111000'
    CALL    LCD_REG         ;Codigo de instruccion
    CALL    DELAY_5MS       ;Temporiza 5 mS.
    MOVLW   b'00111000'
    CALL    LCD_REG         ;Codigo de instruccion
    CALL    DELAY_5MS       ;Temporiza 5 mS.
    MOVLW   b'00111000'
    CALL    LCD_REG         ;Codigo de instruccion
    CALL    DELAY_5MS       ;Temporiza 5 mS.
    RETURN
;
LCD_REG

    BCF     RA,0            ;Desactiva RS (Modo instruccion)
    MOVWF   RB              ;Saca el codigo de instruccion
    CALL    LCD_BUSY        ;Espera a que se libere el LCD
LCD_E

    BSF     RA,2            ;Activa se�al E
    NOP                     ;Espera 1uS
    BCF     RA,2            ;Desactiva se�al E
    RETURN
;
LCD_BUSY 

    BSF     RA,1            ;Pone el LCD en modo lectura
    BSF     STATUS,5        ;Selecciona el Banco 1
    MOVLW   0xFF  
    MOVWF   TRISB           ;Puerta B act�a de entrada
    BCF     STATUS,5        ;Selecciona el Banco 0
    BSF     RA,2            ;Activa el LCD (Se�al E)
    NOP           
L_BUSY

    BTFSC   RB,7            ;Chequea el bit BUSY
    GOTO    L_BUSY          ;Est  a "1" (Ocupado)
    BCF     RA,2            ;Desactiva el LCD (Se�al E)
    BSF     STATUS,5        ;Selecciona el Banco 1
    CLRF    TRISB           ;Puerta B actua como salida
    BCF     STATUS,5        ;Selecciona el Banco 0
    BCF     RA,1            ;Pone el LCD en modo escritura
    RETURN
;
B_DISPLAY

	MOVLW	B'00000001'
	CALL	LCD_REG
	RETURN
;
MOV_DESP

	MOVFW	cursor
	CALL	LCD_REG
	RETURN
;
CUR_DISP

	MOVFW	curdisp
	CALL	LCD_REG
	RETURN
;
PO_ES_LCD	

	MOVFW	poeslcd
	CALL	LCD_REG
	RETURN
;
LCD_DATO 

    BCF     RA,0            ;Desactiva RS (Modo instrucci�n)
    MOVWF   RB              ;Valor ASCII a sacar por RB
    CALL    LCD_BUSY        ;Espera a que se libere el LCD
    BSF     RA,0            ;Activa RS (Modo dato)  
    GOTO    LCD_E           ;Genera pulso en se�al E
;
PRE_LCD

	CALL	LCD_PORT
	CALL	B_DISPLAY
	MOVLW	B'00000110'
	MOVWF	cursor
	CALL	MOV_DESP
	MOVLW	B'00001100'
	MOVWF	curdisp
	CALL	CUR_DISP
	MOVLW	B'10000000'
	MOVWF	poeslcd
	CALL	PO_ES_LCD
	RETURN
;
;****************
;NOTA AL MARGEN:
;****************
;Si se tiene limitada la memoria de programa, se puede eliminar la rutina PRE_LCD
;Tambi�n se puede eliminar las variables y trabajar directamente desde W
;para ello, se debe eliminar la primer l�nea (no la etiqueta) de las rutinas
;MOV_DESP, CUR_DISP, PO_ES_LCD y antes de ejecutar estas rutinas, se debe cargar en W
;la configuraci�n deseada. Recuerden que, si se hace esta modificaci�n, la rutina PRE_LCD, no funcionar�.
;La raz�n de utilizar las variables, es para que el usuario pueda guardar la configuraci�n
;al principio del programa, hacer otras tareas y luego inicializar el LCD.