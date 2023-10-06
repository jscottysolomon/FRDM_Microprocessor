/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    PWM_Example.c
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

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
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

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    	SIM->SCGC5 |= 1<<10;	//Port B

    	// Setup PWM
        SIM->SCGC6 |= (1 << 24); // Clock Enable TPM0
        SIM->SCGC6 |= (1 << 26); // Clock Enable TPM2
        SIM->SOPT2 |= (0x2 << 24); // Set TPMSRC to OSCERCLK

        //B2 TPM
        PORTB->PCR[2] &= ~0x700;
		PORTB->PCR[2] |= (3 << 8); //ALT 3 (TPM2)

		//B3 TPM
		PORTB->PCR[3] &= ~0x700;
		PORTB->PCR[3] |= (3 << 8); //ALT 3 (TPM2)

		// Enable LED1 as PWM
        SIM->SCGC5 |= (1 << 12);
        PORTD->PCR[5] &= ~(0x700); // Clear MUX
        PORTD->PCR[5] |= (4 << 8); //ALT 4 (TPM0_CH5)


        // Setup Channel 5
        TPM0->CONTROLS[5].CnSC |= (0x2 << 2) | (0x2 << 4);  // Edge PWM
        TPM0->MOD = 7999;  //
        TPM0->SC |= 0x01 << 3; // Start the clock!

        TPM2->CONTROLS[0].CnSC |= (0x2 << 2) | (0x2 << 4);  // Edge PWM
		TPM2->MOD = 7999;  //
		TPM2->SC |= 0x01 << 3; // Start the clock!

        TPM2->CONTROLS[1].CnSC |= (0x2 << 2) | (0x2 << 4);  // Edge PWM
		TPM2->MOD = 7999;  //
		TPM2->SC |= 0x01 << 3; // Start the clock!

       /* This for loop should be replaced. By default this loop allows a single stepping. */
        int i = 0;
        while(1) {
            i++;
            if(i > 7999)
             i=0;
            // Set new PWM value
            TPM0->CONTROLS[5].CnV = i;
            TPM2->CONTROLS[1].CnV = i;
            TPM2->CONTROLS[0].CnV = i;
            delay_ms(5000);
           }
          /* Never leave main */
        return 0;
}
