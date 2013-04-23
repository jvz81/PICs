;********************************************************************************
;********************************************************************************
;																				*	
;	Aqui se presenta varias rutinas para manejar operaciones aritmeticas		*
;	para numeros de 16 bits, estas rutinas fueron sacadas de la hoja de 		*
;	aplicacion de Microchip AN526.												*
;																				*
;	Como en los programas que se utilizan operaciones tambien se suele manejar 	*
;	envio de datos en BCD o ASCII se tienen tambien rutinas que convierten		*
;	de BCD a binario y de binario a BCD.										*
;																				*
;	Entre las rutinas tenemos:													*
;																				*
;********************************************************************************
;	D_add		la rutina original se encuentra en el programa de prueba		*
;				DBL_ADD.asm, maneja la suma y la resta (D_sub) de 16 bits,		*
;	se hizo una modificacion en la parte de la resta aumentando la variable 	*
;	carry, la cual guarda en su bit menos significativo se tiene la 			*
;	informacion del	resultado de la resta, esto para poder hacer comparaciones.	*
;																				*
;	D_add		ACCb = ACCb + ACCa												*
;	D_sub		ACCb = ACCb - ACCa												*
;				carry = 1	Si ACCa <= ACCb (ACCb - ACCa >= 0)					*
;						0	Si ACCa > ACCb (ACCb - ACCa < 0)					*
;********************************************************************************
;																				*
;********************************************************************************
;	D_mpyF		la rutina original se encuentra en el programa de prueba		*
;				DBL_MPYF.asm, al declarar las variables que usa se debe tener	*
;	en cuenta que la variable sign solo se la debe declarar solo si se quiere	*
;	realizar operaciones con signo. Ademas se debe tomar en cuenta que la 		*
;	rutina se ejecuta mas rapido si en ACCb tenemos menos cantidad de bits		*
;																				*
;	D_mpyF		ACCb:ACCc = ACCb * ACCa											*
;********************************************************************************
;																				*
;********************************************************************************
;	FXD3216U	la rutina original se encuentra en el archivo FXD26.A16 que		*
;				pertenece a la hoja de aplicacion AN617.						*
;																				*
;	FXD3216U	AARG = AARG/BARG	,el residuo queda en REM					*
;********************************************************************************
;																				*
;********************************************************************************
;	B2_BCD																		*
;																				*
;	La rutina recibe el numero de 16 bits en H_byte:L_byte y devuelve 5 valores	*
;	en R0,R1 y R2 los datos estan guardados de la siguiente forma:				*
;																				*
;	|	dig5	|	dig4	|	dig3	|	dig2	|	dig1	|				*
; 	|	R0 0:3	|	R1 4:7	|	R1 0:3	|	R2 4:7	|	R2 0:3	|				*
;																				*
;	El nible alto de R0 no se usa												*
;********************************************************************************
;																				*
;********************************************************************************
;	BCDtoB																		*
;																				*
;	La rutina recibe los 5 digitos BCD en R0,R1 y R2, estas variables guardan 	*
;	los valores de la misma forma que B2_BCD, la rutina devuelve el valor 		*
;	binario en 16 bits en las variables H_byte:L_byte							*
;********************************************************************************
;																				*
;********************************************************************************
;	Para estas rutinas se necesitan las siguientes variables					*
;																				*
;	Para	D_add, D_sub	: ACCbHI, ACCbLO, ACCaHI, ACCaLO, carry				*
;	Para	D_mpyF			: Ademas de las anteriores, ACCcHI, ACCcLO,			*
;						  	  ACCdHI ACCdLO y sign(si se quier usar signo)		*
;	Para	FXD3216U		: AARGB0, AARGB1, AARGB2, AARGB3					*
;						 	  BARGB0, BARGB1, REMB0, REMB1, LOOPCOUNT,temp		*
;	Para	B2_BCD			: H_byte, L_byte, R0, R1, R2, count y temp, ademas	*
;							  de usar el FSR e INDF								*
;********************************************************************************
;																				*
;	Para Poder ver estas variables en la ventana watch se debe definir primero	*
;	la parte baja de las variables de 16 bits o 32 bits							*
;
;	ACCaLO,ACCaHI				
;	ACCcLO,ACCcHI				
;	ACCbLO,ACCbHI
;	ACCdLO,ACCdHI
;	carry
;
;	AARGB3,AARGB2,AARGB1,AARGB0	;para la operaciones de division	
;	BARGB1,BARGB0
;	REMB1,REMB0
;	LOOPCOUNT
;	temp
;********************************************************************************
;																				*
;********************************************************************************
;	Revisiones																	*
;																				*
;	Rev 1	02/11/2010:	Correccion y mejora de los comentarios 					*
;																				*
;********************************************************************************

