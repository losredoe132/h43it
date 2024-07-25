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

#include "include/usart.c"
#include "include/led_pins.c"
#include "include/system_init.c"
#include "include/button_interaction.c"


#include <util/delay.h>




const uint8_t btn_pin = PIN2_bm;

volatile uint8_t manully_triggered = 0;
volatile int x ;
volatile uint8_t pit_interrupt;
int i ;
int j ;
int c;




int main() {


	RTCA_init(10); // set periodic RTC triggering "awakening" delay in seconds
	TCA0_init();
	TCB0_init();
	USART0_init();

	PORTA.DIRSET = 0b11100011;
	PORTB.DIRSET = 0b11111111;
	PORTC.DIRSET = 0b11111111;

	// Button setup
	PORTA.PIN2CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm; // Enable pull-up resistor

	
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
			if (x>32){
				
				for ( c=32; c>=1; c--){x=c;_delay_ms(100);				}
				for ( c=1; c<=32; c++){x=c;_delay_ms(100);				}
				for ( c=32; c>=1; c--){x=c;_delay_ms(100);				}
				
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

	if (i>32){i=0;} // i overflow carrying

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