#define F_CPU 3333333
#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>



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



const uint8_t led_pin = PIN2_bm; // PORT C

const uint8_t btn_pin = PIN6_bm; // PORT C
volatile int x ;
int i ;

int main() {

	// LED setup

	//
	// 	PORTA.DIRSET=0b00000100;
	// 	PORTB.DIRSET=0b00000001;
	
	PORTA.DIRSET = 0b10111111;
	PORTB.DIRSET = 0b11111111;

	// Button setup
	PORTA.PIN6CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm; // Enable pull-up resistor

	i=0;
	x=2;

	sei(); // Set global interrupts
	PORTA.OUT = 0b00000000;
	PORTB.OUT = 0b00000001;
	
	

	while(1){

		for (i=0; i<=x; i++)
		{
			if(~PORTA.IN & PIN6_bm){
				
			}
			else{
				PORTA.OUT = port_a_b_outs[i][0];
				PORTB.OUT = port_a_b_outs[i][1];
				// Toggle state of pin 4
				
			}
			
			
			_delay_ms(1);
		}
	}


}
ISR(PORTA_PORT_vect) {

	
	if (i <=15){
		x++;
	}
	else{
		x=1;
	}
	_delay_ms(100);
	while (~PORTA.IN& btn_pin){
		
	}
	PORTA.INTFLAGS |= btn_pin; // Clear interrupt flag

}