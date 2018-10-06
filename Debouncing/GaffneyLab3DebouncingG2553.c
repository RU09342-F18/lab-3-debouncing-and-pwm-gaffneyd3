#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL3 = LFXT1S_2; //tells MC to use internal crystal

    P1DIR |= BIT0; //sets direction of p1.0 to output
    P1DIR &= ~BIT3; //sets direction of P1.3 to input

    P1REN |= BIT3; //enables the pull up-down resistor
    P1OUT |= BIT3; //sets the pull down resistor

    P1DIR &= ~BIT3; //sets input to button

    TA0CTL = TASSEL_1 + MC_0 + ID_3; //sets clock to: A clk sets mc to: off mode sets ID to: 8 divider
    CCTL0 = CCIE; //enables interupt
    CCR0 = 200; //sets register to 200

    P1IE |= BIT3; //enables button interupt
    P1IES |=BIT3; //sets to rising edge
    P1IFG &= ~BIT3; //clears interupt flag

    _BIS_SR(LPM0_bits + GIE);

    return 0;
}
#pragma vector = PORT1_VECTOR;
__interrupt void Port_1(void)
{ //BUTTON CLICK
    TA0CTL = TASSEL_1 + MC_1 + ID_3; //sets clock to: A clk sets mc to: up mode sets ID to: 8 divider
    P1IE &= ~BIT3; //enables button interupt



}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void) {
    //timer 0 toggle LED p1.6
    TA0CTL = TASSEL_1 + MC_0 + ID_3; //sets clock to: A clk sets mc to: up mode sets ID to: 8 divider
    P1OUT ^= BIT0;
    TACTL |= TACLR;
    P1IE |= BIT3; //enables button interupt
    P1IFG &= ~BIT3; //clears flag

}
