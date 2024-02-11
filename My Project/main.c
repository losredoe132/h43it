#include <atmel_start.h>
#include <avr/io.h>
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// OUTs
	PORTA.DIR = PIN4_bm|PIN5_bm;

	// INs
	PORTB.DIRCLR  = PIN2_bm;
	PORTB.PIN2CTRL |= PORT_PULLUPEN_bm;


	/* Replace with your application code */
	while (1) {
		// Read Button State
		bool status = PORTB.IN & PIN2_bm;

		PORTA.OUTCLR = PIN5_bm;

		PORTA.OUTCLR = PIN4_bm;

		if (!status) {
			PORTA.OUTSET = PIN5_bm;

		}
		else {
			PORTA.OUTSET = PIN4_bm;
		}
	}
}
