
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)
static void USART0_sendChar(char c)
{
	while (!(USART0.STATUS & USART_DREIF_bm))
	{
		; /* Wait for USART ready for receiving next char */
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
	PORTMUX.CTRLB = PORTMUX_USART0_ALTERNATE_gc; // Use alt UARTpins
	// PORTA.DIR &= ~PIN3_bm;                  /* Configure RX pin as an input */
	PORTA.DIR |= PIN1_bm; /* Configure TX pin as an output */

	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600);

	USART0.CTRLB |= USART_TXEN_bm; /* Transmitter Enable bit mask. */

	stdout = &USART_stream; /* Bind UART to stdio output stream */
}
