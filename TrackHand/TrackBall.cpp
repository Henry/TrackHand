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

#include "TrackBall.h"
#include <spi4teensy3.h>
#include "EEPROMParameters.h"
#include "initialize.h"
#include "debug.h"

// -----------------------------------------------------------------------------

void TrackBall::configure()
{
    if (initialize)
    {
        eepromSet(resolution, resolution_);
        eepromSet(scrollDivider, scrollDivider_);
    }
    else
    {
        // Update the movement resolution and scroll-divider
        // from the values stored in EEPROM
        resolution_ = eepromGet(resolution);
        scrollDivider_ = eepromGet(scrollDivider);
    }

    setResolution(resolution_);
}


bool TrackBall::configure(const char cmd)
{
    switch (cmd)
    {
        case 'r':
            eepromSetFromSerial(cmd, resolution);
            resolution_ = eepromGet(resolution);
            setResolution(resolution_);
            return true;
            break;
        case 's':
            eepromSetFromSerial(cmd, scrollDivider);
            scrollDivider_ = eepromGet(scrollDivider);
            return true;
            break;
        case 'p':
            Serial.print("TrackBall resolution ");
            Serial.println(eepromGet(resolution));
            Serial.print("TrackBall scrollDivider ");
            Serial.println(scrollDivider_);
            return true;
            break;
    }

    return false;
}


void TrackBall::resolution(const uint8_t res)
{
    resolution_ = res;
    eepromSet(resolution, resolution_);
    setResolution(resolution_);
}


void TrackBall::scrollDivider(const uint8_t sdiv)
{
    scrollDivider_ = sdiv;
    eepromSet(scrollDivider, scrollDivider_);
}


TrackBall::TrackBall(const ptrdiff_t eepromStart)
:
    eepromStart_(eepromStart)
{}


void TrackBall::begin()
{
    ADNS9800::begin();
    configure();
}


void TrackBall::wake()
{
    ADNS9800::wake();
    configure();
}


// Clip a 16bit integer to an 8bit integer
inline int8_t clip8(int16_t y)
{
    if (y > INT8_MAX) return INT8_MAX;
    if (y < INT8_MIN) return INT8_MIN;
    return y;
}


bool TrackBall::moveOrScroll(const bool moving)
{
    if (moved_)
    {
        // Reset the interrupt flag
        moved_ = false;

        // Read the ball motion from the ADNS-9800
        int16_t xy[2];
        adnsBurstMotion(xy);

        if (moving)
        {
            // Clip the movement to -128 to 127 per call
            // It would be possible to accumulate the overflow
            // and apply subsequently but limiting the speed by clipping as
            // is done here might be better anyway.
            Mouse.move(clip8(-xy[1]), clip8(-xy[0]));
        }
        else
        {
            // Reset scroll counter if direction changes
            if
            (
                (xy[0] > 0 && scrollCount_ > 0)
             || (xy[0] < 0 && scrollCount_ < 0)
            )
            {
                scrollCount_ = 0;
            }

            // Accumulate the scroll motion
            scrollCount_ -= xy[0];

            // Divide and clip the scroll motion before sending
            Mouse.scroll(clip8(scrollCount_/scrollDivider_));

            // Reduce the scroll count according to that sent
            // ignoring the clipping to limit scroll-speed
            scrollCount_ %= scrollDivider_;
        }

        return true;
    }

    return false;
}


// -----------------------------------------------------------------------------
