#define F_CPU 3333333
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main()
{
	PORTA.DIR = 0b10111111;
	PORTB.DIR = 0b11111111;
	
	PORTA.DIRCLR = PIN6_bm;
	PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
	// PA6
	
	int port_a_b_outs[16][2]={
		{0b11111101,0b00000100}, // LED 1
		{0b11111011,0b00000100}, // LED 3
		{0b11101110,0b00000100}, // LED 4
		{0b11011110,0b00000100}, // LED 5
		
		{0b11011110,0b00001000}, // LED 5
		{0b11101110,0b00001000}, // LED 6
		{0b11111011,0b00001000}, // LED 7
		{0b11111101,0b00001000}, // LED 8
		
		{0b11111101,0b00000001}, // LED 9
		{0b11111011,0b00000001}, // LED 10
		{0b11101110,0b00000001}, // LED 11
		{0b11011110,0b00000001}, // LED 12
		
		{0b11011110,0b00000010}, // LED 13
		{0b11101110,0b00000010}, // LED 14
		{0b11111011,0b00000010}, // LED 15
		{0b11111101,0b00000010}, // LED 16
		
	};
	
	int i;
	
	while(1){

		for (i=0; i<=15; i++)
		{
			if(~PORTA.IN & PIN6_bm){
				
			}
			else{
						PORTA.OUT = port_a_b_outs[i][0];
						PORTB.OUT = port_a_b_outs[i][1];
						// Toggle state of pin 4
				
			}
			
	
			_delay_ms(250);
		}
	}
}