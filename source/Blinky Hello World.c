/*
 * @author:	J. Scotty Solomon
 * @date:	09/23/23
 * @file    Blinky Hello World.c
 * @brief   Application entry point.
 *
 * Traffic light simulator. LEDs blink in order of red, green, and yellow.
 * If a switch is pressed (or held down) only the yellow LED blinks whereas
 * the other LEDs stay off.
 *
 * The red and green LEDs are the LEDs that are built into the FRDM board.
 * The yellow LED is connected via a breadboard.
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"

/*
 * The code below is taken from the video entitle "02 - Generating Events" and is
 * used as a busy delay. I did not write the code for this function.
 */
int delay_ms(unsigned short delay_t) {
    SIM->SCGC6 |= (1 << 24); // Clock Enable TPM0
    SIM->SOPT2 |= (0x2 << 24); // Set TPMSRC to OSCERCLK
    TPM0->CONF |= (0x1 << 17); // Stop on Overflow
    TPM0->SC = (0x1 << 7) | (0x07); // Reset Timer Overflow Flag, Set Prescaler 128
    TPM0->MOD = delay_t * 61 + delay_t/2; //

    TPM0->SC |= 0x01 << 3; // Start the clock!

    while(!(TPM0->SC & 0x80)){} // Wait until Overflow Flag
    return 1;
}

//only have to pick one switch just put it in project description

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    /* Init FSL debug console. */
	#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
		BOARD_InitDebugConsole();
	#endif

    //LED1: Port D, Pin 5
    //LED2: Port E, Pin 29
    //LED3: Port A, Pin 12
    //SW1: Port C, Pin 3
    //SW2: Port C, Pin 12

    //Enabling Ports
    SIM->SCGC5 |= 1<<9; 	//Port A for LED3
	SIM->SCGC5 |= 1<<11; 	//Port C for SW1, SW2
    SIM->SCGC5 |= 1<<12;	//Port D for LED1
    SIM->SCGC5 |= 1<<13; 	//Port E for LED2

    //set port A, pin 3 for GPIO
    PORTA->PCR[12] &= ~0x700;
	PORTA->PCR[12] |= 0x700 & (1 << 8);

    //set port C, pin 3 for GPIO
    PORTC->PCR[3] &= ~0x703;
   	PORTC->PCR[3] |= 0x703 & ((1 << 8) | 0x3); //enable pull ups

	//set port C, pin 12 for GPIO
	PORTC->PCR[12] &= ~0x700;
	PORTC->PCR[12] |= 0x700 & (1 << 8);

    //set port D, pin 5 for GPIO
    PORTD->PCR[5] &= ~0x700;
    PORTD->PCR[5] |= 0x700 & (1 << 8);

    //set port E, pin 29 for GPIO
    PORTE->PCR[29] &= ~0x700;
    PORTE->PCR[29] |= 0x700 & (1 << 8);

    //Setting output
    GPIOA->PDDR |= (1<<12);	//port A, pin 1
    GPIOD->PDDR |= (1<<5);	//port D, pin 5
    GPIOE->PDDR |= (1<<29);	//port E, pin 29

    //Setting input
    GPIOC->PDDR &= ~(1<<3);	//port C, pin 3
    GPIOC->PDDR &= ~(1<<12);//port C, pin 12

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    volatile static int x = 0 ;
    volatile static int test = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */

//    GPIOD->PTOR &= ~(1<<5);
//	GPIOE->PTOR &= ~(1<<29);

    while(1) {
        i++;

        if(!(GPIOC->PDIR & 0x8) || x == 1) {
        	GPIOD->PDOR |= (1<<5);
        	GPIOE->PDOR |= (1<<29);
        	x = 1;
        }
//        else {
//        	x = 0;
//        }

		if (x == 1) {
			GPIOA->PDOR |= (1<<12);
			test += delay_ms(1000);
//			GPIOA->PDOR &= ~(1<<12);
		}
//		else if (i % 100000 == 0) {
		else {

        	GPIOD->PDOR |= (1<<5);
        	test += delay_ms(1000);
        	GPIOD->PDOR &= ~(1<<5);
//        	test += delay_ms(10);

        	GPIOA->PDOR |= (1<<12);
			test += delay_ms(1000);
			GPIOA->PDOR &= ~(1<<12);
//			delay_ms(1000);

            GPIOE->PDOR |= (1<<29);
            test += delay_ms(1000);
            GPIOE->PDOR &= ~(1<<29);


        }
    }

    /*
	 * Okay so evidently don't need variables just used
	 * while loops with variables
	 */
	static int count = 0;
	static int switch_has_been_pressed = 0; switch_is_on = 0; switch_is_off = 0;

	while(1) {
		//checking switch state
		if(!(GPIOC->PDIR & 0x8)) {
			switch_has_been_pressed = 1;
			count = 0;

			//turn off red and green
		}

		if(switch_has_been_pressed) {
			if(switch_is_on) {
				//turn on yellow
				if(!count) {
					//turn yellow on
				} else if(count == 40) {
					//turn off led
				} else if(count == 80) {
					//reset count
					count = 0;
				}
			} else if(switch_is_off) {
				//turn off yellow;
			}

		}
		else if(count <= 40) {
			//turn on green
		} else if(count <= 80) {
			//turn on yellow
		} else if(count <= 120) {
			//turn on red
		} else {
			count = 0;
		}

		count++;
	}
    return 0 ;
}
