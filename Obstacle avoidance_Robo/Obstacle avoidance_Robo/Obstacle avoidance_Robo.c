#include<avr\io.h>
#include<avr\interrupt.h>
#include <util\delay.h>
#include "uart.h"

// sensor PB0 LEFT, PB1 MIDDLE , PB2 RIGHT

#define Forward  0x06
#define Backward 0x09
#define Left 0x0a
#define Right 0x05
#define Stop 0x00

#define mobile PIND

#define Robot PORTC
#define Sensor PORTB

unsigned char key;
unsigned char volatile F_NewKey=0;


volatile unsigned char cnt=0, Prev_Dirn=0,duty=40,new_value,old_value;

ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	
	cnt++;          // Increment the cnt value each time the isr is executed

	if(cnt > duty)
	{
		Robot = Stop;
	}
	else if(cnt < duty)
	{
		Robot = Prev_Dirn;
	}
	else if(cnt==100)
	{
		Robot = Prev_Dirn;
		cnt =0;
	}
	
	TCNT1H=0xff;    // Reload the 16-bit count value
	TCNT1L=0x00;    // in timer1 count registers
}



void main(void)
{


	DDRC = 0xff; //making port c output
	DDRD = 0x00;// inputs
	DDRB = 0X00;// sensor inputs.

	UART_Init(9600);

	PORTB = 0X07;


	while(1)
	{
		UART_TxChar((PINB & 0X07)+0x30);

		switch(PINB & 0X07)

		{
			case 0: Prev_Dirn=Robot=Forward; break;
			case 1: Prev_Dirn=Robot = Left;_delay_ms(100); break;
			case 2: Prev_Dirn=Robot = Backward;_delay_ms(500);Robot = Left;_delay_ms(100); break;
			case 3: Prev_Dirn=Robot = Left;_delay_ms(100); break;
			case 4: Prev_Dirn=Robot = Right; _delay_ms(100); break;
			case 5: Prev_Dirn=Robot = Backward;_delay_ms(500);Robot = Left;_delay_ms(100); break;
			case 6: Prev_Dirn=Robot = Right; _delay_ms(100); break;
			case 7: Prev_Dirn=Robot = Backward;_delay_ms(500);Robot = Left;_delay_ms(100); break;
			default: Robot = Stop; break;
		}

	}
}
