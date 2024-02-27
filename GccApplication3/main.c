#define F_CPU 3333333


#include <avr/io.h>

#include <avr/sleep.h>

#include <avr/interrupt.h>

#include <stdint.h>

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
	{0b00000000,0b00000000}, // ALL OFF
	
};



const uint8_t led_pin = PIN2_bm; // PORT C

const uint8_t btn_pin = PIN6_bm; // PORT C
volatile int x ;
volatile int i ;

void RTC_init(int RTCdelay)
{
	RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;    // 32.768kHz Internal Crystal Oscillator (INT32K)

	while (RTC.STATUS > 0);               // Wait for all register to be synchronized
	RTC.PER = RTCdelay;                   // Set period for delay
	RTC.INTCTRL |= RTC_OVF_bm;            // Enable overflow Interrupt which will trigger ISR
	RTC.CTRLA = RTC_PRESCALER_DIV32_gc    // 32768 / 32 = 1024 (sec) ~ 1 ms
	| RTC_RTCEN_bm                        // Enable: enabled
	| RTC_RUNSTDBY_bm;                    // Run In Standby: enabled
}

int main() {


	// LED setup
	RTC_init(100);
	//
	// 	PORTA.DIRSET=0b00000100;
	// 	PORTB.DIRSET=0b00000001;
	
	PORTA.DIRSET = 0b10111111;
	PORTB.DIRSET = 0b11111111;

	// Button setup
	PORTA.PIN6CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm; // Enable pull-up resistor

	i=1;
	x=200;

	PORTA.OUT = 0b00000000;
	PORTB.OUT = 0b00000001;
	
	sei(); // Set global interrupts
	while(1){
		

		// Toggle state of pin 4
		
		
	}


}
ISR(PORTA_PORT_vect) {

	i++;
	_delay_ms(100);
	while (~PORTA.IN& btn_pin){

	}
	PORTA.INTFLAGS |= btn_pin; // Clear interrupt flag

}

ISR(RTC_CNT_vect)
{
	RTC.INTFLAGS = RTC_OVF_bm;            // Clear flag by writing '1':
	PORTA.OUT = port_a_b_outs[i][0];
	PORTB.OUTTGL= port_a_b_outs[i][1];
	
}