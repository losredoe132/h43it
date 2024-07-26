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


#define CONSECUTIVE_COUNTS_PRESSED_SHORT 30
#define CONSECUTIVE_COUNTS_PRESSED_LONG 100
// Condition functions
bool consecutive_counts_pressed_greater_0() {
	if (consecutive_counts_pressed >0){
		return true;}
	else{
		return false; }
}

bool consecutive_counts_released_greater_0() {
	if (consecutive_counts_released>0){
	return true;}
	else{
	return false; }
}

bool consecutive_counts_pressed_short() {
   if ((consecutive_counts_pressed >100)){
		return true;}
	else{
		return false; }
}

bool consecutive_counts_pressed_long() {
	if ((consecutive_counts_pressed >CONSECUTIVE_COUNTS_PRESSED_LONG)){
	return true;}
	else{
	return false; }
}


// Example state action functions
void stateAEnter() {
	LEDOnById(1);
    printf("Entering stateA \n");
}

void stateBEnter() {
	LEDOnById(2);
	printf("Entering stateB \n");
}
void stateCEnter() {
	LEDOnById(3);
	printf("Entering stateC \n");
}

void stateDEnter() {
	LEDOnById(5);
	printf("Entering stateD \n");
}


State stateA, stateB, stateC, stateD;

// Define transitions for each state
Transition stateATransitions[] = {
    { &stateB, consecutive_counts_pressed_greater_0 }
};

Transition stateBTransitions[] = {
    { &stateA, consecutive_counts_released_greater_0 }, 
    { &stateC, consecutive_counts_pressed_short }
		
};

Transition stateCTransitions[] = {
	{ &stateA, consecutive_counts_released_greater_0 },
	{ &stateD, consecutive_counts_pressed_long }
	
};
Transition stateDTransitions[] = {
	{ &stateA, consecutive_counts_released_greater_0 }
	
};

// Define states
State stateA = { "State A", stateAEnter, stateATransitions, 1 };
State stateB = { "State B", stateBEnter, stateBTransitions, 2};
State stateC = { "State B", stateCEnter, stateCTransitions, 1 };
State stateD = { "State D", stateDEnter, stateDTransitions, 1 };



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
        transitionState(&currentState);
    }

	return 0;
}