#define F_CPU 3333333
#define TCB_CMP_EXAMPLE_VALUE   (0xffff)
#define TCAdelay 1
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#include "include/usart.c"
#include "include/led_pins.c"
#include "include/system_init.c"
#include "include/button_interaction.c"

const uint8_t btn_pin = PIN2_bm;


const struct State {
	const uint8_t Out; // 6-bit output
	const struct State *Next[4]; // next states
};

typedef const struct State State_t;

#define goN &FSM[0]
#define waitN &FSM[1]
#define goE &FSM[2]
#define waitE &FSM[3]

State_t FSM[4] = {
	{21,{goN,waitN,goN,waitN}},
	{22,{goE,goE,goE,goE}},
	{11,{goE,goE,waitE,waitE}},
	{12,{goN,goN,goN,goN}}
};

int main(void) {
	USART0_init();
	
	
	State_t *pt; // pointer to current state
	uint32_t input; // car sensor input
	pt = goN; // initial state
	printf("Starting: \n");
	printf(" %d ", pt->Out);
	while(1){
		// 1) set lights to current state's Out
		printf("%d",pt->Out);
		// 2) specified wait for this state
		_delay_ms(1000);
		// 3) input from car detectors
		input = 1;
		// 4) next depends on state and input
		pt = pt->Next[input];
	}
	return 0;
}