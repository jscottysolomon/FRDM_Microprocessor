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
 * @file    Morse_Code.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"

const char* letterMorseCode[] = {
    ".-XX",  // A
    "-...",  // B
    "-.-.",  // C
    "-..",   // D
    ".XXX",  // E
    "..-.",  // F
    "--.X",   // G
    "....",  // H
    "..XX",    // I
    ".---",  // J
    "-.-X",   // K
    ".-..",  // L
    "--XX",    // M
    "-.XX",    // N
    "---X",   // O
    ".--.",  // P
    "--.-",  // Q
    ".-.X",   // R
    "...X",   // S
    "-XXX",     // T
    "..-X",   // U
    "...-X",  // V
    ".--X",   // W
    "-..-",  // X
    "-.--",  // Y
    "--.."   // Z
};

const char* intMorseCode[] = {
    "-----", //0
    ".----", //1
    "..---", //2
    "...--", //3
    "....-", //4
    ".....", //5
    "-....", //6
    "--...", //7
    "---..", //8
    "----.", //9
};

int red = 0; int green = 0;

void switchLED();
void delay_ms(unsigned short);
void displayMorse(char *message, int length);
void handleMorse(char* morse, int len);

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

void displayMorse(char *message, int length) {
	char i = 0;

	while(message[i] != '.') {
		char curr = message[i];

		if(curr == ' ') {
			//delay
//			delay_ms(600);
			switchLED();
		}

		if(curr >= '0' && curr <= '9') {
//			printf("%s ", intMorseCode[curr - '0']);
			handleMorse(intMorseCode[curr - '0'], 4);
		}

		curr = toupper(message[i]);

		if(curr >= 'A' && curr <= 'Z') {
			int index = curr - 'A';
//			printf("%s ",letterMorseCode[index]);
			handleMorse(letterMorseCode[index],2);

		}

		i++;
		if(i > length) {
			printf("went past length!\n");
			break;
		}
	}
}

void switchLED() {
	if(!red && ! green) {
		red = 1;
	} else if(red) {
		green = 1;
		red = 0;
	} else if(green) {
		red = 1;
		green = 0;
	}
}

void handleMorse(char* morse, int len) {
	for(int i = 0; i <= len; i++) {
		int delay = 0;

		if(morse[i] == '-')		delay = 500;
		else if(morse[i] == '.')	delay = 200;
		else break;

		if(red) {
			GPIOE->PDDR |= (1<<29);
			delay_ms(delay);
			GPIOE->PDDR &= ~(1<<29);

		} else {
			GPIOD->PDDR |= (1<<5);
			delay_ms(delay);
			GPIOD->PDDR &= ~(1<<5);
		}

		if(i != len) {
			delay_ms(250);
		}
	}

	delay_ms(600);
}

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    /* Init FSL debug console. */
	#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
		BOARD_InitDebugConsole();
	#endif

    SIM->SCGC5 |= 1<<12;	//Port D for LED1
	SIM->SCGC5 |= 1<<13; 	//Port E for LED2

	//set port D, pin 5 for GPIO
	PORTD->PCR[5] &= ~0x700;
	PORTD->PCR[5] |= 0x700 & (1 << 8);

	//set port E, pin 29 for GPIO
	PORTE->PCR[29] &= ~0x700;
	PORTE->PCR[29] |= 0x700 & (1 << 8);

//	GPIOD->PDDR |= (1<<5);	//port D, pin 5
//	GPIOE->PDDR |= (1<<29);	//port E, pin 29

//    i

    while(1) {
//    	delay_ms(1000);
//        GPIOE->PDDR &= ~(1<<29);
//        delay_ms(1000);
//        GPIOE->PDDR |= (1<<29);

    	char* test = "SOS SOS SOS SOS.";

    	displayMorse(test,26);

    	delay_ms(10000);
    	switchLED();
    }

    return 0 ;
}
