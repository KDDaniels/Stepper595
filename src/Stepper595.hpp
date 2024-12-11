/*
*    This library is meant to make it easy to control one or two stepper motors through
*    a shift register using SPI. Built for the 28BYJ-48 stepper with the ULN2003
*    driver board
*
*    Copyright (C) 2024 Kendall Daniels
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
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
*/

#ifndef STEPPER595_HPP
#define STEPPER595_HPP

#include <Arduino.h>

/**
 * @class Stepper595
 * @brief This class controls up to two stepper motors through a 74HC595 shift register by utilizing SPI.
 * Provides interface to easily control stepper movements with minimal wires
 */
class Stepper595
{

public:
    /**
     * @brief Construct a new Stepper595 object
     * 
     * @param CS_PIN {int} CS/latch pin; can be anything
     */
    Stepper595(unsigned char CS_PIN);

    /**
     * @brief Destroy the Stepper 595 object and end SPI communication
     * 
     */
    ~Stepper595();


    /**
     * @brief Steps the selected motor CW or CCW with a delay of 10ms default (change with setDelay)
     * 
     * @param motor {bool} Which register pins to control; { 0=ABCD, 1=EFGH }
     * @param dir {bool} Which direction to step; { 0 = CCW, 1 = CW }
     */
    bool step(bool motor, bool dir);


    /**
     * @brief Set the delay amount between steps.
     * Default val: 10ms
     * 
     * @param amount {int} ms between steps
     */
    void setDelay(uint8_t amount);


private:

    void initialize();

    unsigned char _latch;
    unsigned char _dataPin;
    unsigned char _clkPin;

    unsigned long _currentMillis;
    unsigned long _targetMillis[2];
    unsigned char _delayAmount;

    char _currentStep[2];

    unsigned char _data;
    unsigned char _pattern[4] = {0b1001,
                                 0b1100,
                                 0b0110,
                                 0b0011};
};

#endif