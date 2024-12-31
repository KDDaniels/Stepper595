/*
*    This library is meant to make it easy to control a stepper motor through
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
*/


#include "Stepper595.hpp"

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny24__)
    #include <tinySPI.h>
#else
    #include <SPI.h>
#endif

/**
 * @brief Construct a new Stepper595 object

 * @param CS_PIN {uint8_t} CS/latch pin
 */
Stepper595::Stepper595(uint8_t CS_PIN)
{
    _latch = CS_PIN;

    _currentMillis = millis();
    _delayAmount = 2;

    this -> initialize();
}

/**
 * @brief Destroy the Stepper595 object and end SPI communication
 * 
 */
Stepper595::~Stepper595()
{
    SPI.end();
}


/**
 * @brief Initializes the pins and SPI protocol
 */
void Stepper595::initialize()
{
    SPI.begin();

    _data = 0;
    for (uint8_t i = 0; i < 2; i++)
    {
        _currentStep[i] = 0;
        _targetMillis[i] = millis();
    }

    pinMode(_latch, OUTPUT);
    digitalWrite(_latch, HIGH);
}


/**
 * @brief Sets the delay amount between steps
 * 
 * @param amount {uint8_t} Delay time in milliseconds (max:256)
 */
void Stepper595::setDelay(uint8_t amount)
{
    if (amount > 256) return;
    _delayAmount = amount;
}


/**
 * @brief Steps the selected motor CW or CCW
 * 
 * @param motor  {bool} Which register pins to control; { 0=ABCD, 1=EFGH }
 * @param dir  {bool} Which direction to step; { 0 = CCW, 1 = CW }
 */
bool Stepper595::step(bool motor, bool dir)
{
    if (long(millis()) - long(_targetMillis[motor]) >= 0)
    {
        _data = _pattern[_currentStep[motor]];

        if (motor == MOTOR_2)
        {
            _data = _data << 4;
        }

        digitalWrite(_latch, LOW);
        SPI.transfer(_data);
        digitalWrite(_latch, HIGH);
    
        if (dir == CCW)
        {
            _currentStep[motor]++;
            if (_currentStep[motor] > 3)
            {
                _currentStep[motor] = 0;
            }
        }
        else if (dir == CW)
        {
            if (_currentStep[motor] == 0)
            {
                _currentStep[motor] = 4;
            }
            _currentStep[motor]--;
        }

        _targetMillis[motor] = long(millis()) + long(_delayAmount);
        
        return true;
    }

    return false;
}

/**
 * @brief Sends 0b00000000 to the shift register to disable all control signals for both motors
 */
void Stepper595::stop()
{
    digitalWrite(_latch, LOW);
    SPI.transfer(0b00000000);
    digitalWrite(_latch, HIGH);
}

/**
 * @brief Disables specific control signals for one motor
 * 
 * @param motor {bool} The motor to disable
 */
void Stepper595::stop(bool motor=0)
{
    if (!motor) // MOTOR_1
    {
        digitalWrite(_latch, LOW);
        SPI.transfer(0b0000);
        digitalWrite(_latch, HIGH);
    }
    else // MOTOR_2
    {
        digitalWrite(_latch, LOW);
        SPI.transfer(0b0000 << 4); // I know it's the same as calling stop() but it's clearer this way (I think)
        digitalWrite(_latch, HIGH);
    }
}


