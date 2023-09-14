## Traffic Light Simulator

### Description

The traffic light simulator makes use of the FRDM KL46Z Board. Two built-in LEDs are used and one external LED is used. The red LED is LED1 and the green LED is LED2. The external LED is wired to the board via the PTA12 pin. 

Once turned on, the device imitates a standard traffic light, iterating through yellow, green, and red, respectively. Each LED is on for the same amount of time. Once the color cycle ends, it repeats. If SW1 is held down, the color cycle is interrupted. Any LEDs that are on when the switch is first pressed are turned off.  Only the yellow LED blinks on and off. The yellow LED stays on for the same amount of time during the regular cycle and turns off once the switch is released. After the switch is released, the original cycle continues, beginning with green.

### Implementation

The traffic system design makes use of the memory-mapped hardware available on the FRDM KL46Z board. Before the core while-loop of the program begins, port A, C, D, and E are enabled. Then, pin 12 of port A, pin 3 of port C, and pin 5 of port D, and pin 20 of port E are enabled for GPIO. Pull ups are also enabled for SW1. Each LED is set up for output and SW1 is set up for input.

Once all pins and ports have been configured, the main logic of the traffic simulator begins. The system enters a continuous while-loop.

First, the system checks if SW1 is currently being pressed. If it is and the yellow LED is not set to be blinking, it prepares to let the external LED blink. LED1 and LED2 are turned off if they are on. Then, the yellow LED blinks until SW1 is released. If SW1 is not being pressed, the system checks if the yellow LED is in the middle of an independent blinking cycle. If it is, the cycle is terminated. Otherwise, the main color cycle is initiated. 

The main color cycle is relatively simple. It turns on the LEDs in the order of green, yellow, and blue. When a LED is turned on, a boolean variable corresponding to the LED’s color is changed to true. This allows for any necessary LEDs to be turned off correctly if SW1 is pressed.

Both the main color cycle and the independent yellow cycle make use of a count variable. Each time the while loop comes to an end, the count variable is incremented. Changes in LED colors are made when the count variable is a multiple of 900,000. For the main cycle, the green LED is first turned on when count equals zero. When count reaches 900,000, the green LED is turned off and the yellow LED is turned on. At a count of 1,800,000, the yellow LED is turned off and the red LED is turned on. At 2,700,000, the red LED is turned off and the count is reset to zero, which resets the standard color cycle. When the yellow cycle begins, the count variable is reset to zero and the yellow LED is turned on. At a count of 900,000, the yellow LED is turned off. At a count of 1,800,000, the count is reset to zero and the yellow cycle repeats. If the count is not equal to a multiple of 900,000, the count variable is simply incremented and the next iteration of the loop is started. This simulates a busy waiting loop that ensures that all LEDs are illuminated for the same duration of time.

All LEDs are turned on via bit masking that enables the memory-mapped hardware to send an output signal to the designated port and pin that each led is mapped to. The algorithm is somewhat robust, although it could be improved via the use of clocks to ensure that wait time is exactly equal rather than approximately equal. Still, the traffic simulator performs as expected without any issues.

### Results

The traffic simulator performs as expected. A demo of the project can be found [here](https://youtube.com/shorts/mpbwAzr6ciY?feature=share).
