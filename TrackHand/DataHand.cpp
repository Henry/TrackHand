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

// -----------------------------------------------------------------------------

// Construct the keyboard matrix
KeyMatrix keyMatrix;

// Construct the trackball
TrackBall trackBall;

// Construct the power-saving mode for the keyboard and trackball
PowerSave powerSave(keyMatrix, trackBall);

void saveProp(const uint address, const uint8_t val)
{
    if (val != eeprom_read_byte((uint8_t*)address))
    {
        eeprom_write_byte((uint8_t*)address, val);
    }
}


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

            switch (command)
            {
                case 0:
                    if (Serial.available() >= 1)
                    {
                        // Read the value at the address provided
                        uint addr = Serial.read();
                        uint8_t value = eeprom_read_byte((uint8_t*)addr);

                        // Write the value back
                        Serial.write(value);
                    }
                    else
                    {
                        // Discard any buffered input
                        usb_serial_flush_input();
                        Serial.println("TrackHand: getting value failed");
                    }
                    break;

                case 1:
                    if (Serial.available() >= 3)
                    {
                        uint addr = Serial.read();
                        uint8_t value = Serial.read();
                        uint8_t check = Serial.read();

                        // Simple parity check of the data provided
                        if ((addr ^ value) == check)
                        {
                            saveProp(addr, value);
                            Serial.print("TrackHand: setting value at address ");
                            Serial.print(addr);
                            Serial.print(" to ");
                            Serial.print(value);
                            Serial.println(" successful");

                            trackBall.configure();
                        }
                        else
                        {
                            // Discard any buffered input
                            usb_serial_flush_input();
                            Serial.println("TrackHand: setting value failed");
                        }
                    }
                    break;
            }
        }

        keyMatrix.pause();
    }

    return 0;
}


// -----------------------------------------------------------------------------
