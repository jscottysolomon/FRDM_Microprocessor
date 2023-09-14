/**
 * @author J. Scotty Solomon
 * @date 14-September-23
 *
 * Converts a string into morse code and displays the message
 * on LED1 and LED2.
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

#define PERIOD_DELAY 600
#define LETTER_LENGTH 3
#define NUMBER_LENGTH 4

const char* letterMorseCode[] = {
    ".-XX",  // A
    "-...",  // B
    "-.-.",  // C
    "-..",   // D
    ".XXX",  // E
    "..-.",  // F
    "--.X",  // G
    "....",  // H
    "..XX",  // I
    ".---",  // J
    "-.-X",  // K
    ".-..",  // L
    "--XX",  // M
    "-.XX",  // N
    "---X",  // O
    ".--.",  // P
    "--.-",  // Q
    ".-.X",  // R
    "...X",  // S
    "-XXX",  // T
    "..-X",  // U
    "...-X", // V
    ".--X",  // W
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
void handleMorse(const char* morse, int len);

/**
 * Enables input/output for LED1/2 and SW1. Enters a
 * permanent loop in which a string is displayed as
 * morse code once. String can be displayed again by
 * pressing SW1.
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

	//Enabling Ports
	SIM->SCGC5 |= 1<<12;	//Port D for LED1
	SIM->SCGC5 |= 1<<13; 	//Port E for LED2
	SIM->SCGC5 |= 1<<11; 	//Port C for SW1

	//set port D, pin 5 for GPIO
	PORTD->PCR[5] &= ~0x700;
	PORTD->PCR[5] |= 0x700 & (1 << 8);

	//set port E, pin 29 for GPIO
	PORTE->PCR[29] &= ~0x700;
	PORTE->PCR[29] |= 0x700 & (1 << 8);

	//set port C, pin 3 for GPIO
	PORTC->PCR[3] &= ~0x703;
	PORTC->PCR[3] |= 0x703 & ((1 << 8) | 0x3); //enable pull ups

	//Setting input
	GPIOC->PDDR &= ~(1<<3);	//port C, pin 3

	int i = 0;
	char * str = "CSE 325";

    while(1) {
    	if(i == 0) {
    		red = green = 0;
    		displayMorse(str,strlen(str));
    	}

    	if(!(GPIOC->PDIR & 0x8)) {
    		red = green = 0; //resetting LEDs
    		displayMorse(str,strlen(str));
    	}
    	i++;
    }

    return 0 ;
}

/**
 * Delays clock by desired miliseconds. This code is from
 * the lecture videos. I did not write it.
 *
 * @delay_t delay amount in miliseconds
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

/**
 * Takes a string and outputs its morse code representation
 * on LED1.
 *
 * @message string to be converted to morse code
 * @length length of desired string
 */
void displayMorse(char *message, int length) {
	int i = 0;

	while(message[i] != '.') {
		unsigned char curr = message[i];

		if(curr == ' ') {
			switchLED();
		}

		if(curr >= '0' && curr <= '9') {
			handleMorse(intMorseCode[curr - '0'], NUMBER_LENGTH);
		}

		curr = toupper(message[i]);

		if(curr >= 'A' && curr <= 'Z') {
			int index = curr - 'A';
			handleMorse(letterMorseCode[index],LETTER_LENGTH);

		}

		i++;
		if(i > length) {
			printf("went past length!\n");
			break;
		}
	}

	switchLED();
	if(red) {
		GPIOE->PDDR |= (1<<29);
		delay_ms(PERIOD_DELAY);
		GPIOE->PDDR &= ~(1<<29);
	} else {
		GPIOD->PDDR |= (1<<5);
		delay_ms(PERIOD_DELAY);
		GPIOD->PDDR &= ~(1<<5);
	}
}

/**
 * Switches which LED should be turned on next.
 */
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

/**
 * Outputs the morse code for a single character
 *
 * @morse morse code representation of a character in a string
 * @len max length of morse code string
 */
void handleMorse(const char* morse, int len) {
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
