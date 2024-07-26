

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