LSB		equ	0


		#define	_C		STATUS,0
		#define	_Z		STATUS,2


;*******************************************************************
;                 Double Precision Addition & Subtraction
;
;*******************************************************************;
;   Addition :  ACCb(16 bits) + ACCa(16 bits) -> ACCb(16 bits)
;      (a) Load the 1st operand in location ACCaLO & ACCaHI ( 16 bits )
;      (b) Load the 2nd operand in location ACCbLO & ACCbHI ( 16 bits )
;      (c) CALL D_add
;      (d) The result is in location ACCbLO & ACCbHI ( 16 bits )
;
;   Performance :
;               Program Memory  :       07
;               Clock Cycles    :       08
;*******************************************************************;
;   Subtraction : ACCb(16 bits) - ACCa(16 bits) -> ACCb(16 bits)
;      (a) Load the 1st operand in location ACCaLO & ACCaHI ( 16 bits )
;      (b) Load the 2nd operand in location ACCbLO & ACCbHI ( 16 bits )
;      (c) CALL D_sub
;      (d) The result is in location ACCbLO & ACCbHI ( 16 bits )
;
;   Performance :
;               Program Memory  :       14
;               Clock Cycles    :       17
;
;
;       Program:          DBL_ADD.ASM 
;       Revision Date:   
;                         1-13-97      Compatibility with MPASMWIN 1.40
;
;*******************************************************************;

;*******************************************************************
;         Double Precision Subtraction ( ACCb - ACCa -> ACCb )
;
D_sub   
	BCF		carry, 0x00
	call    neg_A11           ; At first negate ACCa; Then add
;
;*******************************************************************
;       Double Precision  Addition ( ACCb + ACCa -> ACCb )
;
D_add  
	movf		ACCaLO,W
	addwf		ACCbLO, F       ;add lsb
	btfsc		STATUS,C        ;add in carry
	incf		ACCbHI, F
	movf		ACCaHI,W
	addwf		ACCbHI, F       ;add msb
	BTFSC		STATUS,C
	BSF			carry, 0x00	
	retlw		0
;
;
neg_A11
	comf		ACCaLO,F
	comf		ACCaHI,F
	incf		ACCaLO,F
	btfss		STATUS,Z		;al incrementar se produce acarreo?
	retlw		.0				;No => retornamos
	incf		ACCaHI,F
	btfss		STATUS,Z		;al incrementar se produce acarreo?
	retlw		.0				;No => retornamos
	bsf			carry,0			;Si => indicamos eso en el acarreo
	retlw		.0
neg_A1	
;	decf		ACCaHI, F
;	BSF			carry, 0x00
neg_A2
;;	comf		ACCaHI, F
;	retlw		0

;****************************************************************************
;****************************************************************************


