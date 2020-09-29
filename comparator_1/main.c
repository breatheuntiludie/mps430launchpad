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
	CACTL1 |= CAON;           // включаем компаратор
	_BIS_SR(LPM0_bits + GIE); // режим экономии LPM0 && разрешаем прерывание
}

/*
 * google "#pragma"
 */
#pragma vector = TIMERA0_VECTOR
__interrupt void CCR0_ISR(void)
{
        P1OUT ^= flash;                 // если flash == 0, светодиод не горит
                                        // если flash == LED1, мигаем светодиодом
} // CCR0_ISR

#pragma vector = COMPARATORA_VECTOR
__interrupt void COMPA_ISR(void)
{
  if ((CACTL2 & CAOUT)==0x01)
  {
    CACTL1 |= CAIES;    // значение высокое, ждем спадания фронта
    flash = LED1;       // разрешаем светодиоду мигать
  }
  else
  {
    CACTL1 &= ~CAIES;   // значение низкое, ждем возрастания фронта
    flash = 0;          // отключаем светодиод
    P1OUT = 0;          // обнуляем выход порта P1
  }
} // COMPA_ISR
