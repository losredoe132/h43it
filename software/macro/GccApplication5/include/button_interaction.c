volatile uint16_t consecutive_counts_pressed ;
volatile uint16_t consecutive_counts_released ;
volatile uint16_t pit_interrupts_since_last_increase;



void wait_until_button_released()
{
	while(consecutive_counts_released<6){printf("Waiting for release with consecutive_counts_pressed:%d, consecutive_counts_released:%d\n",consecutive_counts_pressed, consecutive_counts_released );}
	consecutive_counts_pressed=0;
	consecutive_counts_released=0;
	printf("Released\n");
}