;****************************************************************************
;****************************************************************************
;*******************************************************************
;                       Double Precision Multiplication
;
;               ( Optimized for Speed : straight Line Code )
;
;*******************************************************************;
;   Multiplication : ACCb(16 bits) * ACCa(16 bits) -> ACCb,ACCc ( 32 bits )
;      (a) Load the 1st operand in location ACCaLO & ACCaHI ( 16 bits )
;      (b) Load the 2nd operand in location ACCbLO & ACCbHI ( 16 bits )
;      (c) CALL D_mpyF
;      (d) The 32 bit result is in location ( ACCbHI,ACCbLO,ACCcHI,ACCcLO )
;
;   Performance :
;               Program Memory  :       240
;               Clock Cycles    :       233
;
;       Note : The above timing is the worst case timing, when the
;               register ACCb = FFFF. The speed may be improved if
;               the register ACCb contains a number ( out of the two
;               numbers ) with less number of 1s.
;
;               The performance specs are for Unsigned arithmetic ( i.e,
;               with "SIGNED equ  FALSE ").
;
;       Program:          DBL_MPYF.ASM 
;       Revision Date:   
;                         1-13-97      Compatibility with MPASMWIN 1.40
;
;*******************************************************************;
;
;*******************************************************************
SIGNED  equ     0x00            ; Set This To 'TRUE' if the routines
;                               ; for Multiplication & Division needs
;                               ; to be assembled as Signed Integer
;                               ; Routines. If 'FALSE' the above two
;                               ; routines ( D_mpy & D_div ) use
;                               ; unsigned arithmetic.
;*******************************************************************
;       multiplication macro
;
mulMac  MACRO
	LOCAL   NO_ADD
;
	rrf     ACCdHI, F       ;rotate d right
	rrf     ACCdLO, F
	btfss   STATUS,C    ;need to add?
	goto    NO_ADD          ; no addition necessary
	movf    ACCaLO,W        ; Addition ( ACCb + ACCa -> ACCb )
	addwf   ACCbLO, F       ;add lsb
	btfsc   STATUS,C    ;add in carry
	incf    ACCbHI, F
	movf    ACCaHI,W
	addwf   ACCbHI, F       ;add msb
NO_ADD  rrf     ACCbHI, F
	rrf     ACCbLO, F
	rrf     ACCcHI, F
	rrf     ACCcLO, F
;
	ENDM
;
;*******************************************************************;
;               Double Precision Multiply ( 16x16 -> 32 )
;         ( ACCb*ACCa -> ACCb,ACCc ) : 32 bit output with high word
;  in ACCb ( ACCbHI,ACCbLO ) and low word in ACCc ( ACCcHI,ACCcLO ).
;
D_mpyF                          ;results in ACCb(16 msb's) and ACCc(16 lsb's)
;
     IF   SIGNED
     CALL    S_SIGN
     ENDIF
;
	call    setup
;
; use the mulMac macro 16 times
;
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
	mulMac
;
    IF    SIGNED
	btfss   sign,MSB
	retlw   0
	comf    ACCcLO          ; negate ACCa ( -ACCa -> ACCa )
	incf    ACCcLO
	btfsc   STATUS,Z
	decf    ACCcHI
	comf    ACCcHI
	btfsc   STATUS,Z
neg_B   comf    ACCbLO          ; negate ACCb
	incf    ACCbLO
	btfsc   STATUS,Z
	decf    ACCbHI
	comf    ACCbHI
	retlw   0
    ELSE
	retlw   0
    ENDIF
;
;*******************************************************************
;
setup   movlw   .16             ; for 16 shifts
	movwf   temp
	movf    ACCbHI,W          ;move ACCb to ACCd
	movwf   ACCdHI
	movf    ACCbLO,W
	movwf   ACCdLO
	clrf    ACCbHI
	clrf    ACCbLO
	retlw   0
;
;*******************************************************************
;

;
;*******************************************************************
;  Assemble this section only if Signed Arithmetic Needed
;
     IF    SIGNED
;
S_SIGN  movf    ACCaHI,W
	xorwf   ACCbHI,W
	movwf   sign
	btfss   ACCbHI,MSB        ; if MSB set go & negate ACCb
	goto    chek_A
