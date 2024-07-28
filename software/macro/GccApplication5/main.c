#define F_CPU 3333333
#define TCB_CMP_EXAMPLE_VALUE   (0xffff)
#define TCAdelay 1
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>
const uint8_t btn_pin = PIN2_bm;

uint8_t array_day_activation[32];
uint8_t n_counts_awake ; 

uint8_t i ;



#include "include/usart.c"
#include "include/led_pins.c"
#include "include/system_init.c"


// Forward declaration of state struct for function pointer types
struct State;

typedef struct {
    struct State* nextState;
    bool (*condition)();  // Function pointer to a condition function
} Transition;

typedef struct State {
    const char* name;
    void (*onEnter)();  // Function pointer to onEnter function
    Transition* transitions;
    size_t numTransitions;
} State;

typedef struct State State_t;


#define CONSECUTIVE_COUNTS_PRESSED_SHORT 30
#define CONSECUTIVE_COUNTS_PRESSED_LONG 100

#include "include/transition_conditions.c"
#include "include/state_actions.c"
#include "include/transition_table.c"


void transitionState(State** currentState) {
    for (size_t i = 0; i < (*currentState)->numTransitions; ++i) {
        if ((*currentState)->transitions[i].condition()) {
            *currentState = (*currentState)->transitions[i].nextState;
            (*currentState)->onEnter();
            return;
        }
    }
}



int main(void) {

	PORTA.DIRSET = 0b11100011;
	PORTB.DIRSET = 0b11111111;
	PORTC.DIRSET = 0b11111111;
	USART0_init();
	TCA0_init();
	TCB0_init();

	// Button setup
	PORTA.PIN2CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm; // Enable pull-up resistor

	sei();
	
	i =0;	
	n_counts_awake=0; 
	for(int idx = 0; idx <= 32; idx++) {
		array_day_activation[idx] = 1; // Initialize each element to 0
	}
	
	
	printf("Booting finished\n"); 
	
	// Set initial state 
	State* currentState = &Idle;

    // Initial state action
    if (currentState->onEnter) {
        currentState->onEnter();
    }

    // Example FSM execution loop
    while(true) {
		_delay_ms(100);
        transitionState(&currentState);
    }

	return 0;
}