
#include <avr/io.h>

#define IO(x) _SFR_IO_ADDR(x)

;***** AVR_Init *****
;   input:
;
;   Using :
;   r25

.global AVR_Init
AVR_Init:

    ldi     r25,   		0xFF				;set tmp to 0xFF
    out     IO(DDRA),   r25					;set the output pin to output
	out     IO(DDRC),   r25					;
	out     IO(DDRD),   r25					;

	clr		XH								;
	ldi		XL,			0x64				;
	ldi		r25, 		0b00000000
	st		X, 			r25					;
	ldi		XL,			0x61				;
    ldi     r25,   		0xFF				;
	st     	X,			r25					;
	ret


;***** AVR_Plot_Pixel *****
;   input:
;   r25, DataH
;   r24, DataL
;   r22, AddressX
;   r20, AddressY
;  	Using :
;   r23
;	Output :
;   r24, Result

.global AVR_Plot_Pixel
AVR_Plot_Pixel:                      		;output the pixel to the screen and returns 1 if failure
	#define tmp r23
	ldi		XL,			0x63				
	clr		XH
	ld		tmp, 		X
	sbrs    tmp,   		4	    			
	rjmp    AVR_Plot_Pixel_End
				
	ldi 	tmp,		0xFF
	ldi		XL,			0x61				
	st     	X,			tmp					

	ldi		XL,			0x62					
	st     	X,			r24			
	out     IO(DDRA), 	tmp
    out     IO(PORTA),  r25	
	out     IO(PORTC),  r22   
    out     IO(PORTD),  r20   
	sbi     IO(PORTD),  7  		
    cbi     IO(PORTD),  7     
    clr     r20
	st     	X,			r20		
	out     IO(PORTA),  r20   		
	out		IO(DDRA),   r20   		
	ldi		XL,			0x61				
	st     	X,			r20		
	out     IO(PORTD),  r20   		
	out     IO(PORTC),  r20   		
	ldi 	r24, 		0x00
	ldi		XL,			0x63
	ld		tmp, 		X
	sbrs    tmp,   		4
	ldi 	r24, 		0x01
    ret
	#undef   tmp
	
AVR_Plot_Pixel_End:
	ldi 	r24, 		0x01
	ret


;***** AVR_Output_Pixel *****
;   input:
;   r25, DataH
;   r24, DataL
;   r22, AddressX
;   r20, AddressY
;  	Using :
;   r23

.global AVR_Output_Pixel
AVR_Output_Pixel:                      	;output the pixel to the screen
	#define tmp r23
	ldi		XL,			0x63				
	clr		XH
AVR_Output_Pixel_Loop:
	ld		tmp, 		X
	sbrs    tmp,   		4	    			
	rjmp    AVR_Output_Pixel_Loop	
				
	ldi 	tmp,		0xFF
	ldi		XL,			0x61				
	st     	X,			tmp					

	ldi		XL,			0x62					
	st     	X,			r24			
	out     IO(DDRA),  	tmp
    out     IO(PORTA),  r25	
	out     IO(PORTC),  r22   
    out     IO(PORTD),  r20   
	sbi     IO(PORTD),  7  	
    cbi     IO(PORTD),  7     
    clr     r20
	st     	X,			r20					
	out     IO(PORTA),  r20   
	out		IO(DDRA),   r20   
	ldi		XL,			0x61				
	st     	X,			r20		
	out     IO(PORTD),  r20   
	out     IO(PORTC),  r20   
    ret
	#undef   tmp

;***** AVR_Output_All *****
;   input:
;   r25, DataH
;   r24, DataL
;
;   Using :
;   r23

.global AVR_Output_All
AVR_Output_All:

	ldi		XL,			0x62			;
	clr		XH							;
	st     	X,			r24				;
	out     IO(PORTA),  r25				;output the second 8bit data to PORTA

    #define   AdX r25
    #define   AdY r24
	#define   tmp r23

    clr    AdX
    clr    AdY
	ldi		XL,			0x63		

