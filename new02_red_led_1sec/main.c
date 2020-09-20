#include <msp430g2553.h>


/**
 * main.c
 */
void main(void)
{
    unsigned int    counter;

    counter = 0;
	WDTCTL = WDTPW + WDTHOLD;	// stop watchdog timer
	P1OUT = 0;
	P1DIR = BIT0;
	for (;;)
	{
	    P1OUT ^= BIT0;
	    for (counter = 0; counter < 60000; counter++){
	    }
	}
}
