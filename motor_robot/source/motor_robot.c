/*
 *@author: J. Scotty Solomon
 *@version 28-Sep-23
 */

/**
 * @file    motors.c
 * @brief   Application entry point.
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
 */

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    //Enabling Ports
    SIM->SCGC5 |= 1<<9; 	//Port A
    SIM->SCGC5 |= 1<<10;	//Port B
    SIM->SCGC5 |= 1<<11; 	//Port C

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

	//Setting up pins for GPIO output
	GPIOA->PDDR |= (1<<15);	//A15
	GPIOA->PDDR |= (1<<12);	//A12
	GPIOB->PDDR |= (1<<0);	//B0
	GPIOB->PDDR |= (1<<1);	//B1
	GPIOB->PDDR |= (1<<2);	//B2
	GPIOC->PDDR |= (1<<1);	//C1
	GPIOC->PDDR |= (1<<2);	//C2
	GPIOC->PDDR |= (1<<3);	//C3


	//Turning on things
	GPIOA->PDOR |= (1<<15); //A15
	GPIOA->PDOR |= (1<<12); //A12

	GPIOB->PDOR |= (1<<1); //B1
	GPIOB->PDOR |= (1<<2); //B2

	GPIOC->PDOR |= (1<<1); //C1
	GPIOC->PDOR |= (1<<2); //C2

	#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
		BOARD_InitDebugConsole();
	#endif

    PRINTF("Hello World\n");

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}
