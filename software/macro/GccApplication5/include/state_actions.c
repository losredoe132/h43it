
void IdleAction() {
    printf("Entering state Idle with n_counts_awake=%d and steps_since_last_activation=%d\n", n_counts_awake, steps_since_last_activation);
	n_counts_awake ++;	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;    /* source (sys_clk/8) +  start timer */

}

void ActivationAction() {
    printf("Entering state Activation \n");
	n_counts_awake=0;
	array_day_activation[day_counter]=1; 
	
}

void GoingToBedAction() {
    printf("Entering state GoingToBed \n");
	_delay_ms(20); // waiting until message is sent over Serial. TODO can be deleted in productive code without serial com
	TCA0.SINGLE.CTRLA =  ~TCA_SINGLE_ENABLE_bm;    /* source (sys_clk/8) +  start timer */
	allLEDoff();
	
	sleep_mode(); // actually going to sleep	
}

void MissedActivationAction() {
	day_counter++;
	if (day_counter > 31){
		day_counter = 1; 
	}
    printf("Entering state MissedActivation increasing day_counter to %d\n", day_counter);
	steps_since_last_activation = 0 ; 
}

void ResetAction() {
    printf("Entering state Reset \n");
	n_counts_awake=0;
	for(int idx = 0; idx < 32; idx++) {
		array_day_activation[idx] = 0; // Initialize each element to 0
	}
}

void JustWokeUpAction() {
	TCA0.SINGLE.CTRLA =  ~TCA_SINGLE_ENABLE_bm;    /* source (sys_clk/8) +  start timer */
    printf("Entering state JustWokeUp \n");
	n_counts_awake=0;
}


void PITAction() {
    printf("Entering state PIT with steps_since_last_activation=%d\n", steps_since_last_activation);
}








