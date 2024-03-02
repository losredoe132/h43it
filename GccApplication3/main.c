#define F_CPU 3333333
#define STAY_ON_TIME 5 // s
#define RUN_TROUGH_SPEED 1 // ms
#define TCB_CMP_EXAMPLE_VALUE   (0xffff)

#include <avr/io.h>

#include <avr/sleep.h>

#include <avr/interrupt.h>

#include <stdint.h>
#include <stdio.h>

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

#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)
static void USART0_sendChar(char c)
{
	while (!(USART0.STATUS & USART_DREIF_bm))
	{
		;                                   /* Wait for USART ready for receiving next char */
	}
	USART0.TXDATAL = c;
}

static int USART0_printChar(char c, FILE *stream)
{
	USART0_sendChar(c);
	return 0;
}

static FILE USART_stream = FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);

void USART0_init(void)
{

	PORTB.DIR &= ~PIN3_bm;                  /* Configure RX pin as an input */
	PORTB.DIR |= PIN2_bm;                   /* Configure TX pin as an output */

	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600);

	USART0.CTRLB |= USART_TXEN_bm;          /* Transmitter Enable bit mask. */

	stdout = &USART_stream;                 /* Bind UART to stdio output stream */
}


const uint8_t btn_pin = PIN6_bm;

volatile uint8_t manully_triggered = 0;
volatile int x ;
int i ;
int j ;
volatile uint16_t consecutive_counts_pressed ;
volatile uint16_t consecutive_counts_released ;



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

void TCB0_init (void)
{
	/* Load the Compare or Capture register with the timeout value*/
	TCB0.CCMP = TCB_CMP_EXAMPLE_VALUE;
	
	/* Enable TCB and set CLK_PER divider to 1 (No Prescaling) */
	TCB0.CTRLA = TCB_CLKSEL_CLKDIV2_gc | TCB_ENABLE_bm ;
	
	/* Enable Capture or Timeout interrupt */
	TCB0.INTCTRL = TCB_CAPT_bm;
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


	//RTCA_init(1); // set periodic RTC triggering "awakening" delay in seconds
	TCA0_init(1);
	TCB0_init();
	PORTA.DIRSET = 0b10111111;
	PORTB.DIRSET = 0b11111111;
	//USART0_init();

	// Button setup
	PORTA.PIN6CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm; // Enable pull-up resistor
	
	// Initalize variables
	consecutive_counts_pressed=0;
	consecutive_counts_released=0;
	x=7;
	i=0;
	
	sei();
	SLPCTRL.CTRLA |= SLPCTRL_SMODE_STDBY_gc; // set POWER DOWN as sleep mode
	SLPCTRL.CTRLA |= SLPCTRL_SEN_bm; // enable sleep mode
	
	while(consecutive_counts_released<6){printf("Waiting for release with consecutive_counts_pressed:%d, consecutive_counts_released:%d\n",consecutive_counts_pressed, consecutive_counts_released );}
	printf("Released");

	while(1){
		
		if (consecutive_counts_pressed> 10){
			x++;
			printf("Increasing x to %d", x);
				consecutive_counts_pressed=0;
				consecutive_counts_released=0;
			while(consecutive_counts_released<6){printf("Waiting for release with consecutive_counts_pressed:%d, consecutive_counts_released:%d\n",consecutive_counts_pressed, consecutive_counts_released );}
			printf("Released");
		}
		
		if (consecutive_counts_released>200){
			printf("going to sleep...");
			
			sleep_cpu();
			printf("waking up...");
			consecutive_counts_pressed=0;
			consecutive_counts_released=0;
			sei();
			SLPCTRL.CTRLA |= SLPCTRL_SMODE_PDOWN_gc; // set POWER DOWN as sleep mode
			SLPCTRL.CTRLA |= SLPCTRL_SEN_bm; // enable sleep mode
			// wait until user releases button
			
			while(consecutive_counts_released<6){printf("Waiting for release with consecutive_counts_pressed:%d, consecutive_counts_released:%d\n",consecutive_counts_pressed, consecutive_counts_released );}
			printf("Released");
			

		}
		
		// TODO AWAKEING Animation!
		
		
	}


}
ISR(PORTA_PORT_vect) {
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


ISR(TCB0_INT_vect)
{
	// Counting consecutive ticks of pressed or released button. Debouncing and short and long press detection.
	if (~PORTA.IN & btn_pin){
		consecutive_counts_pressed++;
		consecutive_counts_released=0;
	}
	else{
		consecutive_counts_pressed=0;
		consecutive_counts_released++;
	}
	TCB0.INTFLAGS = TCB_CAPT_bm; /* Clear the interrupt flag */
}