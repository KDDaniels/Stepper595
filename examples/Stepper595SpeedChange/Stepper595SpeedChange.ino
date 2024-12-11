/*
*   Connect the MOSI pin to the SER pin of your shift register
*   Connect the SCLK pin to the SCK pin on your shift register
*   { QA, QB, QC, QD } connect to { IN1, IN2, IN3, IN4 }
*   
*
*                  74HC595N
*                   __   __
*      IN2 <-  QB =|1 *-*16|= vcc  <- +5v
*      IN3 <-  QC =|2    15|= QA   -> IN1
*      IN4 <-  QD =|3    14|= SER  <- MOSI
*      IN1 <-  QE =|4    13|= OE   -> GND
*      IN2 <-  QF =|5    12|= RCK  <- LATCH
*      IN3 <-  QG =|6    11|= SCK  <- SCK
*      IN4 <-  QH =|7    10|= SCLR <- +5v
*      GND <- GND =|8_____9|= SQH
*
*
*   Stepper595.step() returns a boolean value
*   it can be called normally ( Stepper595.step(0, 0); )
*   or can be used to get an exact number of steps for precise movement
*   ( if (Stepper595.step(0, 0)) steps++; )
* 
*   It is also nonblocking, meaning it will step
*   normally while allowing other tasks to run
*
*
*   This example demonstrates changing the speed conditionally
*/

#include <Stepper595.hpp>

const char LATCH_PIN = 4;
Stepper595 stepper(LATCH_PIN);

int steps = 0;
bool dir = 0;

void setup()
{}

void loop()
{
    if (stepper.step(0, dir) ) 
    { dir ? steps++ : steps--; }

    if ( steps / 100 >= 2 ) { stepper.setDelay(steps / 100); }

    if ( steps > 2000 ) 
    { dir = 1; }
    else if ( steps == 0 )
    { dir = 0; }
}