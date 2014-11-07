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
/// Title: TrackBall class using the ADNS9800 laser sensor
///  Description:
//    Move the pointer on interrupt
//    or scroll if the allocated modifier key is pressed.
//    Provides support for low-power sleep mode.
//
//    Both the pointer motion resolution and scroll divider are
//    configurable by sending the new values via USB serial and stored in
//    EEPROM.
// -----------------------------------------------------------------------------

#ifndef TrackBall_H
#define TrackBall_H

#include "WProgram.h"
#include "ADNS9800.h"

// -----------------------------------------------------------------------------

class TrackBall
:
    public ADNS9800
{
    //- Resolution of the pointer motion in units of 50cpi
    uint8_t resolution_ = 10;

    //- Scroll divider reduce the scroll speed relative to the pointer motion.
    uint8_t scrollDivider_ = 50;

    //- Current scroll counter used with scrollDivider_ to reduce scroll speed
    int16_t scrollCount_ = 0;

    //- Structure representing the storage of the parameters in EEPROM
    struct parameters
    {
        uint8_t resolution;
        uint8_t scrollDivider;
    };

    //- Start of the EEPROM storage for the configuration parameters
    ptrdiff_t eepromStart_;


public:

    // Constructor
    TrackBall(const ptrdiff_t eepromStart);

    // Member functions

        //- Setup SPI and ADNS9800 interfaces
        void begin();

        //- Wake after sleep
        void wake();

        //- Configure from parameters stored in EEPROM
        void configure();

        //- Configure parameters stored in EEPROM from Serial
        bool configure(const char cmd);

        //- Return the end of the EEPROM storage
        //  for the configuration parameters
        ptrdiff_t eepromEnd()
        {
            return eepromStart_ + sizeof(parameters);
        }

        //- Change and save the pointer movement resolution
        void resolution(const uint8_t res);

        //- Change and save the scroll divider
        void scrollDivider(const uint8_t sdiv);

        //- If data is present move the pointer (if move = true)
        //  or scroll the screen (if move = false) and return true
        //  otherwise return false
        bool moveOrScroll(const bool moving);
};


// -----------------------------------------------------------------------------
#endif // TrackBall_H
// -----------------------------------------------------------------------------
