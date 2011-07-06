//******************************************************************************
//  Seven segment display
//
//  based loosely on some TI example code:
//   MSP430F20xx Demo - Software Toggle P1.0
//
//  Increments a seven segment display from 0-9, repeat
//  
//  If the user presses a button on pin 3
//  it goes into "button mode" and will increment upon each button press
//  and with display an error state of 'E' when incremented higher than 9
//
//******************************************************************************

#include <io.h>
#include <signal.h>

//Define 7 segments to pins
#define SEGA BIT0
#define SEGB BIT1
#define SEGC BIT2
#define SEGD BIT7 //BIT3 is used for sw1
#define SEGE BIT4
#define SEGF BIT5
#define SEGG BIT6

#define     BUTTON                BIT3
#define     BUTTON_OUT            P1OUT
#define     BUTTON_DIR            P1DIR
#define     BUTTON_IN             P1IN
#define     BUTTON_IE             P1IE
#define     BUTTON_IES            P1IES
#define     BUTTON_IFG            P1IFG
#define     BUTTON_REN            P1REN


unsigned int current_number = 0;
unsigned int loop_mode = 1;
unsigned int button_pressed = 0;

void number_six(void);
void display_number(unsigned int);

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

    //Set pins used for segments to output
	P1DIR |= SEGA;
	P1DIR |= SEGB;
	P1DIR |= SEGC;
	P1DIR |= SEGD;
	P1DIR |= SEGE;
	P1DIR |= SEGF;
	P1DIR |= SEGG;
	
	__enable_interrupt();                     // Enable interrupts.
	
	//config button
//	BUTTON_DIR &= ~BUTTON;
//	BUTTON_OUT |= BUTTON;
//	BUTTON_REN |= BUTTON;
//	BUTTON_IES |= BUTTON;
//	BUTTON_IFG &= ~BUTTON;
//	BUTTON_IE |= BUTTON;
	
	P1OUT  = BUTTON;                            // P1.4 set, else reset
	//P1OUT |= BUTTON;
	P1REN |= BUTTON;                            // P1.4 pullup
	P1IE  |= BUTTON;                             // P1.4 interrupt enabled
	P1IES |= BUTTON;                            // P1.4 Hi/lo edge
	P1IFG &= ~BUTTON;                           // P1.4 IFG cleared
	
//	_BIS_SR(LPM4_bits + GIE);


	display_number(0);
	
//	__bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled

	int i;
	volatile unsigned int delay_counter;
	
	//infinite loop
	for (;;)
	{
        //
		if (button_pressed) {
			display_number(current_number);
			//current_number = current_number++ % 10;
			current_number++;
						
			button_pressed = 0;
		}
	

        //loop
		for (i = 0; i < 10 && loop_mode; i++) {
			display_number(i);
		
            //fake some delay
			delay_counter = 50000;
			do (delay_counter--);
			while (delay_counter != 0);
		}

	}
	return 0;
}

void display_number(unsigned int display_this)
{
//	P1OUT = 0x00;
	P1OUT  = BUTTON;
	//P1OUT &= SEGA & SEGB & SEGC & SEGD & SEGE & SEGF & SEGG;
	
	switch (display_this) {
		case 0:
			P1OUT |= SEGA | SEGB | SEGC | SEGD | SEGE | SEGF;
			break;
		case 1:
			P1OUT |= SEGB | SEGC;
			break;
		case 2:
			P1OUT |= SEGA | SEGB | SEGD | SEGE | SEGG;
			break;
		case 3:
			P1OUT |= SEGA | SEGB | SEGC | SEGD | SEGG;
			break;
		case 4:
			P1OUT |= SEGB | SEGC | SEGF | SEGG;
			break;
		case 5:
			P1OUT |= SEGA | SEGC | SEGD | SEGF | SEGG;
			break;
		case 6:
			P1OUT |= SEGA | SEGC | SEGD | SEGE | SEGF | SEGG;
			break;
		case 7:
			P1OUT |= SEGA | SEGB | SEGC | SEGF;
			break;
		case 8:
			P1OUT |= SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGG;
			break;
		case 9:
			P1OUT |= SEGA | SEGB | SEGC | SEGD | SEGF | SEGG;
			break;
		default:
			P1OUT |= SEGA | SEGD | SEGE | SEGF | SEGG;
	}
		
	
}

/* *************************************************************
 * Port Interrupt for Button Press
 * *********************************************************** */
interrupt(PORT1_VECTOR) PORT1_ISR(void)
{

	P1IFG &= ~BUTTON;                           // P1.4 IFG cleared
	
//	
//	BUTTON_IFG = 0;
//	BUTTON_IE &= ~BUTTON;            /* Debounce */
//	WDTCTL = WDT_ADLY_250;
//	IFG1 &= ~WDTIFG;                 /* clear interrupt flag */
//	IE1 |= WDTIE;
	
	button_pressed = 1;
	loop_mode = 0;

}


