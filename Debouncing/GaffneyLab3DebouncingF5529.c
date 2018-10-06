#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR |= BIT0; //sets direction
    P1DIR &= ~BIT1; //sets direction to input

    P1REN |= BIT1; //enables the pull up-down resistor
    P1OUT |= BIT1; //enables pull down resistor

    P1IE |= BIT1; //enables the interupt for p1.1
    P1IES |= BIT1; //enables rising edge for p1.1
    P1IFG &= ~BIT1; //clears the flag

    TB0CTL=TBSSEL_1 + MC_0 + ID_3; //TASSEL_1 sets it to SMCLK ID_3 sets it to 8 divider, MC_0 sets it to off

    TB0CCTL0 |= CCIE; //ENABLES INTERUPT
    TB0CCR0 = 650; //sets the CCR to count to 32K/8/6.1 = 650

    _BIS_SR(LPM0_bits + GIE);//enable global interupts and low power mode enable

    return 0;
}
#pragma vector = PORT1_VECTOR;
    __interrupt void Port_1(void)
    {
        TB0CTL=TBSSEL_1 + MC_1 + ID_3; //TASSEL_1 sets it to SMCLK ID_3 sets it to 8 divider, MC_1 sets it to up
        P1IE &= ~BIT1; //interupt to falling edge
    }
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0(void) {
    //timer 0 toggle LED p1.6
    P1OUT ^= BIT0;//toggles P1.0 to be on or off
    TB0CTL=TBSSEL_1 + MC_0 + ID_3; //TASSEL_1 sets it to SMCLK ID_3 sets it to 8 divider, MC_0 sets it to off, TACLR
    TBCTL |= TBCLR; //clear clock
    P1IFG &= ~BIT1; //clears flag
    P1IE |= BIT1; //interupt to rising edge

}
