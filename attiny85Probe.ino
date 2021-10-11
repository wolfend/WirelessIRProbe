//Power mgmt based on:https://bigdanzblog.wordpress.com/2014/08/10/attiny85-wake-from-sleep-on-pin-state-change-code-example/

#include <avr/sleep.h>
#include <avr/interrupt.h>

const int switchPin                     = 1;
const int IRled                         = 0;
const int minXmtLoops                   = 2000; //2000 x 26usec = 52msec

void setup() {

    pinMode(switchPin, INPUT_PULLUP);
    pinMode(IRled, OUTPUT);
    
//Configuring unused I/O for low power when sleeping    
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    
    } // setup

void sleep() {

    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT1);                   // Use PB1 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement(?DW-what does this comment mean?)

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

//This is the point in the program flow where the CPU waits in low power mode until interrupt occurs. 
//Execution continues from this point when interrupt occurs (when probe touches and switch opens).

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT1);                  // Turn off PB1 as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on
    } // sleep

ISR(PCINT0_vect) {
    // This is called when the interrupt occurs, No code is here so it immediately returns to point in program flow 
    //following the statement in the sleep() function that initially put the CPU to sleep.
    }

void loop() {    
    int i= minXmtLoops; //now awake, we will send at least this many cycles of 38KHz for switch debounce
   
    while ((digitalRead(switchPin)!=0) || (i>0)) //Expecting a normally closed switch
   {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRled, HIGH);  // this takes about 1 microsecond to happen
   delayMicroseconds(12);         // hang out for 12 microseconds
   digitalWrite(IRled, LOW);   // this also takes about 1 microsecond
   delayMicroseconds(12);         // hang out for 12 microseconds
   i--;                       //decrement debounce counter
    }
    
  delay(100); //now that switch has closed (probe no longer touching), 100msec delay for debounce before sleeping
  sleep();
    }// loop
