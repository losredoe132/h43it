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
	
	while(1){
		if(~PORTA.IN & PIN6_bm) /* check if PB2 is pulled to GND */
		{
			PORTB.OUTCLR = PIN1_bm; // Toggle state of pin 4
		}
		else{
			PORTB.OUTSET = PIN1_bm; // all off
		}
		_delay_ms(1);
	}
}