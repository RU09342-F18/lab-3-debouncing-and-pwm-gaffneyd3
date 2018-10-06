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

    P2SEL0 |= BIT0;//sets pwm output as the LED
    P2SEL1 &= ~BIT0;//sets pwm output as the LED

    P1REN |= BIT1; //enables the pull up-down resistor
    P1OUT |= BIT1; //sets the pull down resistor

    TB1CCTL1 = OUTMOD_7; //enables interupt
    TB1CCR0 = 1000; //sets clock to count to 255 and then reset
    TB1CCR1 = 500; //register to change the duty cycle to 50%

    P1IE |= BIT1; //enables button interupt
    P1IES |=BIT1; //sets to rising edge
    P1IFG &= ~BIT1; //clears interupt flag

    TB1CTL = TBSSEL_2 + MC_1 + ID_2; //sets clock to: SMclk sets mc to: up mode sets ID to: 4 divider

    __bis_SR_register(LPM0_bits + GIE);

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

    if(TB1CCR1< 1000 && TB1CCR1 >= 0){ //checks if counter is within bounds
        TB1CCR1 += 100; //increments duty cycle by 10%
    }else if(TBCCR1 >= 1000){

        TB1CCR1 = 0;
    }else{
        TB1CCR1 = 100; //resets duty cycle
    }

    P1IFG &= ~BIT1; //clears flag


}
