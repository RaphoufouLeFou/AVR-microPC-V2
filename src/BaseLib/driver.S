
#include <avr/io.h>


;***** AVR_Init *****
;   input:
;
;   Using :
;   r25

.global AVR_Init
AVR_Init:

    ldi     r25,   	0xFF				;set tmp to 0xFF
    out     _SFR_IO_ADDR(DDRA),   r25	;set the output pin to output
	out     _SFR_IO_ADDR(DDRC),   r25	;
	out     _SFR_IO_ADDR(DDRD),   r25	;

	clr		XH							;
	ldi		XL,		0x64				;
	ldi		r25, 	0b00010000
	st		X, 	r25						;
	ldi		XL,		0x61				;
    ldi     r25,   	0xFC				;
	st     	X,		r25					;
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
	#define   tmp r23
	ldi		XL,		0x63				;
	clr		XH
AVR_Output_Pixel_Loop:
	;ld		tmp, 	X
	;sbrs    tmp,   4	    			
	;rjmp    AVR_Output_Pixel_Loop	
		
	ldi		XL,		0x62					
	st     	X,		r24					
    out     _SFR_IO_ADDR(PORTA),  r25	
	out     _SFR_IO_ADDR(PORTC),  r22   
    out     _SFR_IO_ADDR(PORTD),  r20   
	sbi     _SFR_IO_ADDR(PORTD),  7  	;
    cbi     _SFR_IO_ADDR(PORTD),  7     
    clr     r20
	st     	X,		r20					;Clear the ports
	out     _SFR_IO_ADDR(PORTA),  r20   ;
	out     _SFR_IO_ADDR(PORTD),  r20   ;
	out     _SFR_IO_ADDR(PORTC),  r20   ;
    ret


;***** AVR_Output_All *****
;   input:
;   r25, DataH
;   r24, DataL
;
;   Using :
;   r23

.global AVR_Output_All
AVR_Output_All:

	ldi		XL,		0x62				;
	clr		XH							;
	st     	X,		r24					;
	out     _SFR_IO_ADDR(PORTA),  r25	;output the second 8bit data to PORTA

    #define   AdX r25
    #define   AdY r24
	#define   tmp r23

    clr    AdX
    clr    AdY
	ldi		XL,		0x63				;

outputAllLoop:
	ld		tmp, 	X
	sbrs    tmp,   4	    			
	rjmp    AVR_Output_Pixel_Loop	

    out     _SFR_IO_ADDR(PORTD),  r20   ;
    out     _SFR_IO_ADDR(PORTC),  r22   ;
	sbi     _SFR_IO_ADDR(PORTD),  7  	;
    cbi     _SFR_IO_ADDR(PORTD),  7     ;

	cpi     AdX,    0xFF        		;compare the X address to 0x80
	breq    outputAllEndLoop       		;if it's not 0x80, jump to the outputAllLoop loop

	inc     AdX                 		;increase the X address
	rjmp	outputAllLoop       		;jump to the outputAllLoop loop

outputAllEndLoop:
    clr     AdX                 		;reset the X address to 0
    ;clr     tmp
	;out     _SFR_IO_ADDR(PORTB), tmp	;Clear the ports
    cpi     AdY, 0x80           		;compare the Y address to 0x80
	breq    outputAllEnd        		;if it's 0x80, jump to the outputAllEnd point
    inc     AdY				    		;increase the Y address
	rjmp    outputAllLoop       		;jump to the outputAllLoop loop
outputAllEnd: 
    clr     r20
	st     	X,		r20					;Clear the ports
	out     _SFR_IO_ADDR(PORTA),  r20   ;
	out     _SFR_IO_ADDR(PORTD),  r20   ;
	out     _SFR_IO_ADDR(PORTC),  r20   ;
    ret


;***** AVR_Read_Inputs *****
;   input:
;
;   Using :
;   r24

.global AVR_Read_Inputs
AVR_Read_Inputs:
	#define    res    r24
	ldi   r25,    0x00				;set tmp to 0xFF
	out     _SFR_IO_ADDR(DDRB),   r25	;
	in      res,    _SFR_IO_ADDR(PINB)	;read PINC
	ret


;***** AVR_Write_Outputs *****
;   input:
;	r24, Data

;   Using :
;   r25

.global AVR_Write_Outputs
AVR_Write_Outputs:
	#define    res    r24
	ldi r25,    			0xFF	;
	out _SFR_IO_ADDR(DDRB), r25		;
	out _SFR_IO_ADDR(PORTB),res		;write PINC
	ret


;***** AVR_Init_UART *****
;   input:
;	r25, BaudrateH
;	r24, BaudrateL
;   Using :
;   r25

.global AVR_Init_UART
AVR_Init_UART:
	sts	UBRR0L,r24			; load baud prescale
	sts	UBRR0H,r25			; to UBRR0

	ldi	r24,(1<<RXEN0)|(1<<TXEN0)	; enable transmitter
	sts	UCSR0B,r24			; and receiver
	ldi	r24,(1<<UCSZ00)|(1<<UCSZ01)	; 8-bit data
	sts	UCSR0C,r24			; no parity, 1 stop bit

	ret					; return from subroutine


;***** AVR_Send_UART *****
;   input:
;	r24, Data
;   Using :
;   r24

.global AVR_Send_UART
AVR_Send_UART:
	lds		r25,UCSR0A			; load UCSR0A into r24
	sbrs	r25,UDRE0			; wait for empty transmit buffer
	rjmp	AVR_Send_UART				; repeat loop
	sts	UDR0,r24			; transmit character

	ret					; return from subroutine


;***** AVR_Receive_UART *****
;   input:
;   Using :
;   r24

.global AVR_Receive_UART
AVR_Receive_UART:	
	lds		r24,UCSR0A			; load UCSR0A into r17
	sbrs	r24,UDRE0		; wait for empty transmit buffer
	rjmp	AVR_Receive_UART			; repeat loop

	lds	r24,UDR0			; get received character

	ret						; return from subroutine


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
	ldi	iLoopRl,iValL	; intialize inner loop count in inner
	ldi	iLoopRh,iValH	; loop high and low registers
iLoop2:	sbiw	iLoopRl,1		; decrement inner loop registers
	brne	iLoop2			; branch to iLoop if iLoop registers != 0
	nop				; no operation
	ret				; return from subroutine


	;***** AVR_Delay *****
;   input:
;   Using :
;   r23

#define loopCt r24

.global AVR_Delay
AVR_Delay:
	ldi loopCt, 0x7f
	caca:
	ldi	iLoopRl,iValL	; intialize inner loop count in inner
	ldi	iLoopRh,iValH	; loop high and low registers

iLoop:	sbiw	iLoopRl,1		; decrement inner loop registers
	brne	iLoop			; branch to iLoop if iLoop registers != 0
	dec	loopCt			; decrement outer loop register
	brne	caca		; branch to oLoop if outer loop register != 0
	nop				; no operation
	ret				; return from subroutine