/************************************************/
/*                                              */
/*   made by ggeri / @breatheuntiludie          */
/*   with the help of dmitalexnic@gmail.com     */
/*   for vibro-tactile stimulation in BCI       */
/*   program for non-commercial use             */
/*                                              */
/************************************************/

#include <msp430g2553.h>

#define LED1    BIT0
#define LED2    BIT6
#define AIN1    BIT1

char                flash = 0; // here is our LED (1 or 2)
unsigned long int   count = 0; // typical counter
unsigned long int   seed = 1; // global variable, seed for random expression
unsigned long int   lcg_res = 0; // global variable
unsigned long int   time_rot = 1579; // 1500 == 38 ms
unsigned long int   a = 1103515245; // ft_random variable
unsigned long int   m = 32768; // ft_random variable
int                 c = 12345; // ft_random variable
char                key = 0;
/*
** I use linear congruential expression for reaching num (1 - 8),
** num 8 - target stimulation (P1.0)
*/

/*
** CAIFG - flag, that shows us the part of "front", CAIES - writes this
** if 1 then down, if 0 then up
** as CAIFG, CAIE, CAIES take 0-2 bits
** we can say, that we work with up-front (we write num "2" = 010 -> check main())
*/

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // disable watchdog timer
    P1OUT = 0; // ouput of all P1 pins is logical 0
    P1DIR = BIT0^BIT6; // enable P1.0 and P1.6 pins
/*
**  enable P1.3 as button for changing time_rot
*/
    P1REN |= BIT3; // enable pull up/down resistor for P1.3
    P1OUT |= BIT3; // enable P1.3
    P1IES = BIT3; // interrupt when 1->0
    P1IFG &= ~BIT3;
/*
**  turn on and set comparator for our tasks
*/
    CACTL1 = CAREF1 + CARSEL + CAIE; // 0.5U = Vcc, inverted input, allowed to interrupt
    CACTL2 = P2CA4 + CAF; // non-inverted input CA1 (P1.1 pin for comming signal), output is filtered
    CAPD = AIN1; // disable digital work of P1.1
    CACTL1 |= CAON; // turn on comparator

    __enable_interrupt(); // enable any interruptions during routine of our microcontroller

    for (;;)
    {
        P1OUT |= BIT3;
        P1IE = BIT3; // enable interruptions
    }
}

/*
** google "#pragma" and interruption
*/

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT = 0;
    for (count = 0; count < 60000; count++); //timer
    if (key == 0)
    {
        P1OUT ^= LED1 ^ LED2; // light both LEDs (1)
        for (count = 0; count < 60000; count++);
        P1OUT ^= LED1 ^ LED2; // disable both led
        for (count = 0; count < 60000; count++);
        P1OUT ^= LED1 ^ LED2; // light both LEDs (2)
        for (count = 0; count < 60000; count++);
        time_rot = 3158; // 80ms rotation
        P1OUT ^= LED1 ^ LED2; // disable LEDs
        key++;
        P1IFG = 0; // P1IFG -> Port1 Interrupt Flag (disabled now)
    }
    else if (key == 1)
    {
        P1OUT ^= LED1 ^ LED2; // light both LEDs (1)
        for (count = 0; count < 60000; count++);
        P1OUT ^= LED1 ^ LED2; // disable both led
        for (count = 0; count < 60000; count++);
        P1OUT ^= LED1 ^ LED2; // light both LEDs (2)
        for (count = 0; count < 60000; count++);
        P1OUT ^= LED1 ^ LED2; // disable both led
        for (count = 0; count < 60000; count++);
        P1OUT ^= LED1 ^ LED2; // light both LEDs (3)
        for (count = 0; count < 60000; count++);
        time_rot = 4737; // 120ms rotation
        P1OUT ^= LED1 ^ LED2; // disable LEDs
        key++;
        P1IFG = 0; // P1IFG -> Port1 Interrupt Flag (disabled now)
    }
    else if (key == 2)
    {
        P1OUT ^= LED1 ^ LED2; // light both LEDs (1)
        for (count = 0; count < 60000; count++);
        P1OUT ^= LED1 ^ LED2; // disable both led
        time_rot = 1579; // 40ms rotation
        key = 0;
        P1IFG = 0; // P1IFG -> Port1 Interrupt Flag (disabled now)
    }
}

#pragma vector = TIMERA0_VECTOR
__interrupt void CCR0_ISR(void)
{
    P1OUT ^= flash;
} // CCR0_ISR


#pragma vector = COMPARATORA_VECTOR
__interrupt void COMPA_ISR(void)
{
  if ((CACTL2 & CAOUT) == 0x01)
  {
      seed = ((seed * a + c) / 65536) % m; // take our "random" number
      lcg_res = seed % 9; // making a num for stimulation (depends on our BCI)
      if (lcg_res == 8) // target stimulation
      {
          CACTL1 |= CAIES; // setting Comparator_A interrupt flag CAIFG (CAIFG == 0 bit in CACTL1)
          flash = LED1; // remember led1 -> P1.0
          P1OUT ^= flash; // flash led
          for (count = 0; count < time_rot; count++); // timer of rotation/LED-on
          CACTL1 &= ~CAIES; // low meaning, w8ing for rising front
          flash = 0; // clear
          P1OUT ^= LED1; // turn off led
      }
      else // non-target stimulation
      {
          CACTL1 |= CAIES;
          flash = LED2; // remember led2 -> P1.6
          P1OUT ^= flash;
          for (count = 0; count < time_rot; count++);
          CACTL1 &= ~CAIES;
          flash = 0;
          P1OUT ^= LED2;
      }
  }
  else
  {
      CACTL1 &= ~CAIES;
      flash = 0;
      P1OUT &= ~(LED1 ^ LED2);
  }
} // COMPA_ISR
