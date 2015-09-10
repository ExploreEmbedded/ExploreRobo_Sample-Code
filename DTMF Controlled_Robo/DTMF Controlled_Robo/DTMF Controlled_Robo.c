#include<avr\io.h>
#include<avr\interrupt.h>
#include <util\delay.h>
#include "uart.h"

// sensor PB0 LEFT, PB1 MIDDLE , PB2 RIGHT

#define Forward  0x06
#define Backward 0x09
#define Left 0x0a
#define Right 0x05
#define Stop 0xff

#define mobile PIND

#define Robot PORTC

unsigned char volatile key;
unsigned char volatile F_NewKey=0;

ISR(INT1_vect)
{
	
	F_NewKey = 1;

}

void main(void)
{

	//UART_Init();

	//aUART_TxString("AVR Robo TEST\n\r");
	DDRC = 0xff; //making port c output
	DDRD = 0x00;// inputs

	PORTC = 0xff;
	_delay_ms(100);

	EICRA |= (1 << ISC00) | (1 << ISC01);    // The rising edge of INTx generates an interrupt request
	EIMSK |= (1 << INT1);     // Turns on INT1

	MCUCR = 0x0c;
	SREG = 0X80;
	sei(); //global interrupt enable
	

	//key = UART_RxChar();

	while(1)
	{

		if(F_NewKey ==1)
		{
			key = (mobile&0xf0);
			F_NewKey =0;
		}


		switch(key)

		{
			
			case 0x20 :Robot = Forward;break;
			case 0x80 :Robot = Backward;break;
			case 0x40 :Robot = Left;_delay_ms(5); Robot = 0x00; key=0;break ;
			case 0x60 :Robot = Right;_delay_ms(5); Robot = 0x00;key=0;break;
			
			default:Robot = Stop; key=0;; break;


		}

	}
}