;
	comf    ACCbLO          ; negate ACCb
	incf    ACCbLO
	btfsc   STATUS,Z
	decf    ACCbHI
	comf    ACCbHI
;
chek_A  btfss   ACCaHI,MSB        ; if MSB set go & negate ACCa
	retlw   0
	goto    neg_A
;
     ENDIF
;
;****************************************************************************
;****************************************************************************
;*******************************************************************
;**********************************************************************************************
;**********************************************************************************************
        
;       32/16 Bit Unsigned Fixed Point Divide 32/16 -> 32.16

;       Input:  32 bit unsigned fixed point dividend in AARGB0, AARGB1,AARGB2,AARGB3
;               16 bit unsigned fixed point divisor in BARGB0, BARGB1

;       Use:    CALL    FXD3216U

;       Output: 32 bit unsigned fixed point quotient in AARGB0, AARGB1,AARGB2,AARGB3
;               16 bit unsigned fixed point remainder in REMB0, REMB1

;       Result: AARG, REM  <--  AARG / BARG

;       Max Timing:     2+699+2 = 703 clks

;       Max Timing:     2+663+2 = 667 clks

;       PM: 2+240+1 = 243               DM: 9

FXD3216U        CLRF            REMB0
                CLRF            REMB1

                CALL 			UDIV3115L

                RETLW           0x00

;**********************************************************************************************
;**********************************************************************************************
UDIV3115L       

;       Max Timing:     9+6*17+16+16+6*17+16+16+6*17+16+16+6*17+16+8 = 537 clks

;       Min Timing:     9+6*16+15+15+6*16+15+15+6*16+15+15+6*16+15+3 = 501 clks

;       PM: 157                                 DM: 9

                MOVF            BARGB1,W
                SUBWF           REMB1, F
                MOVF            BARGB0,W
                BTFSS           _C
                INCFSZ          BARGB0,W
                SUBWF           REMB0, F
                RLF             AARGB0, F

                MOVLW           7
                MOVWF           LOOPCOUNT

LOOPU3115A      RLF             AARGB0,W
                RLF             REMB1, F
                RLF             REMB0, F
                MOVF            BARGB1,W
                BTFSS           AARGB0,LSB
                GOTO            UADD15LA

                SUBWF           REMB1, F
                MOVF            BARGB0,W
                BTFSS           _C
                INCFSZ          BARGB0,W
                SUBWF           REMB0, F
                GOTO            UOK15LA

UADD15LA        ADDWF           REMB1, F
                MOVF            BARGB0,W
                BTFSC           _C
                INCFSZ          BARGB0,W
                ADDWF           REMB0, F
        
UOK15LA RLF             AARGB0, F

                DECFSZ          LOOPCOUNT, F
                GOTO            LOOPU3115A

                RLF             AARGB1,W
                RLF             REMB1, F
                RLF             REMB0, F
                MOVF            BARGB1,W
                BTFSS           AARGB0,LSB
                GOTO            UADD15L8

                SUBWF           REMB1, F
                MOVF            BARGB0,W
                BTFSS           _C
                INCFSZ          BARGB0,W
                SUBWF           REMB0, F
                GOTO            UOK15L8

UADD15L8        ADDWF           REMB1, F
                MOVF            BARGB0,W
                BTFSC           _C
                INCFSZ          BARGB0,W
                ADDWF           REMB0, F
        
UOK15L8 RLF             AARGB1, F

                MOVLW           7
                MOVWF           LOOPCOUNT

LOOPU3115B      RLF             AARGB1,W
                RLF             REMB1, F
                RLF             REMB0, F
                MOVF            BARGB1,W
                BTFSS           AARGB1,LSB
                GOTO            UADD15LB

                SUBWF           REMB1, F
                MOVF            BARGB0,W
                BTFSS           _C
                INCFSZ          BARGB0,W
                SUBWF           REMB0, F
                GOTO            UOK15LB

