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

volatile uint8_t manully_triggered = 0;
volatile int x ;
int i ;
int j ;

void RTCA_init(int RTCdelay)
{
	while (RTC.STATUS > 0) { /* Wait for all register to be synchronized */
	}

	RTC.CTRLA = RTC_PRESCALER_DIV32768_gc /* 32768 */
	| 1 << RTC_RTCEN_bp       /* Enable: enabled */
	| 0 << RTC_RUNSTDBY_bp;   /* Run In Standby: disabled */
	
}
void RTC_PIT_init(){
	RTC.CLKSEL = RTC_CLKSEL_INT1K_gc;
	while (RTC.PITSTATUS > 0) { /* Wait for all register to be synchronized */
	}
	RTC.PITINTCTRL = RTC_PI_bm;
	RTC.PITCTRLA = RTC_PERIOD_CYC8192_gc | RTC_PITEN_bm;
}


void TCA0_init(int TCAdelay)
{
	// enable overflow interrupt
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;

	// set Normal mode
	TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;

	// disable event counting
	TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);

	// set the period
	TCA0.SINGLE.PER = TCAdelay;

	// set clock
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;    /* source (sys_clk/8) +  start timer */
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


	//RTCA_init(10); // set periodic RTC triggering "awakening" delay in seconds
	TCA0_init(1);
	RTC_PIT_init();
	
	PORTA.DIRSET = 0b10111111;
	PORTB.DIRSET = 0b11111111;

	// Button setup
	PORTA.PIN6CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm; // Enable pull-up resistor

	x=1;
	i=0;

	sei(); // Set global interrupts
	

	
	while(1){
		// TODO AWAKEING Animation!
		
	if (manully_triggered>0){
	RTC_PIT_init();
	}
		
		_delay_ms(1000);
		
		if  (~PORTA.IN & btn_pin)
		{
			x++;
		}
		_delay_ms(1000);
		
		// wait until button is released
		allLEDoff();
		
		SLPCTRL.CTRLA |= SLPCTRL_SMODE_PDOWN_gc; // set POWER DOWN as sleep mode
		SLPCTRL.CTRLA |= SLPCTRL_SEN_bm; // enable sleep mode
		
		cli();
		manully_triggered  = 0 ; 
		PORTA_INTFLAGS |= PORT_INT7_bm;
		// Toggle state of pin 4
		sei();
		sleep_cpu();
	}


}
ISR(PORTA_PORT_vect) {
	manully_triggered = 1;
	PORTA.INTFLAGS |= btn_pin; // Clear interrupt flag
}

ISR(RTC_PIT_vect)
{
	RTC.PITINTFLAGS = RTC_PI_bm;// Clear interrupt flag
}

ISR(TCA0_OVF_vect)
{
	i++;
	
	if (i<=x){LEDOnById(i);}
	else{allLEDoff();}
	
	if (i>16){i=0;}

	// The interrupt flag has to be cleared manually
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
