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
/// Title: Main function
///  Description:
//    Constructs the KeyMatrix, TrackBall and PowerSave classes
//    and enters the matrix scan loop.
//    The trackball operates pointer movement on interrupt.
//    The trackball may also be used for scrolling selected by appropriate key.
// -----------------------------------------------------------------------------

#include "KeyMatrix.h"
#include "TrackBall.h"
#include "PowerSave.h"
#include "MCP23018.h"
#include "EEPROMParameters.h"

// -----------------------------------------------------------------------------

// Construct the keyboard matrix
// with the parameters stored in EEPROM starting at 0
KeyMatrix keyMatrix(0);

// Construct the trackball
// with the parameters stored in EEPROM starting immediately after the keyMatrix
TrackBall trackBall(keyMatrix.eepromEnd());

// Construct the power-saving mode for the keyboard and trackball
// with the parameters stored in EEPROM starting immediately after the trackBall
PowerSave powerSave(keyMatrix, trackBall, trackBall.eepromEnd());

int main(void)
{
    keyMatrix.begin();
    trackBall.begin();
    powerSave.begin();

    while (1)
    {
        // Put into power-save mode when keys are not pressed
        // or trackball not moved for some time
        powerSave
        (
            // Check for key presses
            keyMatrix.keysPressed()

            // Check for pointer motion and move or scroll
         || trackBall.moveOrScroll(!keyMatrix.scroll())
        );

        if (Serial.available())
        {
            uint8_t command = Serial.read();

            bool handled = false;

            handled |= keyMatrix.configure(command);
            handled |= trackBall.configure(command);
            handled |= powerSave.configure(command);

            if (!handled)
            {
                // Discard any buffered input
                usb_serial_flush_input();
                Serial.print("TrackHand: configuration command ");
                Serial.print(command);
                Serial.println(" failed");
            }
        }

        keyMatrix.pause();
    }

    return 0;
}


// -----------------------------------------------------------------------------