outputAllLoop:
	ld		tmp, 		X
	sbrs    tmp,   		4	    			
	rjmp    AVR_Output_Pixel_Loop	

    out     IO(PORTD),  r20   
    out     IO(PORTC),  r22   
	sbi     IO(PORTD),  7  	
    cbi     IO(PORTD),  7     

	cpi     AdX,   		0xFF        	;compare the X address to 0x80
	breq    outputAllEndLoop       		;if it's not 0x80, jump to the outputAllLoop loop

	inc     AdX                 		;increase the X address
	rjmp	outputAllLoop       		;jump to the outputAllLoop loop

outputAllEndLoop:
    clr     AdX                 		;reset the X address to 0
    ;clr     tmp
	;out     IO(PORTB), tmp				;Clear the ports
    cpi     AdY, 		0x80           	;compare the Y address to 0x80
	breq    outputAllEnd        		;if it's 0x80, jump to the outputAllEnd point
    inc     AdY				    		;increase the Y address
	rjmp    outputAllLoop       		;jump to the outputAllLoop loop
outputAllEnd: 

    clr     r20
	st     	X,			r20				;Clear the ports
	out     IO(PORTA),  r20   ;
	out     IO(PORTD),  r20   ;
	out     IO(PORTC),  r20   ;
	out		IO(DDRA),   r20   ;
	ldi		XL,			0x61				
	st     	X,			r20		

    ret


;***** AVR_Read_Inputs *****
;   input:
;
;   Using :
;   r24

.global AVR_Read_Inputs
AVR_Read_Inputs:
	#define res r24
	ldi  	r25,   		0x00			;set tmp to 0xFF
	out		IO(DDRB),   r25				;
	in      res,    	IO(PINB)		;read PINC
	ret


;***** AVR_Write_Outputs *****
;   input:
;	r24, Data

;   Using :
;   r25

.global AVR_Write_Outputs
AVR_Write_Outputs:
	#define res r24
	ldi 	r25,		0xFF			;
	out 	IO(DDRB), 	r25				;
	out 	IO(PORTB),	res				;write PINC
	ret


;***** AVR_Init_UART *****
;   input:
;	r25, BaudrateH
;	r24, BaudrateL
;   Using :
;   r25

.global AVR_Init_UART
AVR_Init_UART:
	sts		UBRR0L,		r24				; load baud prescale
	sts		UBRR0H,		r25				; to UBRR0

	ldi		r24,		(1<<RXEN0)|(1<<TXEN0)	; enable transmitter
	sts		UCSR0B,		r24				; and receiver
	ldi		r24,		(1<<UCSZ00)|(1<<UCSZ01)	; 8-bit data
	sts		UCSR0C,		r24				; no parity, 1 stop bit
	ret									; return from subroutine


;***** AVR_Send_UART *****
;   input:
;	r24, Data
;   Using :
;   r25

.global AVR_Send_UART
AVR_Send_UART:
	lds		r25,		UCSR0A			; load UCSR0A into r25
	sbrs	r25,		UDRE0			; wait for empty transmit buffer
	rjmp	AVR_Send_UART				; repeat loop
	sts		UDR0,		r24				; transmit character
	ret									; return from subroutine


;***** AVR_Receive_UART *****
;   input:
;   Using :
;   r24

.global AVR_Receive_UART
AVR_Receive_UART:	
	lds		r24,		UCSR0A			; load UCSR0A into r24
	sbrs	r24,		UDRE0			; wait for empty transmit buffer
	rjmp	AVR_Receive_UART			; repeat loop
	lds		r24,		UDR0			; get received character
	ret									; return from subroutine

;***** AVR_Receive_UART_String *****
;   input:
;   r25, String address H
;   r24, String address L
;   Using :
;   r23

.global AVR_Receive_UART_String
AVR_Receive_UART_String:
	mov 	XH, 		r25
	mov 	XL, 		r24
