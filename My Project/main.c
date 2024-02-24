#define F_CPU 3333333
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	PORTB.DIR = 0b1111111;    // Set pin 4 as output
	PORTA.DIR = 0b1111111;    // Set pin 4 as output
	PORTA.OUT = 0b1111111;
	
	PORTA.DIRCLR = PIN6_bm;
	PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
	// PA6
	
	int port_a_outs[4]={
		0b11111101,
		0b11111011,
		0b11101110,
		0b11011110,
	};
	
	int port_b_outs[4]={
		0b00000001,
		0b00000010,
		0b00000100,
		0b00001000,
	};
	
	int i, j;
	while(1){


		for (i=0; i<=3; i++)
		{
			PORTB.OUT = port_b_outs[i];
			
			for (j=0; j<=3; j++)
			{
				
				PORTA.OUT =port_a_outs[j]; // Toggle state of pin 4
				_delay_ms(150);
			}

		}
		
		
		
		


	}
}