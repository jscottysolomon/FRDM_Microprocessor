/*
 * @author:	J. Scotty Solomon
 * @date:	09/23/23
 * @file    Blinky Hello World.c
 * @brief   Application entry point.
 *
 * Traffic light simulator. LEDs blink in order of green, yellow, and red
 * If the SW1 switch is held down, only the yellow LED blinks whereas
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

    //idk why this works
    GPIOD->PDOR &= ~(1<<5);
    GPIOD->PDOR |= (1<<5);
    GPIOD->PDOR |= (1<<5);
    GPIOE->PDOR |= (1<<29);

    /*
	 * Okay so evidently don't need variables just used
	 * while loops with variables
	 */
	static int count = 0;
	static int switch_has_been_pressed = 0;
	static int green = 0, yellow = 0, red = 0;
	static int yellow_cycle = 0;

	while(1) {
		//checking switch state
		if(!(GPIOC->PDIR & 0x8)) {
			if(!yellow_cycle&& !switch_has_been_pressed) {
				switch_has_been_pressed = 1;
				count = 0;

				if(yellow) {
					GPIOA->PDOR &= ~(1<<12);
					yellow = 0;
				}
			}

			//turn off red and green
			if(red) {
				GPIOE->PDOR |= (1<<29);
				red = 0;
			}
			if(green) {
				GPIOD->PDOR |= (1<<5);
				green = 0;
			}

		} else {
			switch_has_been_pressed = 0;
			if(yellow_cycle) {
				yellow_cycle = 0;
				count = 0;
			}
		}

		if(switch_has_been_pressed) {
			//yellow cycle
			yellow_cycle = 1;

			if(count == 0) {
				//turn yellow on
				GPIOA->PDOR |= (1<<12);
				yellow = 1;
			} else if(count == 900000) {
				//turn off yellow
				GPIOA->PDOR &= ~(1<<12);
				yellow = 0;
			} else if(count == 1800000) {
				//reset count
				count = -1;
			}

		}
		else if(count == 0) {
			//check if yellow is on (from yellow cycle) and if so turn it off
			if(yellow) {
				GPIOA->PDOR &= ~(1<<12);
				yellow = 0;
			}

			//turn on green
			GPIOD->PDOR &= ~(1<<5);
			green = 1;
		} else if(count == 900000) {
			//turn on yellow, turn off green
			GPIOD->PDOR |= (1<<5);
			GPIOA->PDOR |= (1<<12);
			green = 0;
			yellow = 1;
		} else if(count == 1800000) {
			//turn on red, turn off yellow
			GPIOA->PDOR &= ~(1<<12);
			GPIOE->PDOR &= ~(1<<29);
			yellow = 0;
			red = 1;
		} else if(count == 2700000) {
			//turn off red, reset count
			GPIOE->PDOR |= (1<<29);
			red = 0;
			count = -1;
		}

		count++;
	}
    return 0 ;
}
