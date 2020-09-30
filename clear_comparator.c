#include <msp430g2553.h>

#define LED1    BIT0
#define AIN1    BIT1

char    flash = 0;
/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// stop watchdog timer
	P1OUT = 0;
	P1DIR = BIT0;
	CACTL1 = CAREF1 + CARSEL + CAIE;
	/*
	**  0.5U = Vcc, inverted input, allowed to interrupt
    */
	CACTL2 = P2CA4 + CAF; // uninverted input CA1, output is filtered
	CAPD = AIN1; // google it
	CACTL1 |= CAON;           //
	_BIS_SR(LPM0_bits + GIE); //
}

/*
 * google "#pragma"
 */
#pragma vector = TIMERA0_VECTOR
__interrupt void CCR0_ISR(void)
{
        P1OUT ^= flash;
} // CCR0_ISR

#pragma vector = COMPARATORA_VECTOR
__interrupt void COMPA_ISR(void)
{
  if ((CACTL2 & CAOUT)==0x01)
  {
    CACTL1 |= CAIES;    //
    flash = LED1;       //
    P1OUT ^= flash;
  }
  else
  {
    CACTL1 &= ~CAIES;   //
    flash = 0;          //
    P1OUT = 0;          // Led = 0
  }
} // COMPA_ISR
