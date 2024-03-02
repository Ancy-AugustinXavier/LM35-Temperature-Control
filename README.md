# LM35 Temperature Control with Arduino

This Arduino sketch interfaces an LM35 temperature sensor with an Arduino Uno to control the onboard LED based on temperature readings. 

- **Sensor Pin:** A0
- **LED Pin:** 13

## Setup
- The code sets up the LED pin as an output and the sensor pin as an input.
- Timer 1 is configured with a prescaler of 256 to generate interrupt for every 250 milliseconds.
- The interrupt service routine (ISR) toggles the LED state based on temperature readings.

## Timer Interrupt 
- To control the timer1 we have two main registers, the TCCR1A and the TCCR1B.
- TCCR1A register is for controlling PWM mode. Disable PWM mode on pins 9 and 10 by setting all bits in TCCR1A to 0. This step is necessary to ensure that timer 1 is not controlling PWM signals on pins 9 and 10 to avoid interference with the timer interrupt

- Configure Timer 1 for interrupts with a prescaler of 256. TCCR1B register is used to set the prescaler value. The first 3 bits (CS10, CS11, CS12) define the prescaler value
- To set the prescaler to 256, we need to set CS12 bit to 1 and CS11 and CS10 bits to 0
- This configuration sets the prescaler value to 256
  TCCR1B |= (1 << CS12);

- Enable timer interrupt for compare and match mode in the Timer Mask Register 1 using
  TIMSK1 |= (1 << OCIE1A);

- Calculate and set value to compare register A for desired interrupt frequency
- System clock: 16 MHz
- Prescaler: 256
- Timer 1 frequency: 16 MHz / 256 = 62.5 KHz
- Pulse time: 1 / 62.5 KHz = 16 us
- Count up to: 250 ms / 16 us = 15625
- Set the value to the Output Compare Register OCR1A = 15625;

## Temperature Monitoring
- The LM35 temperature sensor reads the analog input and converts it to temperature in Celsius.
- If the temperature falls below 25°C, the LED blinks every 250 milliseconds.
- If the temperature rises above 30°C, the LED blinks every 500 milliseconds.

## Interrupt Service Routine (ISR)
- The ISR updates the temperature reading and toggles the LED state accordingly for 250 ms and 500 ms.
- Upon each interrupt, the ISR increments a counter (`count`) to keep track of time intervals.
- The timer is reset (`TCNT1 = 0`) to prepare for the next interrupt.
- The current temperature is obtained using the `checkTemperature()` function.
- If the temperature is below 25°C, the LED blinks for every 250 ms, indicating a low temperature condition.
- If the temperature is above 30°C, checks if the count is even, ensuring it blinks at the next iteration thereby blinking for every 500ms.

## Procedure
- Connect the LM35 temperature sensor to pin A0 and the LED to pin 13 of the Arduino Uno.
- Upload the sketch to your Arduino board.
- Monitor LED blinking behavior.

## Tinkercad Simulation
- A Tinkercad simulation screenshot is attached below to visualize the setup and behavior of the project in a virtual environment.
- The simulation demonstrates the interaction between the temperature sensor, Arduino Uno, and the LED based on temperature readings.
- Check out the Tinkercad simulation [here](https://www.tinkercad.com/things/hO9t8GFOT5o-lm35-interface-with-arduino-uno-timer-interrupt?sharecode=vkIi7biMhjhfrOfD4OhYMYCNY2j85iH37Vi5Fm8xEUg).

![Tinkercad Simulation]()

