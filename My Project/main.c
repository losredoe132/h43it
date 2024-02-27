

#define F_CPU 3333333
#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>

volatile uint8_t x;

const uint8_t led_pin = PIN2_bm; // PORT C
const uint8_t btn_pin = PIN6_bm; // PORT C


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

int main() {

	// LED setup


	PORTA.DIRSET=0b00000100;
	PORTB.DIRSET=0b00000001;

	// Button setup

	PORTA.DIRCLR = btn_pin; // Set button as input

	PORTA.PIN6CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm; // Enable pull-up resistor

	

	sei(); // Set global interrupts
	PORTA.OUT = 0b00000000;
	PORTB.OUT = 0b00000001;
	
	

	while(1){


	}


}
ISR(PORTA_PORT_vect) {

	PORTA.OUTTGL = led_pin; // toggle led
	_delay_ms(100);
	while (~PORTA.IN& btn_pin){
		
	}
	PORTA.INTFLAGS |= btn_pin; // Clear interrupt flag

}