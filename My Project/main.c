#define F_CPU 3333333
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

#include <stdio.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

void RTC_init(void);
void USART0_init(void);
void CLKCTRL_init(void);
void ADC0_init(void);

static void USART0_sendChar(char c)
{
	while (!(USART0.STATUS & USART_DREIF_bm))
	{
		;                                   /* Wait for USART ready for receiving next char */
	}
	USART0.TXDATAL = c;
}

static int USART0_printChar(char c, FILE *stream)
{
	USART0_sendChar(c);
	return 0;
}

static FILE USART_stream = FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);

void USART0_init(void)
{

	PORTB.DIR &= ~PIN3_bm;                  /* Configure RX pin as an input */
	PORTB.DIR |= PIN2_bm;                   /* Configure TX pin as an output */

	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600);

	USART0.CTRLB |= USART_TXEN_bm;          /* Transmitter Enable bit mask. */

	stdout = &USART_stream;                 /* Bind UART to stdio output stream */
}




int main(void)
{
	USART0_init();

	
	while (1)
	{
		printf("hello from ATtiny printf"); 
	}
}