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
// -----------------------------------------------------------------------------

#include "Mode.h"

// -----------------------------------------------------------------------------

Mode::Mode
(
    const bool locked,
    const KEYCODE_TYPE keyCodes[],
    const uint8_t ledPin
)
:
    locked_(locked),
    keyCodes_(keyCodes),
    led_(ledPin)
{}


KEYCODE_TYPE Mode::keyCode(const uint8_t key) const
{
    return keyCodes_[key];
}


void Mode::sleep() const
{
    led_.off();
}

void Mode::wake() const
{
    led_.on();
}


const Mode *Mode::set(const Mode *mode) const
{
    if (mode)
    {
        mode->led_.off();
    }
    led_.on();
    return this;
}


// -----------------------------------------------------------------------------
