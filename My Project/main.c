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
		0b1111101,
		0b1111011,
		0b1101111,
		0b1011111
	};
	int i ;
	while(1){

		PORTB.OUT = 0b00000001; // all off
		
		
		for (i=0; i<4; i++)
		{
			
			PORTA.OUT =port_a_outs[i]; // Toggle state of pin 4
			_delay_ms(120);
		}



	}
}