
void IdleAction()
{
	statusLEDoff();
	printf("Entering state Idle with n_counts_awake=%d and %d hours and %d seconds of day %d \n", n_counts_awake, h_of_this_day, s_of_this_day, day_counter);
	n_counts_awake++;
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm; /* source (sys_clk/8) +  start timer */
}

void ActivationAction()
{
	printf("Entering state Activation \n");
	n_counts_awake = 0;
	array_day_activation[day_counter] = 1;
}

void GoingToBedAction()
{
	printf("Entering state GoingToBed \n");
	_delay_ms(20);							   // waiting until message is sent over Serial. TODO can be deleted in productive code without serial com
	TCA0.SINGLE.CTRLA = ~TCA_SINGLE_ENABLE_bm; /* source (sys_clk/8) +  start timer */
	allLEDoff();

	sleep_mode(); // actually going to sleep
}

void ResetAction()
{
	printf("Entering state Reset \n");
	n_counts_awake = 0;
	s_of_this_day = 1800;
	h_of_this_day = 21;
	day_counter = 1;
	is_PIT_ISR = 0;
	i = 0;
	statusLEDon();
	

	for (int idx = 0; idx < 32; idx++)
	{
		array_day_activation[idx] = 0; // Initialize each element to 0
	}
}

void JustWokeUpAction()
{
	TCA0.SINGLE.CTRLA = ~TCA_SINGLE_ENABLE_bm; /* source (sys_clk/8) +  start timer */
	printf("Entering state JustWokeUp \n");
	n_counts_awake = 0;
}

void PITAction()
{
}

void WaitUntilReleaseAction()
{
	printf("Entering state WaitUntilRelease \n");
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm; /* source (sys_clk/8) +  start timer */
}

void MissedActivationAction()
{
	printf("Entering state MissedActivation \n");
}
