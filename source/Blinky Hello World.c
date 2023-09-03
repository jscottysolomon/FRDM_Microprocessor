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
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    //LED1: Port D, Pin 5
    //LED2: Port E, Pin 29
    //SW1: Port C, Pin 3
    //SW2: Port C, Pin 12

    //Enabling Port C for SW1, SW2
	SIM->SCGC5 |= 1<<11;

    //Enabling Port D for LED1
    SIM->SCGC5 |= 1<<12;

    //Enabling Port E for LED2
    SIM->SCGC5 |= 1<<13;

    //set port C, pin 3 for GPIO
//    PORTC->PCR[3] &= ~0x700;
//	PORTC->PCR[3] |= 0x700 & (1 << 8);
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


    //set port D, pin 5 as output
    GPIOD->PDDR |= (1<<5);

    //set port E, pin 29 as output
    GPIOE->PDDR |= (1<<29);

    //set port C, pin 3 as input
    GPIOC->PDDR &= ~(1<<3);

    //set port C, pin 12 as input
    GPIOC->PDDR &= ~(1<<12);



    //LED2: PTE29
    //SIM_SCGC5 -> bit 13

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        if(!(GPIOC->PDIR & 0x8)) {
			GPIOD->PTOR |= (1<<5);
        }
        else if (i % 100000 == 0) {
        	GPIOD->PTOR |= (1<<5);
            GPIOE->PTOR |= (1<<29);
        }
    }
    return 0 ;
}
