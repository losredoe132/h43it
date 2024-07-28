

void RTCA_init(){
	RTC.CLKSEL = RTC_CLKSEL_INT1K_gc;				// 1024 Hz from OSCULP32K
	RTC.CTRLA = RTC_RTCEN_bm;					// enable RTC
	RTC.PITINTCTRL = RTC_PI_bm;					// enable periodic interrupt
	//RTC.PITCTRLA = RTC_PERIOD_CYC32768_gc | RTC_PITEN_bm;		// set period; enable PIT
	RTC.PITCTRLA = RTC_PERIOD_CYC1024_gc | RTC_PITEN_bm;		// set period; enable PIT
	
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

uint8_t consecutive_counts_pressed=0;
uint8_t  consecutive_counts_released=0;

ISR(PORTA_PORT_vect) {
	PORTA.INTFLAGS |= btn_pin; // Clear interrupt flag
	is_PIT_ISR = 0; 
}

ISR(RTC_PIT_vect)
{
	is_PIT_ISR = 1; 
	s_of_this_day++;
	if (s_of_this_day > 60*60){
		h_of_this_day++; 
		s_of_this_day=0; 
	}
	if (h_of_this_day > 24){
		printf("New day: %d\n", day_counter);
		day_counter++;
		s_of_this_day = 0; 
		h_of_this_day = 0 ; 
	}

	RTC.PITINTFLAGS = RTC_PI_bm;// Clear interrupt flag
}



ISR(TCA0_OVF_vect)
{
	// "Running" trough the LED table defined above and switch LEDs on & off according to the x value
	i++;

	if (array_day_activation[i]==1){LEDOnById(i);}
	else{allLEDoff();}

	if (i>30){i=0;} // i overflow carrying

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