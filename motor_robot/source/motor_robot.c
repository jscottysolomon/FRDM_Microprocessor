/*
 *@author: J. Scotty Solomon
 *@version 28-Sep-23
 *
 * This project showcases motor driving via a microprocessor. The car can go in two fixed
 * paths, a straight S or a curved S. Below is the pin map for the FRDM board and the motor driver:
 *
 * Driver | FRDM Board
 * PWMA: B2
 * AI1:  B1
 * AI1:  B0
 *
 * PWMB: B3
 * BI1:  C1
 * PI2:  C2
 *
 * VCC and STBY: P3V3
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
#define INIT_DELAY 200

void delay_ms(unsigned short delay_t);
void straight();
void stop();
void figure1();
void figure2();
void turn_left();
void turn_right();
void curve_right();
void curve_left();

/*
 * Main entry point, which enables the the neccessary GPIO pins and switches.
 *
 * Afterward, the system enters a busy loop until either switch is pressed. Switch
 * 1 causes the car to move in the path of a straight S. Switch 2 causes the car to
 * move in the path of a curved S.
 */
int main(void) {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

	#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
		BOARD_InitDebugConsole();
	#endif

    //Enabling Ports
    SIM->SCGC5 |= 1<<10;	//Port B
    SIM->SCGC5 |= 1<<11; 	//Port C

    //Enabling Pins
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

	//C12
	PORTC->PCR[12] &= ~0x703;
	PORTC->PCR[12] |= 0x703 & ((1 << 8) | 0x3); //enable pull ups


	//Setting up pins for GPIO output
	GPIOB->PDDR |= (1<<0);	//B0
	GPIOB->PDDR |= (1<<1);	//B1
	GPIOB->PDDR |= (1<<2);	//B2
	GPIOB->PDDR |= (1<<3);	//B3
	GPIOC->PDDR |= (1<<1);	//C1
	GPIOC->PDDR |= (1<<2);	//C2

	//Setting input switches
	GPIOC->PDDR &= ~(1<<3);	//C3
	GPIOC->PDDR &= ~(1<<12);//C12

	int i = 0;

    while(1) {
    	//SW1
    	if(!(GPIOC->PDIR & 0x8)) {
    		figure1();
    	}

    	//Sw2
    	if(!(GPIOC->PDIR & (1 <<12))) {
			figure2();
		}
    	i++;

    }
    return 0;
}

/** Straight S*/
void figure1() {
	delay_ms(INIT_DELAY);

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
	delay_ms(TURN_TIME*1.07);
	stop();
	delay_ms(STOP_TIME);

	//Straight
	straight();
	delay_ms(STRAIGHT_TIME);
	stop();
	delay_ms(STOP_TIME);

	//Right Turn
	turn_right();
	delay_ms(TURN_TIME*1.2);
	stop();
	delay_ms(STOP_TIME);

	//Straight
	straight();
	delay_ms(STRAIGHT_TIME);
	stop();
	delay_ms(STOP_TIME);

	//Right Turn
	turn_right();
	delay_ms(TURN_TIME*1.34);
	stop();
	delay_ms(STOP_TIME);

	//Straight
	straight();
	delay_ms(STRAIGHT_TIME);
	stop();
	delay_ms(STOP_TIME);
}

/*Curved S*/
void figure2() {
	delay_ms(INIT_DELAY);
	int x = 0;

	while(x < 27) {
		curve_left();

		delay_ms(25);
		stop();
		x++;
	}

	x = 0;

	while(x < 30) {
		curve_right();
		delay_ms(25);
		stop();
		x++;
	}
}

/**
 * Curves to the left by letting the left wheel spin
 * more frequently than the right wheel
 */
void curve_left() {
	GPIOC->PDOR |= (1<<1); //C1
	GPIOB->PDOR |= (1<<3); //B3

	delay_ms(50);

	GPIOB->PDOR |= (1<<2); //B2
	GPIOB->PDOR |= (1<<1); //B1
}

/**
 * Curves to the right by letting the right wheel spin
 * more frequently than the left wheel
 */
void curve_right() {
	//A
	GPIOB->PDOR |= (1<<2); //B2
	GPIOB->PDOR |= (1<<1); //B1
	delay_ms(75);
	//B
	GPIOC->PDOR |= (1<<1); //C1
	GPIOB->PDOR |= (1<<3); //B3
}

/**
 * Signals to motor driver to stop all wheels
 */
void stop() {
	//A
	GPIOB->PDOR &= ~(1<<2); //B2
	GPIOB->PDOR &= ~(1<<1); //B1

	//B
	GPIOC->PDOR &= ~(1<<1); //C1
	GPIOB->PDOR &= ~(1<<3); //B3
}

/**
 * Sets left and right wheels to go straight at
 * an even pace
 */
void straight() {
	//A
	GPIOB->PDOR |= (1<<2); //B2
	GPIOB->PDOR |= (1<<1); //B1

	//B
	GPIOC->PDOR |= (1<<1); //C1
	GPIOB->PDOR |= (1<<3); //B3
}

/**
 * Turns on the right wheel so the car
 * does an even turn of 90 degrees
 */
void turn_left() {
	GPIOB->PDOR |= (1<<3); //B3
	GPIOC->PDOR |= (1<<1); //C1
}

/**
 * Turns on the left wheel so the car
 * does an even turn of 90 degrees
 */
void turn_right() {

	GPIOB->PDOR |= (1<<2); //B2
	GPIOB->PDOR |= (1<<1); //B1
}

/**
 * This code is from the lecture slides. It creates a busy timer
 * that will not return until delay_t has passed
 *
 * @delay_t: delay time in milliseconds
 */
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
