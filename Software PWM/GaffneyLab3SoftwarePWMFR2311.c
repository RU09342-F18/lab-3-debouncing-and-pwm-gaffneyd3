#include <msp430.h>


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //unlocks the gpio pins


    P1DIR |= BIT0; //sets direction of p1.0 to output
    P2DIR |= BIT0; //sets direction of p2.0 to output
    P1DIR &= ~BIT1; //sets direction of P1.3 to input

    P1REN |= BIT1; //enables the pull up-down resistor
    P1OUT |= BIT1; //sets the pull down resistor

    TB0CCTL1 = CCIE; //enables interupt
    TB0CCR0 = 255; //sets clock to count to 255 and then reset
    TB0CCR1 = 127; //register to change the duty cycle to 50%

    P1IE |= BIT1; //enables button interupt
    P1IES |=BIT1; //sets to rising edge
    P1IFG &= ~BIT1; //clears interupt flag

    TB0CTL = TBSSEL_2 + MC_1 + ID_2 + TBIE; //sets clock to: SMclk sets mc to: up mode sets ID to: 4 divider

    __bis_SR_register(GIE);
    while(1);
}
#pragma vector = PORT1_VECTOR;
__interrupt void Port_1(void)
{ //BUTTON CLICK
    if(P1IES & BIT1){//if rising edge of button is detected
        P1OUT |= BIT0;
        P1IES &= ~BIT1;//sets to falling edge
    }else{
        P1OUT &= ~BIT0;
        P1IES |= BIT1;//sets to falling edge
    }

    if(TB0CCR1< 255 && TB0CCR1 >= 0){ //checks if counter is within bounds
        TB0CCR1 += 25; //increments duty cycle by 10%
    }else{
        TB0CCR1 = 0; //resets duty cycle
    }


    P1IFG &= ~BIT1; //clears flag




}
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B0(void) {
    //timer 0 toggle LED p1.6
    switch(TB0IV)//seperates the different cases for the clocks interupt vector
    {
        case 2: P2OUT &= ~BIT0; break; // turns of led at compared value
        case 4: break; //CCR2 not used
        case 14: P2OUT |= BIT0; //when overflow turn on
            break;
    }

}
