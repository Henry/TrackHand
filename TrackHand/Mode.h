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
//    the indicator LED and if it is modal i.e. stays in this mode after the
//    corresponding mode-key is released.
// -----------------------------------------------------------------------------

#ifndef Mode_H
#define Mode_H

#include "Led.h"

// -----------------------------------------------------------------------------

class Mode
{
    //- Is the mode modal (sticky)
    const bool modal_;

    //- Key-codes for mode
    const KEYCODE_TYPE *keyCodes_;

    //- Indicator led for mode
    mutable Led led_;


public:

    Mode
    (
        const bool modal,
        const KEYCODE_TYPE keyCodes[],
        const uint8_t ledPin
    );

    bool modal() const
    {
        return modal_;
    }

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
