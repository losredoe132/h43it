
void IdleAction() {
	n_counts_awake ++;	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;    /* source (sys_clk/8) +  start timer */

    printf("Entering state Idle with n_counts_awake= %d \n", n_counts_awake);
}

void ActivationAction() {
    printf("Entering state Activation \n");
}

void GoingToBedAction() {
    printf("Entering state GoingToBed \n");
	TCA0.SINGLE.CTRLA =  ~TCA_SINGLE_ENABLE_bm;    /* source (sys_clk/8) +  start timer */
	allLEDoff();
	
	sleep_cpu(); // actually going to sleep
	
	
}

void MissedActivationAction() {
    printf("Entering state MissedActivation \n");
}

void ResetAction() {
    printf("Entering state Reset \n");
}












