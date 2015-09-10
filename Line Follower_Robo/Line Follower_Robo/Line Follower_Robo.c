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

volatile unsigned char cnt=0, Prev_Dirn=0,duty=90;


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

unsigned char key;
unsigned char volatile F_NewKey=0;

void main(void)
{
	unsigned char old_value=0, new_value=0;

	DDRC = 0xff; //making port c output
	DDRD = 0xff;// inputs
	DDRB = 0X00;// sensor inputs.


	UART_Init(9600);
	PORTB = 0X07;

	TCNT1H=0xEF;   // Load the 16-bit count value
	TCNT1L=0x00;   // for 1 sec at 7.3728MHz

	TCCR1A=0x00;
	TCCR1B=0x01;  // Timer mode with 1024 prescler
	TIMSK1=0x01;   // Enable timer1 overflow interrupt(TOIE1)
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG


	while(1)
	{

		new_value = PINB & 0X07;
		PORTD = new_value<<4;

		if(new_value!=old_value)
		{
			Robot = Stop;
			switch(new_value)

			{
				case 0: Prev_Dirn= Robot = Stop;  break;
				case 1: Prev_Dirn= Robot = Right;    break;
				case 2: Prev_Dirn= Robot = Forward;  break;
				case 3: Prev_Dirn= Robot = Right;    break;
				case 4: Prev_Dirn= Robot = Left;     break;
				case 5: Prev_Dirn= Robot = Forward;  break;
				case 6: Prev_Dirn= Robot = Left;     break;
				case 7: Prev_Dirn= Robot = Forward;   break;
				default: Prev_Dirn= Robot = Stop;  break;
			}

			old_value = new_value;
			
			cnt = 0;

			// 	  UART_TxChar(new_value + 0x30);
		}

	}
}
