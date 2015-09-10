#include<avr\io.h>
#include<avr\interrupt.h>
#include <util\delay.h>
#include "uart.h"

// sensor PB0 LEFT, PB1 MIDDLE , PB2 RIGHT

#define Robot PORTC

#define Forward  0x06
#define Backward 0x09
#define Left 0x0a
#define Right 0x05
#define Stop 0x00



unsigned char key;
//unsigned char volatile F_NewKey=0;



void main(void)
{
	Robot = 0xff; 
	UART_Init(9600);

	UART_TxString("Computer controlled Robot!\n\r");
	DDRC = 0xff; //making port c output
	//DDRD = 0x00;// inputs

	PORTC = 0xff;
	_delay_ms(100);

	
	
	



	while(1)
	{
		key = UART_RxChar();
		UART_TxChar(key);
		

		switch(key)

		{
			
			case 'w' :Robot = Forward; UART_TxChar(Robot+0x30);_delay_ms(100); Robot = Stop;break;
			case 's' :Robot = Backward;_delay_ms(100); Robot = Stop;break;
			case 'a' :Robot = Left;_delay_ms(100);UART_TxChar(Robot+0x30); Robot = Stop;break ;
			case 'd' :Robot = Right;_delay_ms(100); Robot = Stop;break;
			
			default:Robot = Stop; break;


		}

	}
}
