### Description

This project revolves around using the FRDM KL46Z board to power two brushless motors via a motor driver, which is connected through a breadboard. The motors each connect to a wheel, which allows the build to act like a miniature car. 

There are two fixed paths that the car can make while it's turned on. If switch one is pressed, the car will move in the pattern of an “S,” in which it will go straight and make turns of 90 degrees. If switch two is pressed, the car will move in the pattern of a curved “S”, in which both wheels are turning at different rates in order to achieve a curved turn. The image below depicts the two paths that the car can take.

This project requires extensive hardware setup, specifically due to the wiring required to connect each part of the car. Figure 2 depicts the circuit that is used to connect the FRDM board to the external battery, both motors, and the motor driver. 

**Figure 2**

![image](https://drive.google.com/uc?export=view&id=1TNC_b-tpvpq20EUevdiAgnontBXzLkFp)


*Figure 2 depicts the circuit used to connect the Battery, FRDM board, Motor driver, and motors.*

### Implementation

The straight S is the simplest portion of the project, though it has the mosts lines of code between the two  fixed paths. My main approach was to break up the algorithm into smaller portions that could be outsourced to their own functions. Each function is responsible for the name which it is named after, and it is not responsible for timing delays, logic checks, et cetera. For instance, the straight() function is responsible for sending signals to PWMA, AI1, PWMB, and AI2, but it is not responsible for checking the current signals being sent or the time for which the car moves in a straight line. The stop() function is responsible for stopping the car by ensuring that no logical 1’s are being sent to the motor driver. Because both wheels need to move at the same pace in order for the car to move straight, both brushed motors have a duty cycle of 100%. Algorithm 1 includes a snippet of the pseudocode used to create the algorithm for the straight ‘S.’ 


```
Algorithm 1: Straight ‘S’

# Port enabling
enable_port_b
enable_port_c

# GPIO enabling
enable_portB_pin2
enable_portB_pin1
enable_portB_pin0

enable_portC_pin1
enable_portC_pin2
enable_portB_pin3

# Straight
straight();
delay(STR_TIME);
stop();
delay(STOP_TIME);

# Left Turn, A
turn_left();
delay(TURN_TIME);
stop();
delay(STOP_TIME);

# Right Turn
turn_right();
delay(TURN_TIME);
stop();
delay(STOP_TIME)
```


The curved ‘S’ is a much simpler algorithm, since it only requires two functions, curve_right() and curve_left(). Each function is responsible for making the car move in a U-like pattern. This is achieved by making one wheel move at 50% of the speed of the other wheel, which results in the car moving in the pattern of a curved ‘U.’ Consequently, depending on the direction of the turn, motor A has a duty cycle of 100% while motor B has a duty cycle of 50%, or vice versa.

Both algorithms rely on self-contained functions, so the algorithms overall are quite robust. The system usually works in the desired way. However, the car moves in a fixed pattern, meaning that if the car isn’t aligned well when it begins, the turns may look to be too wide or too narrow. In order to achieve optimal results, it is best to use the car on a smooth surface. The project was tested on a smooth plastic surface in which the FRDM board was powered by the battery pack in order to ensure free movement of the car.

### Results

Overall, the project behaves as expected. A video depicting the car moving along both paths can be found [here]([https://youtu.be/j1Yx_2fLncw?si=mf8ocq7hosrxdl_a). Figure 3 features the finalized build of the car, including all three frames while Figure 4 features an aerial view of the car, showcasing the wiring used.

**Figure 3**

![image](https://drive.google.com/uc?export=view&id=1X85o4Vub35gmuqqr5fLEFpqr5NO-gopn)

*Figure 3 depicts an aerial view of the car, showcasing the wiring used.*

**Figure 4**

![image](https://drive.google.com/uc?export=view&id=1XBr6RJg3fPQ01vB9-d6iqnAdw3EQ735x)

*Figure 4 depicts the three main frames used to build the car.*


### Discussion

The wiring proved the most difficult part of the project. On my first attempt, I ended up not grounding the system circuit correctly and I also had the motor driver powered from a GPIO pin rather than a pin that could supply the proper voltage. These issues caused significant issues to the board, resulting in the board constantly overheating. After replacing my board and using a multimeter to reaffirm my circuit was grounded correctly, I was able to continue on my project’s development.

Debugging the system when it didn’t produce the desired results also often proved to be difficult. Typically the output of multiple pins would all produce one output. I ended up temporarily connecting the output pins to LEDs so that I could physically see if they were producing output. This method has a slow set up time, since I have to create a separate debug circuit and ensure that it is a valid circuit. The advantage of this method is that, once I start the system, I can physically see right away whether or not output is being created or not, without having to worry about checking different registers. 



