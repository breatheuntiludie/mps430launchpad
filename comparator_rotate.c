#include <msp430g2553.h>

#define LED1    BIT0
#define LED2    BIT6
#define AIN1    BIT1

char    flash = 0;
unsigned long int   count = 0;
unsigned long int   seed = 1; // global variable
unsigned long int   lcg_res = 0; // global variable
unsigned long int   time_rot = 20000;
/*
** linear congruential generator
*/
unsigned long int ft_random()
{
    unsigned long int a = 1103515245;
    unsigned long int m = 32768;
    int c = 12345;

    seed = ((seed * a + c) / 65536) % m;
    return (seed);
}

void    ft_rotate()
{
    lcg_res = ft_random() % 9;

    if (lcg_res == 8)
    {
        for (count = 0; count < 60000; count++);
        {
            CACTL1 |= CAIES;
            flash = LED1;
            P1OUT ^= flash;
        }
        for (count = 0; count < time_rot; count++); // timer
        {
            CACTL1 &= ~CAIES;   //
            flash = 0;          //
            P1OUT = 0;
        }
    }
    else
    {
        for (count = 0; count < 60000; count++);
        {
            CACTL1 |= CAIES;
            flash = LED2;
            P1OUT ^= flash;
        }
        for (count = 0; count < time_rot; count++); // timer
        {
            CACTL1 &= ~CAIES;   //
            flash = 0;          //
            P1OUT = 0;
        }
    }
}

/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // stop watchdog timer
    P1OUT = 0;
    P1DIR = BIT0^BIT6;
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
      ft_rotate();
  else
  {
    CACTL1 &= ~CAIES;   //
    flash = 0;          //
    P1OUT = 0;          // Led = 0
  }
} // COMPA_ISR
