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


// Example condition functions
bool conditionAlwaysTrue() {
	if (consecutive_counts_pressed >2){
		return true;}
	else{
		return false; }
}

bool conditionAlwaysFalse() {
   if (consecutive_counts_released >100){
		return true;}
	else{
		return false; }
}

// Example state action functions
void stateAEnter() {
	LEDOnById(1);
    printf("Entering State A\n");
}

void stateBEnter() {
	LEDOnById(2);
	printf("Entering State B\n");
}

State stateA, stateB;

// Define transitions for each state
Transition stateATransitions[] = {
    { &stateB, conditionAlwaysTrue }
};

Transition stateBTransitions[] = {
    { &stateA, conditionAlwaysFalse }
};

// Define states
State stateA = { "State A", stateAEnter, stateATransitions, 1 };
State stateB = { "State B", stateBEnter, stateBTransitions, 1 };



void transitionState(State** currentState) {
    for (size_t i = 0; i < (*currentState)->numTransitions; ++i) {
        if ((*currentState)->transitions[i].condition()) {
            *currentState = (*currentState)->transitions[i].nextState;
            if ((*currentState)->onEnter) {
                (*currentState)->onEnter();
            }
            return;
        }
    }
}


int main(void) {

	PORTA.DIRSET = 0b11100011;
	PORTB.DIRSET = 0b11111111;
	PORTC.DIRSET = 0b11111111;
	USART0_init();
	TCB0_init();
	TCA0_init();
	

	// Button setup
	PORTA.PIN2CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm; // Enable pull-up resistor

	sei();
	

	
	printf("Booting finished\n"); 
	State* currentState = &stateA;

    // Initial state action
    if (currentState->onEnter) {
        currentState->onEnter();
    }

    // Example FSM execution loop
    while(true) {
		_delay_ms(1);
        printf("Current State: %s\n", currentState->name);
			printf("consecutive_counts_pressed: %d \n", consecutive_counts_pressed);
			printf("consecutive_counts_released: %d \n", consecutive_counts_released);
        transitionState(&currentState);
    }

	return 0;
}