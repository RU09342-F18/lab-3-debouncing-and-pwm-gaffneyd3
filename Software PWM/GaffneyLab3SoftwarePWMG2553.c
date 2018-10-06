#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	P1DIR |= BIT0; //sets direction of p1.0 to output
	P1DIR &= ~BIT3; //sets direction of P1.3 to input

	P1DIR |= BIT6;//sets direction of p1.6 to output

	P1REN |= BIT3; //enables the pull up-down resistor
	P1OUT |= BIT3; //sets the pull down resistor

	//smclk = 1 Mhz

	//sets clock to 250,000 hz
	TA0CCTL1 = CCIE; //enables interupt
	//CCR0 = 0x3E7F; //sets register to 16000 - 1
	TA0CCR0 = 255; //sets clock to count to 255 and then reset
	TA0CCR1 = 127; //register to change the duty cycle to 50%

	P1IE |= BIT3; //enables button interupt
	P1IES |=BIT3; //sets to rising edge
	P1IFG &= ~BIT3; //clears interupt flag

	TA0CTL = TASSEL_2 + MC_1 + ID_2 + TAIE; //sets clock to: SMclk sets mc to: up mode sets ID to: 4 divider

	__bis_SR_register(GIE);
	while(1);
}
#pragma vector = PORT1_VECTOR;
__interrupt void Port_1(void)
{ //BUTTON CLICK
    if(P1IES & BIT3){//if rising edge of button is detected
        P1OUT |= BIT0;
        P1IES &= ~BIT3;//sets to falling edge
    }else{
        P1OUT &= ~BIT0;
        P1IES |= BIT3;//sets to falling edge
    }

    if(TA0CCR1< 255 && TA0CCR1 >= 0){ //checks if counter is within bounds
        TA0CCR1 += 25; //increments duty cycle by 10%
    }else{
        TA0CCR1 = 0; //resets duty cycle
    }


    P1IFG &= ~BIT3; //clears flag




}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A0(void) {
    //timer 0 toggle LED p1.6
    switch(TA0IV)
    {
        case 2: P1OUT &= ~BIT6; break; // turns of led at compared value
        case 4: break; //CCR2 not used
        case 10: P1OUT |= BIT6; //when overflow turn on
            break;
    }

}