AVR_Receive_UART_String_loop:
	lds		r25,		UCSR0A			; load UCSR0A into r17
	sbrs	r25,		UDRE0			; wait for empty transmit buffer
	rjmp	AVR_Receive_UART_String_loop; repeat loop
	lds 	r24, 		UDR0			; get received character
	cpi		r24,		0x0A			; check if rcv'd char is NL
	breq	AVR_Receive_UART_String_end	; branch if NL rcv'd
	st		X+,			r24				; store character to buffer
	call	AVR_Send_UART				; echo character
	rjmp	AVR_Receive_UART_String_loop; get another character


AVR_Receive_UART_String_end:
	ldi		r24,		0x0A			; load NL into r24
	call	AVR_Send_UART				; echo NL
	rjmp 	USART_Flush
	ret									; return from subroutine

USART_Flush:
	lds		r24,		UCSR0A			; load UCSR0A into r24
	sbrs	r24,		UDRE0			; wait for empty transmit buffer
	ret
	in 		r24, 		UDR0
	rjmp 	USART_Flush

;***** AVR_Delay_10ms *****
;   input:
;   Using :
;   r23

#define iValL 62
#define iValH 156
#define iLoopRl r26
#define iLoopRh r27

.global AVR_Delay_10ms
AVR_Delay_10ms:
	ldi		iLoopRl,	iValL			; intialize inner loop count in inner
	ldi		iLoopRh,	iValH			; loop high and low registers
iLoop2:	
	sbiw	iLoopRl,	1				; decrement inner loop registers
	brne	iLoop2						; branch to iLoop if iLoop registers != 0
	nop									; no operation
	ret									; return from subroutine


	;***** AVR_Delay *****
;   input:
;   Using :
;   r23

#define loopCt r24

.global AVR_Delay
AVR_Delay:
	ldi 	loopCt, 	0x7f
caca:
	ldi		iLoopRl,	iValL			; intialize inner loop count in inner
	ldi		iLoopRh,	iValH			; loop high and low registers

iLoop:	
	sbiw	iLoopRl,	1				; decrement inner loop registers
	brne	iLoop						; branch to iLoop if iLoop registers != 0
	dec		loopCt						; decrement outer loop register
	brne	caca						; branch to oLoop if outer loop register != 0
	nop									; no operation
	ret									; return from subroutine

;***** AVR_Read_Rom *****
;   input:
;   r25, AddressH
;   r24, AddressL
;   Output :
;   r24, Data

.global AVR_Read_Rom
AVR_Read_Rom:
	cli									; Disable interrupt before writing to the EEPROM
	sbic 	IO(EECR),	EEWE			; Wait for completion of previous write
	rjmp 	AVR_Read_Rom

	out 	IO(EEARH), 	r25				; Set up address (r24:r25) in address register
	out 	IO(EEARL), 	r24
	sbi 	IO(EECR),	EERE			; Start eeprom read by writing EERE
	
	in 		r24,		IO(EEDR)		; Read data from data register
	sei									; Re-enable the interrupts
	ret


;***** AVR_Write_Rom *****
;   input:
;   r25, AddressH
;   r24, AddressL
;   r22, DataL
.global AVR_Write_Rom
AVR_Write_Rom:
	cli									; Disable interrupt before writing to the EEPROM
	sbic 	IO(EECR),	EEWE			; Wait for completion of previous write
	rjmp 	AVR_Write_Rom
	out 	IO(EEARH), 	r25				; Set up address (r24:r25) in address register
	out 	IO(EEARL), 	r24
	out 	IO(EEDR),	r22				; Write data (r22) to data register
	sbi 	IO(EECR),	EEMWE			; Write logical one to EEMWE
	sbi 	IO(EECR),	EEWE 			; Start eeprom write by setting EEWE
	sei									; Re-enable the interrupts
	ret

.global AVR_DEBUG
AVR_DEBUG:
	ldi 	r25,		0xFF
	out 	IO(DDRE), 	r25
	out 	IO(PORTE),	res
	clr		XH
	ldi		XL,			0x64
	ldi		r25, 		0b00001111
	st		X, 			r25
	ldi		XL,			0x65
	ldi     r25,   		0xFF
	st     	X,			r25
	ret
