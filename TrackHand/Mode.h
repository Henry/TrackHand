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
/// Title: Keyboard mode
///  Description:
//    Stores details of a keyboard mode including the key-code, map,
//    the indicator LED and if it is locked i.e. stays in this mode after the
//    corresponding mode-key is released.
// -----------------------------------------------------------------------------

#ifndef Mode_H
#define Mode_H

#include "Led.h"

// -----------------------------------------------------------------------------

class Mode
{
    //- Is the mode locked (sticky)
    bool locked_;

    //- Key-codes for mode
    const KEYCODE_TYPE *keyCodes_;

    //- Indicator led for mode
    mutable Led led_;


public:

    //- Construct given mode type, key map and indicator LED pin
    Mode
    (
        const bool locked,
        const KEYCODE_TYPE keyCodes[],
        const uint8_t ledPin
    );

    //- Return the locked state of this mode
    bool locked() const
    {
        return locked_;
    }

    //- Lock the mode, i.e. change to locked
    void lock()
    {
        locked_ = true;
    }

    //- Unlock the mode
    void unlock()
    {
        locked_ = false;
    }

    //- Return the key-code for the given key
    KEYCODE_TYPE keyCode(const uint8_t key) const;

    //- Sleep to save power and the laser
    void sleep() const;

    //- Wake after sleep
    void wake() const;

    //- Unset mode and set and return this mode
    const Mode *set(const Mode *mode) const;
};


// -----------------------------------------------------------------------------
#endif // Mode_H
// -----------------------------------------------------------------------------
