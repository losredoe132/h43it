#define F_CPU 3333333
#define STAY_ON_TIME 5 // s
#define RUN_TROUGH_SPEED 1 // ms

#include <avr/io.h>

#include <avr/sleep.h>

#include <avr/interrupt.h>

#include <stdint.h>

#include <util/delay.h>


const uint8_t port_a_b_outs[17][2]={
	{0b11111111,0b00000000}, // ALL OFF
	
	{0b11111101,0b00000100}, // LED 1
	{0b11111011,0b00000100}, // LED 3
	{0b11101111,0b00000100}, // LED 4
	{0b11011111,0b00000100}, // LED 5
	
	{0b11011111,0b00001000}, // LED 5
	{0b11101111,0b00001000}, // LED 6
	{0b11111011,0b00001000}, // LED 7
	{0b11111101,0b00001000}, // LED 8
	
	{0b11111101,0b00000001}, // LED 9
	{0b11111011,0b00000001}, // LED 10
	{0b11101111,0b00000001}, // LED 11
	{0b11011111,0b00000001}, // LED 12
	
	{0b11011111,0b00000010}, // LED 13
	{0b11101111,0b00000010}, // LED 14
	{0b11111011,0b00000010}, // LED 15
	{0b11111101,0b00000010}, // LED 16
};

const uint8_t btn_pin = PIN6_bm;

volatile int x ;
int i ;
int j ;

void RTCA_init(int RTCdelay)
{
	RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;    // 32.768kHz Internal Crystal Oscillator (INT32K)

	while (RTC.STATUS > 0);               // Wait for all register to be synchronized
	RTC.PER = RTCdelay;                   // Set period for delay
	RTC.INTCTRL |= RTC_OVF_bm;            // Enable overflow Interrupt which will trigger ISR
	RTC.CTRLA = RTC_PRESCALER_DIV32768_gc    // 32768 / 32 = 1024 (sec) ~ 1 ms
	| RTC_RTCEN_bm                        // Enable: enabled
	| RTC_RUNSTDBY_bm;                    // Run In Standby: enabled
}



void LEDOnById(int i){
	PORTA.OUT = port_a_b_outs[i][0];
	PORTB.OUT= port_a_b_outs[i][1];
}

void allLEDoff(){
	PORTA.OUT = port_a_b_outs[0][0];
	PORTB.OUT= port_a_b_outs[0][1];
}

int main() {


	RTCA_init(10); // set periodic RTC triggering "awakening" delay in seconds

	
	PORTA.DIRSET = 0b10111111;
	PORTB.DIRSET = 0b11111111;

	// Button setup
	PORTA.PIN6CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm; // Enable pull-up resistor

	x=8;

	sei(); // Set global interrupts
	
	

	
	while(1){
		
		// TODO AWAKEING Animation!

		
		for (int c=0; c<=STAY_ON_TIME*1000/RUN_TROUGH_SPEED/16; c++){
			for (i=1; i<=16; i++){
				if (i<=x){
					
					LEDOnById(i);
				}
				else{
					allLEDoff();
				}
				_delay_ms(RUN_TROUGH_SPEED);
			}
		}

		allLEDoff();
		set_sleep_mode(SLEEP_MODE_IDLE);
		cli();
		sleep_enable();
		PORTA_INTFLAGS |= PORT_INT7_bm;
		// Toggle state of pin 4
		sei();
		sleep_cpu();
		
	}


}
ISR(PORTA_PORT_vect) {

	_delay_ms(200);
	// if button is still pressed
	if (~PORTA.IN& btn_pin){
		x++;
		if (x >17){
			x=16;
		}
	}
	// wait until user released button
	while (~PORTA.IN& btn_pin)
	{
	}
	PORTA.INTFLAGS |= btn_pin; // Clear interrupt flag
}

ISR(RTC_CNT_vect)
{
	RTC.INTFLAGS = RTC_OVF_bm;            // Clear flag by writing '1':
}