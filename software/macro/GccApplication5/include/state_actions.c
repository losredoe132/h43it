
void IdleAction() {
	n_counts_awake ++;	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;    /* source (sys_clk/8) +  start timer */

    printf("Entering state Idle with n_counts_awake= %d \n", n_counts_awake);
}

void ActivationAction() {
	n_counts_awake=0;

	
    printf("Entering state Activation \n");
}

void GoingToBedAction() {
    printf("Entering state GoingToBed \n");
	_delay_ms(20); // waiting until message is sent over Serial. TODO can be deleted in productive code without serial com
	TCA0.SINGLE.CTRLA =  ~TCA_SINGLE_ENABLE_bm;    /* source (sys_clk/8) +  start timer */
	allLEDoff();
	
	sleep_mode(); // actually going to sleep	
}

void MissedActivationAction() {
    printf("Entering state MissedActivation \n");
}

void ResetAction() {
	n_counts_awake=0;
    printf("Entering state Reset \n");
}

void JustWokeUpAction() {
    printf("Entering state JustWokeUp \n");
	n_counts_awake=0;
}






























