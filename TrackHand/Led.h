/// Copyright 2014 Henry G. Weller
// -----------------------------------------------------------------------------
//  This file is part of
/// ---     TrackHand: DataHand with Laser TrackBall
// -----------------------------------------------------------------------------
//
//  TrackHand is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  TrackHand is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//  for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with TrackHand.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
/// Title: Mode indicator LED class
///  Description:
//    Simple indicator LED control class
// -----------------------------------------------------------------------------

#ifndef Led_H
#define Led_H

#include "WProgram.h"

// -----------------------------------------------------------------------------

class Led
{
    const uint8_t pin_;

public:

    //- Construct given the pin number to which the LED is connected
    Led(const uint8_t pin)
    :
        pin_(pin)
    {
        pinMode(pin_, OUTPUT);
        off();
    }

    void on()
    {
        digitalWrite(pin_, HIGH);
    }

    void off()
    {
        digitalWrite(pin_, LOW);
    }
};


// -----------------------------------------------------------------------------
#endif // Led_H
// -----------------------------------------------------------------------------
