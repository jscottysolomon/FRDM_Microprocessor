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

#define STRAIGHT_TIME 1000
#define STOP_TIME 100
#define TURN_TIME 600
#define INIT_DELAY 100

void delay_ms(unsigned short delay_t);
void straight();
void stop();
void figure1();
void turn_left();
void turn_right();

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
	GPIOB->PDDR |= (1<<3);	//B3
	GPIOC->PDDR |= (1<<1);	//C1
	GPIOC->PDDR |= (1<<2);	//C2
	 GPIOC->PDDR |= (1<<3);	//C3

	//Setting input
//	GPIOC->PDDR &= ~(1<<3);	//C3

	//Setting input
//	GPIOC->PDDR &= ~(1<<3);	//C3


	//Turning on thing
	int i = 0;

    while(1) {
    	if(!(GPIOC->PDIR & 0x8)) {
    		figure1();
    	}
    	i++;
    }

    return 0;
}

// void straightS() {
// 	GPIOA->PDOR |= (1<<15); //A15
// 	GPIOA->PDOR |= (1<<12); //A12

// 	// GPIOB->PDOR |= (1<<1); //B1
// 	// GPIOB->PDOR |= (1<<2); //B2

// 	GPIOC->PDOR |= (1<<1); //C1
// 	GPIOC->PDOR |= (1<<2); //C2
// 	// GPIOC->PDOR |= (1<<1); //C1
// 	// GPIOC->PDOR |= (1<<2); //C2
//     while(1) {
// 		if(!(GPIOC->PDIR & 0x8)) {
// 			figure1();
// 		}

//     }
//     return 0 ;
// }

/** Straight S*/
void figure1() {
	delay_ms(INIT_DELAY);
	//Turning on STBY
	GPIOA->PDOR |= (1<<12); //A12

	//Straight
	straight();
	delay_ms(STRAIGHT_TIME);
	stop();
	delay_ms(STOP_TIME);

	//Left Turn, A
	turn_left();
	delay_ms(TURN_TIME);
	stop();
	delay_ms(STOP_TIME);

	//Straight
	straight();
	delay_ms(STRAIGHT_TIME);
	stop();
	delay_ms(STOP_TIME);

	//Left Turn
	turn_left();
	delay_ms(TURN_TIME);
	stop();
	delay_ms(STOP_TIME);

	//Straight
	straight();
	delay_ms(STRAIGHT_TIME);
	stop();
	delay_ms(STOP_TIME);

	//Right Turn
	turn_right();
	delay_ms(TURN_TIME);
	stop();
	delay_ms(STOP_TIME);

	//Straight
	straight();
	delay_ms(STRAIGHT_TIME);
	stop();
	delay_ms(STOP_TIME);

	//Right Turn
	turn_right();
	delay_ms(TURN_TIME);
	stop();
	delay_ms(STOP_TIME);

	//Straight
	straight();
	delay_ms(STRAIGHT_TIME);
	stop();
	delay_ms(STOP_TIME);

	//Turning off STBY
	GPIOA->PDOR &= ~(1<<12); //A12
}

void stop() {
	//A
	GPIOB->PDOR &= ~(1<<2); //B2
	GPIOB->PDOR &= ~(1<<1); //B1

	//B
	GPIOC->PDOR &= ~(1<<1); //C1
	GPIOB->PDOR &= ~(1<<3); //B3
}

void straight() {
	//A
	GPIOB->PDOR |= (1<<2); //B2
	GPIOB->PDOR |= (1<<1); //B1

	//B
	GPIOC->PDOR |= (1<<1); //C1
	GPIOB->PDOR |= (1<<3); //B3
}

void turn_left() {
	GPIOB->PDOR |= (1<<3); //B3
	GPIOC->PDOR |= (1<<1); //C1
}

void turn_right() {

	GPIOB->PDOR |= (1<<2); //B2
	GPIOB->PDOR |= (1<<1); //B1
}

void delay_ms(unsigned short delay_t) {
    SIM->SCGC6 |= (1 << 24); // Clock Enable TPM0
    SIM->SOPT2 |= (0x2 << 24); // Set TPMSRC to OSCERCLK
    TPM0->CONF |= (0x1 << 17); // Stop on Overflow
    TPM0->SC = (0x1 << 7) | (0x07); // Reset Timer Overflow Flag, Set Prescaler 128
    TPM0->MOD = delay_t * 61 + delay_t/2; //

    TPM0->SC |= 0x01 << 3; // Start the clock!

    while(!(TPM0->SC & 0x80)){} // Wait until Overflow Flag
    return;
}