UADD15LB        ADDWF           REMB1, F
                MOVF            BARGB0,W
                BTFSC           _C
                INCFSZ          BARGB0,W
                ADDWF           REMB0, F
        
UOK15LB RLF             AARGB1, F

                DECFSZ          LOOPCOUNT, F
                GOTO            LOOPU3115B

                RLF             AARGB2,W
                RLF             REMB1, F
                RLF             REMB0, F
                MOVF            BARGB1,W
                BTFSS           AARGB1,LSB
                GOTO            UADD15L16

                SUBWF           REMB1, F
                MOVF            BARGB0,W
                BTFSS           _C
                INCFSZ          BARGB0,W
                SUBWF           REMB0, F
                GOTO            UOK15L16

UADD15L16       ADDWF           REMB1, F
                MOVF            BARGB0,W
                BTFSC           _C
                INCFSZ          BARGB0,W
                ADDWF           REMB0, F
        
UOK15L16        RLF             AARGB2, F

                MOVLW           7
                MOVWF           LOOPCOUNT

LOOPU3115C      RLF             AARGB2,W
                RLF             REMB1, F
                RLF             REMB0, F
                MOVF            BARGB1,W
                BTFSS           AARGB2,LSB
                GOTO            UADD15LC

                SUBWF           REMB1, F
                MOVF            BARGB0,W
                BTFSS           _C
                INCFSZ          BARGB0,W
                SUBWF           REMB0, F
                GOTO            UOK15LC

UADD15LC        ADDWF           REMB1, F
                MOVF            BARGB0,W
                BTFSC           _C
                INCFSZ          BARGB0,W
                ADDWF           REMB0, F
        
UOK15LC RLF             AARGB2, F

                DECFSZ          LOOPCOUNT, F
                GOTO            LOOPU3115C

                RLF             AARGB3,W
                RLF             REMB1, F
                RLF             REMB0, F
                MOVF            BARGB1,W
                BTFSS           AARGB2,LSB
                GOTO            UADD15L24

                SUBWF           REMB1, F
                MOVF            BARGB0,W
                BTFSS           _C
                INCFSZ          BARGB0,W
                SUBWF           REMB0, F
                GOTO            UOK15L24

UADD15L24       ADDWF           REMB1, F
                MOVF            BARGB0,W
                BTFSC           _C
                INCFSZ          BARGB0,W
                ADDWF           REMB0, F
        
UOK15L24        RLF             AARGB3, F

                MOVLW           7
                MOVWF           LOOPCOUNT

LOOPU3115D RLF          AARGB3,W
                RLF             REMB1, F
                RLF             REMB0, F
                MOVF            BARGB1,W
                BTFSS           AARGB3,LSB
                GOTO            UADD15LD

                SUBWF           REMB1, F
                MOVF            BARGB0,W
                BTFSS           _C
                INCFSZ          BARGB0,W
                SUBWF           REMB0, F
                GOTO            UOK15LD

UADD15LD        ADDWF           REMB1, F
                MOVF            BARGB0,W
                BTFSC           _C
                INCFSZ          BARGB0,W
                ADDWF           REMB0, F
        
UOK15LD RLF             AARGB3, F

                DECFSZ          LOOPCOUNT, F
                GOTO            LOOPU3115D

                BTFSC           AARGB3,LSB
                GOTO            UOK15L
                MOVF            BARGB1,W
	        ADDWF           REMB1, F
                MOVF            BARGB0,W
                BTFSC           _C
                INCFSZ          BARGB0,W
                ADDWF           REMB0, F
UOK15L

                RETURN


;********************************************************************


