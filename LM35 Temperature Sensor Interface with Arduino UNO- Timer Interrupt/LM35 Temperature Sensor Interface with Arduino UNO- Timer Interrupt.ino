/*************************************************************
A systematic approach to set a timer of 250ms and change the 
behaviour of LED according to the values from LM35 

Author: Ancy Augustin 
**************************************************************/

#define TemperatureSensor (A0)
#define LED (13)
bool LED_STATE = true;
float TempinCelcius=0;
int counter=0;

void setup() {
  pinMode(LED, OUTPUT);                                 //Set the LED to be OUTPUT and Sensor to be INPUT
  pinMode(TemperatureSensor,INPUT);

  cli();                                                // Disable interrupts

  /* TCCR1A register is for controlling PWM mode. Disable PWM mode 
  * on pins 9 and 10 by setting all bits in TCCR1A to 0. This step
  * is to avoid interference with the timer interrupt */

  TCCR1A = 0;                  
  TCCR1B = 0; 

  /* Configure Timer 1 for interrupts with a prescaler of 256.
     TCCR1B register is used to set the prescaler value. The first 
     3 bits (CS10, CS11, CS12(clock select)) define the prescaler 
     value. To set the prescaler to 256, we need to set CS12 bit 
     to 1 and CS11 and CS10 bits to 0 */

  TCCR1B |= (1 << CS12);                                // set the prescaler value to 256

  TIMSK1 |= (1 << OCIE1A);                              // Enable timer interrupt for compare and match mode
  
  /* Calculate and set value to compare register A for desired interrupt frequency
   * System clock: 16 MHz
   * Prescaler: 256
   * Timer 1 frequency: 16 MHz / 256 = 62.5 KHz
   * Pulse time: 1 / 62.5 KHz = 16 us
   * Count up to: 250 ms / 16 us = 15625
  */
  
  OCR1A = 15625;                                      //Set value (250ms) to compare register A  
  sei();                                              //Enable the interrupts

}

void loop() 
{
      //Do Nothing   
}

/* Function to read the Temperature value */
float checkTemperature(void){
  float Temp_AnalogVal=0;
  float Voltage=0,Temp_C=0;

  Temp_AnalogVal = analogRead(TemperatureSensor);	        // Read Sensor Value
  Voltage = (Temp_AnalogVal * (5000.0 / 1024.0));	 // Convert analog value to equivalent voltage
  Temp_C = (Voltage/10);                          // Convert Voltage into Temperature in degree Celsius
  return Temp_C;

}

/*Function to toggle the LED state */
void Toggle_LED(void){
  LED_STATE = !LED_STATE;                       //Invert LED state
  digitalWrite(LED,LED_STATE);                 //Write new state to the LED
}

ISR(TIMER1_COMPA_vect)                          // Enters the ISR for every 250 ms
{
  counter++;                                   // Increments counter to keep track of time intervals
  TCNT1  = 0;                                 //Reset the timer for next interrupt
  TempinCelcius=checkTemperature();          //Obtain the current temperature 
  if(TempinCelcius<25)
  {
  counter=0;                                  //Resets the counter and toggle LED once for 250 ms
  Toggle_LED();
  }

  else if(TempinCelcius>30)
  {                              
    if(counter%2==0)                        //Checks if the count is even to ensure, the time attained 500 ms
    {
      Toggle_LED();                         // Toggle the LED
    }
    else {/*Do nothing*/}                   //LED Toggles in the next iteration
  }
}
