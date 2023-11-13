/*
 * @author J. Scotty Solomon
 * @version 12-Nov-23
 */

/**
 * @file    line_sensor.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"

#define LEFT_BOUNDARY 23
#define RIGHT_BOUNDARY 21

#define PWMA 2 //port b
#define PWMB 3 //port b

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

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

	//Enable Ports
	SIM->SCGC5 |= (1 << 10); 	//port b
	SIM->SCGC5 |= (1 << 11); 	//Port C

	// Setup PWM
	SIM->SCGC6 |= (1 << 26); // Clock Enable TPM2
	SIM->SOPT2 |= (0x2 << 24); // Set TPMSRC to OSCERCLK


	//Pin Configuration

	//B0
	PORTB->PCR[0] &= ~0x700;
	PORTB->PCR[0] |= 0x700 & (1 << 8);

	//B1
	PORTB->PCR[1] &= ~0x700;
	PORTB->PCR[1] |= 0x700 & (1 << 8);

	//B2
	PORTB->PCR[2] &= ~0x700;
	PORTB->PCR[2] |= 0x300;  //ALT 3 (TPM2)

	//B3 TPM
	PORTB->PCR[3] &= ~0x700;
	PORTB->PCR[3] |= 0x300;
//	PORTB->PCR[3] |= (3 << 8); //ALT 3 (TPM2)

	//GPIO
	GPIOB->PDDR |= (1<<0);	//B0
	GPIOB->PDDR |= (1<<1);	//B1
	GPIOC->PDDR |= (1<<1);	//C1

	//TPM2
//	TPM2->CONTROLS[0].CnSC |= (0x2 << 2) | (0x2 << 4); // Edge PWM
	TPM2->CONTROLS[0].CnSC |= (0x1 << 2) | (0x1 << 4);
	TPM2->CONTROLS[1].CnSC |= (0x1 << 2) | (0x1 << 4);

	TPM2->MOD = 20000; //
	TPM2->SC |= 0x7;
	TPM2->SC |= 0x01 << 3; // Start the clock!

//	TPM0->CON

	//ADC0 Config
    SIM->SCGC6 |= (1<<27); // Enable ADC Module
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    ADC0->CFG1 = 0;  // Default everything.
    ADC0->SC3 = 0x07; // Enable Maximum Hardware Averaging
	ADC0->SC3 |= 0x80; // Start Calibration

	while(!(ADC0->SC1[0] & 0x80)){	}

	int cal_v = 0;
//	ADC_CFG1_MODE(3)

	// Calibration Complete, write calibration registers.
	cal_v = ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPS;
	cal_v = cal_v >> 1 | 0x8000;
	ADC0->PG = cal_v;

	cal_v = 0;
	cal_v = ADC0->CLM0 + ADC0->CLM1 + ADC0->CLM2 + ADC0->CLM3 + ADC0->CLM4 + ADC0->CLMS;
	cal_v = cal_v >> 1 | 0x8000;
	ADC0->MG = cal_v;

    PRINTF("Hello World\n");


    volatile static int i = 0 ;

    int rightSensor, leftSensor;

    GPIOB->PDOR |= (1<<1); //B1
    GPIOC->PDOR |= (1<<1); //C1

    while(1) {
        i++ ;
        while(1) {
//			ADC0->SC1[0] = 0x03; // Set Channel, starts conversion.
        	ADC0->SC1[0] = 0x05; //pte17, channel AD5a
			while(!(ADC0->SC1[0] & 0x80)){	}
			rightSensor = ADC0->R[0]; // Resets COCO

			ADC0->SC1[0] = 0x01; //pte16, channel DAD1
			while(!(ADC0->SC1[0] & 0x80)){	}
			leftSensor = ADC0->R[0]; // Resets COCO

			TPM2->CONTROLS[0].CnV = 0;


			printf("Right:%d\tLeft%d\n", rightSensor, leftSensor);
		}
    }
    return 0 ;
}
