#define THROTTLE_SIGNAL_IN 0 // INTERRUPT 0 = DIGITAL PIN 2 - use the interrupt number in attachInterrupt
#define THROTTLE_SIGNAL_IN_PIN 2 // INTERRUPT 0 = DIGITAL PIN 2 - use the PIN number in digitalRead

#define NEUTRAL_THROTTLE 1500 // this is the duration in microseconds of neutral throttle on an electric RC Car
#include <Servo.h>

#define POS_SIGNAL 1300
#define NEG_SIGNAL 1700
Servo motor;

volatile int nThrottleIn = NEUTRAL_THROTTLE; // volatile, we set this in the Interrupt and read it in loop so it must be declared volatile
volatile unsigned long ulStartPeriod = 0; // set in the interrupt
volatile boolean bNewThrottleSignal = false; // set in the interrupt and read in the loop
long countnum = 0; // counter1 for delay
long countnum2 = 0; // counter2 for delay
// we could use nThrottleIn = 0 in loop instead of a separate variable, but using bNewThrottleSignal to indicate we have a new signal 
// is clearer for this first example

void setup()
{
  // tell the Arduino we want the function calcInput to be called whenever INT0 (digital pin 2) changes from HIGH to LOW or LOW to HIGH
  // catching these changes will allow us to calculate how long the input pulse is
  attachInterrupt(THROTTLE_SIGNAL_IN, calcInput, CHANGE);
  // motor.attach(10);
  // motor.writeMicroseconds(POS_SIGNAL);
  Serial.begin(9600); 
}

void loop()
{
  // if a new throttle signal has been measured, lets print the value to serial, if not our code could carry on with some other processing
  if (bNewThrottleSignal)
  {
    Serial.println(nThrottleIn);  
    
    // set this back to false when we have finished
    // with nThrottleIn, while true, calcInput will not update
    // nThrottleIn
    bNewThrottleSignal = false;
  }
  {
    if (countnum > 60000 )
    {
      if (nThrottleIn > 1995 && nThrottleIn < 2200 )
      {
        Serial.println("signal positive");
        Serial.println(countnum);
        Serial.println(countnum2);
        motor.attach(10);
        delay (100);
        countnum = 0 ;
        motor.writeMicroseconds(POS_SIGNAL);
        delay (500);
        motor.detach();
      }
    }
  }
  {
    if (nThrottleIn > 1995 && nThrottleIn < 2200 )
    {
      countnum = countnum +1;
      //delay (1000);
    }
    else
      countnum = 0;
  }
  {
    if (countnum2 > 130000 )
    {
      if (nThrottleIn < 1290 && nThrottleIn > 1240 )
      {
        Serial.println("signal negative");
        Serial.println(countnum);
        Serial.println(countnum2); 
        motor.attach(10);
        delay (100);
        countnum2 = 0 ;
        motor.writeMicroseconds(NEG_SIGNAL);
        delay (500);
        motor.detach();
      }
    }
  }
  {
    if (nThrottleIn < 1290 && nThrottleIn > 1240)
    {
      countnum2 = countnum2 +1;
      //delay (1000);
    }
    else
      countnum2 = 0;
  }
}

void calcInput()
{
  // if the pin is high, its the start of an interrupt
  if (digitalRead(THROTTLE_SIGNAL_IN_PIN) == HIGH)
  { 
    // get the time using micros - when our code gets really busy this will become inaccurate, but for the current application its 
    // easy to understand and works very well
    ulStartPeriod = micros();
  }
  else
  {
    // if the pin is low, its the falling edge of the pulse so now we can calculate the pulse duration by subtracting the 
    // start time ulStartPeriod from the current time returned by micros()
    if (ulStartPeriod && (bNewThrottleSignal == false))
    {
      nThrottleIn = (int)(micros() - ulStartPeriod);
      ulStartPeriod = 0;

      // tell loop we have a new signal on the throttle channel
      // we will not update nThrottleIn until loop sets
      // bNewThrottleSignal back to false
      bNewThrottleSignal = true;
    }
  }
}