;********************************************************************
;                  Binary To BCD Conversion Routine
;      This routine converts a 16 Bit binary Number to a 5 Digit
; BCD Number. This routine is useful since PIC16C55 & PIC16C57
; have  two 8 bit ports and one 4 bit port ( total of 5 BCD digits)
;
;       The 16 bit binary number is input in locations H_byte and
; L_byte with the high byte in H_byte.
;       The 5 digit BCD number is returned in R0, R1 and R2 with R0
; containing the MSD in its right most nibble.
;
;   Performance :
;               Program Memory  :       35
;               Clock Cycles    :       885
;
;
;       Program:          B16TOBCD.ASM 
;       Revision Date:   
;                         1-13-97      Compatibility with MPASMWIN 1.40
;
;*******************************************************************;

B2_BCD  bcf     STATUS,0                ; clear the carry bit
	movlw   .16
	movwf   count
	clrf    R0
	clrf    R1
	clrf    R2
loop16  rlf     L_byte, F
	rlf     H_byte, F
	rlf     R2, F
	rlf     R1, F
	rlf     R0, F
;
	decfsz  count, F
	goto    adjDEC

	RETLW   0
;
adjDEC  movlw   R2
	movwf   FSR
	call    adjBCD
;
	movlw   R1
	movwf   FSR
	call    adjBCD
;
	movlw   R0
	movwf   FSR
	call    adjBCD
;
	goto    loop16
;
adjBCD  movlw   3
	addwf   0,W
	movwf   temp
	btfsc   temp,3          ; test if result > 7
	movwf   0
	movlw   30
	addwf   0,W
	movwf   temp
	btfsc   temp,7          ; test if result > 7
	movwf   0               ; save as MSD

	RETLW   0
;
;************************************************************************
;               BCD To Binary Conversion
;
;       This routine converts a 5 digit BCD number to a 16 bit binary
; number.
;       The input 5 digit BCD numbers are asumed to be in locations
; R0, R1 & R2 with R0 containing the MSD in its right most nibble.
;
;       The 16 bit binary number is output in registers H_byte & L_byte
; ( high byte & low byte repectively ).
;
;       The method used for conversion is :
;               input number X = abcde ( the 5 digit BCD number )
;               X = abcde = 10[10[10[10a+b]+c]+d]+e
;
;   Performance :
;               Program Memory  :       30
;               Clock Cycles    :       121
;
;
;       Program:          BCD2BIN.ASM 
;       Revision Date:   
;                         1-13-97      Compatibility with MPASMWIN 1.40
;
;*******************************************************************;
;
mpy10b
	andlw		0F
	addwf		L_byte, F
	btfsc		STATUS,C
	incf		H_byte, F
mpy10a
	bcf			STATUS,C        ; multiply by 2
	rlf			L_byte,W
	movwf		L_temp
	rlf			H_byte,W        ; (H_temp,L_temp) = 2*N
	movwf		H_temp
;
	bcf			STATUS,C        ; multiply by 2
	rlf			L_byte, F
	rlf			H_byte, F
	bcf			STATUS,C        ; multiply by 2
	rlf			L_byte, F
	rlf			H_byte, F
	bcf			STATUS,C        ; multiply by 2
	rlf			L_byte, F
	rlf			H_byte, F       ; (H_byte,L_byte) = 8*N
;
	movf		L_temp,W
	addwf		L_byte, F
	btfsc		STATUS,C
	incf		H_byte, F
	movf		H_temp,W
	addwf		H_byte, F
	retlw		0               ; (H_byte,L_byte) = 10*N
;
;
BCDtoB
	clrf		H_byte
	movf		R0,W
	andlw		0F
	movwf		L_byte
	call		mpy10a          ; result = 10a+b
;
	swapf		R1,W
	call		mpy10b          ; result = 10[10a+b]
;
	movf		R1,W
	call		mpy10b          ; result = 10[10[10a+b]+c]
;
	swapf		R2,W
	call		mpy10b          ; result = 10[10[10[10a+b]+c]+d]
;
	movf		R2,W
	andlw		0F
	addwf		L_byte, F
	btfsc		STATUS,C
	incf		H_byte, F       ; result = 10[10[10[10a+b]+c]+d]+e
	retlw		0               ; BCD to binary conversion done
;
;*******************************************************************