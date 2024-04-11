#define F_CPU 3333333
#define STAY_ON_TIME 5 // s
#define RUN_TROUGH_SPEED 1 // ms
#define TCB_CMP_EXAMPLE_VALUE   (0xffff)
#define TCAdelay 1
#define RTCdelay
#include <avr/io.h>

#include <avr/sleep.h>

#include <avr/interrupt.h>

#include <stdint.h>
#include <stdio.h>

#include <util/delay.h>


const uint8_t port_a_b_outs[17][2]={
	// CSDPONMU,0bxxxxxTBA
	{0b00011110,0b00000000}, // ALL OFF
	
	{0b00011100,0b00000001}, // LED 1
	{0b00011010,0b00000001}, // LED 2
	{0b00010110,0b00000001}, // LED 3
	{0b00001110,0b00000001}, // LED 4
	
	{0b00011100,0b00000010}, // LED 5
	{0b00011010,0b00000010}, // LED 6
	{0b00010110,0b00000010}, // LED 7
	{0b00001110,0b00000010}, // LED 8
	
	{0b10011100,0b00000000}, // LED 9
	{0b10011010,0b00000000}, // LED 10
	{0b10010110,0b00000000}, // LED 11
	{0b10001110,0b00000000}, // LED 12
	
	{0b00111100,0b00000000}, // LED 13
	{0b00111010,0b00000000}, // LED 14
	{0b00110110,0b00000000}, // LED 15
	{0b00101110,0b00000000}, // LED 16
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
volatile uint8_t pit_interrupt;
int i ;
int j ;
int c;
volatile uint16_t consecutive_counts_pressed ;
volatile uint16_t consecutive_counts_released ;
volatile uint16_t pit_interrupts_since_last_increase;


void RTCA_init(){
	RTC.CLKSEL = RTC_CLKSEL_INT1K_gc;				// 1024 Hz from OSCULP32K
	RTC.CTRLA = RTC_RTCEN_bm;					// enable RTC
	RTC.PITINTCTRL = RTC_PI_bm;					// enable periodic interrupt
	RTC.PITCTRLA = RTC_PERIOD_CYC32768_gc | RTC_PITEN_bm;		// set period; enable PIT
	//RTC.PITCTRLA = RTC_PERIOD_CYC1024_gc | RTC_PITEN_bm;		// set period; enable PIT
	
}

void TCA0_init()
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
	TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm ;
	
	/* Enable Capture or Timeout interrupt */
	TCB0.INTCTRL = TCB_CAPT_bm;
}


void LEDOnById(int i){
	PORTA.OUT = ~port_a_b_outs[i][0];
	PORTB.OUT= ~port_a_b_outs[i][1];
}

void allLEDoff(){
	PORTA.OUT = ~port_a_b_outs[0][0];
	PORTB.OUT= ~port_a_b_outs[0][1];
}

void wait_until_button_released()
{
	while(consecutive_counts_released<6){printf("Waiting for release with consecutive_counts_pressed:%d, consecutive_counts_released:%d\n",consecutive_counts_pressed, consecutive_counts_released );}
	consecutive_counts_pressed=0;
	consecutive_counts_released=0;
	printf("Released\n");
}

int main() {


	RTCA_init(10); // set periodic RTC triggering "awakening" delay in seconds
	TCA0_init();
	TCB0_init();
	//USART0_init();

	PORTA.DIRSET = 0b10111111;
	PORTB.DIRSET = 0b00001111;

	// Button setup
	PORTA.PIN6CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm; // Enable pull-up resistor

	
	// Initialize variables
	consecutive_counts_pressed=0;
	consecutive_counts_released=0;
	pit_interrupts_since_last_increase=0; // TODO read this from EEPROM
	x=10;
	i=0;
	pit_interrupt=0;
	
	sei();
	SLPCTRL.CTRLA |= SLPCTRL_SMODE_STDBY_gc; // set POWER DOWN as sleep mode
	SLPCTRL.CTRLA |= SLPCTRL_SEN_bm; // enable sleep mode
	
	printf("booting finished\n");
	wait_until_button_released();

	while(1){
		if (consecutive_counts_pressed> 5){
			
			if (pit_interrupts_since_last_increase< 0){
				printf("Increase not possible because not enough time has passed since the last time.\n");
				allLEDoff();
				_delay_ms(250);
			}
			else{
				printf("Increasing x to %d \n", x);
				
				x++;
				
				
				pit_interrupts_since_last_increase =0 ;
				
			}

			wait_until_button_released();
			if (x>16){
				
				for ( c=16; c>=1; c--){x=c;_delay_ms(100);				}
				for ( c=1; c<=16; c++){x=c;_delay_ms(100);				}
				for ( c=16; c>=1; c--){x=c;_delay_ms(100);				}
				
			x=1;} // i overflow carrying
		}
		
		if (consecutive_counts_released>200){
			printf("going to sleep...\n");
			allLEDoff();
			sleep_cpu();
			
			
			if (pit_interrupt==0){
				consecutive_counts_pressed=0;
				consecutive_counts_released=0;
				_delay_ms(10);
				wait_until_button_released();
			}


		}
		
		if (pit_interrupt==1){
			allLEDoff();
			_delay_ms(10);
			
			pit_interrupt=0;
			pit_interrupts_since_last_increase++;
			if (pit_interrupts_since_last_increase > 360){
				printf("Reseting x due to long inactivity\n");
				x= 1;
			}
			printf("again just PIT with %d \n", pit_interrupts_since_last_increase);

			allLEDoff();
			
			if (pit_interrupts_since_last_increase%5){
				printf("every nth PIT should be visible\n ");
				
			}
			else{
				sleep_cpu();
			}
			
		}
		

		
		// TODO AWAKEING Animation!
		
		
	}


}
ISR(PORTA_PORT_vect) {
	pit_interrupt = 0 ;
	PORTA.INTFLAGS |= btn_pin; // Clear interrupt flag
}

ISR(RTC_PIT_vect)
{
	pit_interrupt = 1;
	RTC.PITINTFLAGS = RTC_PI_bm;// Clear interrupt flag
}

ISR(TCA0_OVF_vect)
{
	// "Running" trough the LED table defined above and switch LEDs on & off according to the x value
	i++;

	if (i<=x){LEDOnById(i);}
	else{allLEDoff();}

	if (i>16){i=0;} // i overflow carrying

	// The interrupt flag has to be cleared manually
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
	
}


ISR(TCB0_INT_vect)
{
	// Counting consecutive ticks of pressed or released button. Use case: Debouncing and short and long press detection.
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