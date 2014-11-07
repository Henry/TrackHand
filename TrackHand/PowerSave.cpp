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

#include "PowerSave.h"
#include "EEPROMParameters.h"
#include "initialize.h"
#include "debug.h"

// -----------------------------------------------------------------------------

KeyMatrix* PowerSave::keyMatrixPtr = NULL;
TrackBall* PowerSave::trackBallPtr = NULL;

void PowerSave::wake()
{
    if (trackBallPtr)
    {
        trackBallPtr->wake();
    }

    if (keyMatrixPtr)
    {
        keyMatrixPtr->wake();
    }
}


PowerSave::PowerSave(KeyMatrix& km, TrackBall& tb, const ptrdiff_t eepromStart)
:
    eepromStart_(eepromStart)
{
    keyMatrixPtr = &km;
    trackBallPtr = &tb;
}


void PowerSave::begin()
{
    pinMode(wakePin_, INPUT_PULLUP);
    configure();
}


void PowerSave::sleep()
{
    idleCount_ = 0;

    if (trackBallPtr)
    {
        trackBallPtr->sleep();
    }

    if (keyMatrixPtr)
    {
        keyMatrixPtr->sleep();
    }

    powerControl_.DeepSleep(GPIO_WAKE, wakeGPIOPin_, wake);
}


void PowerSave::configure()
{
    if (initialize)
    {
        eepromSet(timeout, timeout_);
    }
    else
    {
        timeout_ = eepromGet(timeout);
    }
}


bool PowerSave::configure(const char cmd)
{
    switch (cmd)
    {
        case 't':
            eepromSetFromSerial(cmd, timeout);
            timeout_ = eepromGet(timeout);
            return true;
            break;
        case 'p':
            Serial.print("PowerSave timeout ");
            Serial.println(timeout_);
            return true;
            break;
    }

    return false;
}


void PowerSave::operator()(const bool changed)
{
    if (changed)
    {
        idleCount_ = 0;
    }
    else
    {
        idleCount_++;
    }

    if (idleCount_ > timeout_*1000/iterTime_)
    {
        sleep();
    }
}


// -----------------------------------------------------------------------------
