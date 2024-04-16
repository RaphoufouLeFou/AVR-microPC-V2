
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
	ldi		XL,		0x61				;
	clr		XH							;
	st     	X,		r25					;
    ldi     r25,   	0xFC				;
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
	ld		tmp, 	X
	sbrs    tmp,   4	    			
	rjmp    AVR_Output_Pixel_Loop	
		
	ldi		XL,		0x62					
	st     	X,		r24					
    out     _SFR_IO_ADDR(PORTA),  r25	
	out     _SFR_IO_ADDR(PORTC),  r22   
    ori     r20,                  0x01  
    out     _SFR_IO_ADDR(PORTD),  r20   
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
	in      res,    _SFR_IO_ADDR(PINB)	;read PINC
	ret

