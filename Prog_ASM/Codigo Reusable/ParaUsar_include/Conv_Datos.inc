;


B2_BCD
	bcf			STATUS,C   ;Borra el carry  
 	movlw		.16 
  	movwf		contador1   
	clrf	    R0  
 	clrf		R1  
 	clrf		R2
loop16 
 	rlf			Aux,f  
 	rlf			Aux+1,f   
	rlf			R2,f 
  	rlf			R1,f 
  	rlf			R0,f 
  	decfsz		contador1,f  
 	goto		adjDEC  
 	return

adjDEC
	movlw		R2 
  	movwf		FSR
  	call		adjBCD  
 	movlw		R1  
 	movwf		FSR
  	call		adjBCD  
 	movlw		R0  
 	movwf		FSR 
  	call		adjBCD 
  	goto		loop16
adjBCD 
	movlw		3  
 	addwf		INDF,W  
 	movwf		temp  
 	btfsc		temp,3          ; test if result > 7 
  	movwf		INDF 
 	movlw		30  
 	addwf		INDF,W  
 	movwf		temp  
 	btfsc		temp,7          ; test if result > 7  
 	movwf		INDF            ; save as MSD  
 	return