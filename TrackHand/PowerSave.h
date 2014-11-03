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
/// Title: Power management class
///  Description:
//    Handles idle-time counting, sleep and wake-up of the KeyMatrix and
//    TrackBall.  Wake-up is achieved by setting wakePin_ high using a physical
//    switch.
// -----------------------------------------------------------------------------

#ifndef PowerSave_H
#define PowerSave_H

#include "KeyMatrix.h"
#include "TrackBall.h"
#include <LowPower_Teensy3.h>

// -----------------------------------------------------------------------------

class PowerSave
{
    //- Pin used to wake from power-saving sleep
    const uint8_t wakePin_ = 33;

    //- Pin used to wake from power-saving sleep
    const uint8_t wakeGPIOPin_ = PIN_33;

    //- Static pointer to the keyMatrix needed by the restart() callback
    static KeyMatrix* keyMatrixPtr;

    //- Static pointer to the trackBall needed by the restart() callback
    static TrackBall* trackBallPtr;

    //- Timeout (s)
    const uint16_t timeout_ = 1200;

    //- Iteration time (ms)
    const uint16_t iterTime_ = 30;

    //- Power controller
    TEENSY3_LP powerControl_;

    //- Time counter
    uint32_t idleCount_ = 0;

    static void wake();


public:

    //- Constructor
    PowerSave(KeyMatrix& km, TrackBall& tb);


    // Member functions

        //- Setup pins, I2C and Serial interfaces
        void begin();

        //- Put KeyMatrix and TrackBall to sleep and Teensy 3.1 into deep sleep
        void sleep();

        //- Read and update configuration from USB serial device
        bool readConfiguration()
        {
            return true;
        }

        //- Check if anything has changed and reset sleep counter
        void operator()(const bool changed);
};


// -----------------------------------------------------------------------------
#endif // PowerSave_H
// -----------------------------------------------------------------------------
