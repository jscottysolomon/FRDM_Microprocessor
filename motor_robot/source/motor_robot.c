/*
 *@author: J. Scotty Solomon
 *@version 28-Sep-23
 *
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/*
 * Pin Mappings:
 *
 * PWMA: B2
 * AI1:  B1
 * AI1:  B0
 *
 * PWMB: B3
 * BI1:  C1
 * PI2:  C2
 *
 * VCC:  A15
 * STBY: A17
 *
 * SW1: C3
 *
 */

/*
 * @brief   Application entry point.
 */

void straightS();

volatile int inProcess = 0;

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

	#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
		BOARD_InitDebugConsole();
	#endif

    //Enabling Ports
    SIM->SCGC5 |= 1<<9; 	//Port A
    SIM->SCGC5 |= 1<<10;	//Port B
    SIM->SCGC5 |= 1<<11; 	//Port C

    //Enabling Pins
    //A15
	PORTA->PCR[15] &= ~0x700;
	PORTA->PCR[15] |= 0x700 & (1 << 8);

	//A12
	PORTA->PCR[12] &= ~0x700;
	PORTA->PCR[12] |= 0x700 & (1 << 8);

	//B0
	PORTB->PCR[0] &= ~0x700;
	PORTB->PCR[0] |= 0x700 & (1 << 8);

	//B1
	PORTB->PCR[1] &= ~0x700;
	PORTB->PCR[1] |= 0x700 & (1 << 8);

	//B2
	PORTB->PCR[2] &= ~0x700;
	PORTB->PCR[2] |= 0x700 & (1 << 8);

	//B3
	PORTB->PCR[3] &= ~0x700;
	PORTB->PCR[3] |= 0x700 & (1 << 8);

	//C1
	PORTC->PCR[1] &= ~0x700;
	PORTC->PCR[1] |= 0x700 & (1 << 8);

	//C2
	PORTC->PCR[2] &= ~0x700;
	PORTC->PCR[2] |= 0x700 & (1 << 8);

	//C3
	PORTC->PCR[3] &= ~0x703;
	PORTC->PCR[3] |= 0x703 & ((1 << 8) | 0x3); //enable pull ups


	//Setting up pins for GPIO output
	GPIOA->PDDR |= (1<<15);	//A15
	GPIOA->PDDR |= (1<<12);	//A12
	GPIOB->PDDR |= (1<<0);	//B0
	GPIOB->PDDR |= (1<<1);	//B1
	GPIOB->PDDR |= (1<<2);	//B2
	GPIOC->PDDR |= (1<<1);	//C1
	GPIOC->PDDR |= (1<<2);	//C2
	GPIOC->PDDR |= (1<<3);	//C3

	//Setting input
	GPIOC->PDDR &= ~(1<<3);	//C3


	//Turning on things

    while(1) {
    	if(!(GPIOC->PDIR & 0x8)) {
    		straightS();
    	}
    }

    return 0;
}

void straightS() {
	GPIOA->PDOR |= (1<<15); //A15
	GPIOA->PDOR |= (1<<12); //A12

	GPIOB->PDOR |= (1<<1); //B1
	GPIOB->PDOR |= (1<<2); //B2

	GPIOC->PDOR |= (1<<1); //C1
	GPIOC->PDOR |= (1<<2); //C2
}
