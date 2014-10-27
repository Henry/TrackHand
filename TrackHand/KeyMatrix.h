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
/// Title: Main key-matrix scanning and mapping class
///  Description:
//    Scans the key-matrix directly on the right-hand unit and indirectly via
//    an IO-expander on the left-hand unit and maps the key-presses to key-codes
//    and sends to the host via USB.
// -----------------------------------------------------------------------------

#ifndef KeyMatrix_H
#define KeyMatrix_H

#include "Mode.h"
#include "MCP23018.h"

// -----------------------------------------------------------------------------

class KeyMatrix
{
    // Private data

        //- Number of matrix columns
        static const uint8_t nColumns_ = 2;

        //- Number of matrix rows
        static const uint8_t nRows_ = 14;

        //- Right-hand matrix column pins
        const uint8_t rhColumns_[nColumns_] =
        {
            14, 16
        };

        //- Right-hand matrix row pins
        const uint8_t rhRows_[nRows_] =
        {
            5, 6, 8, 7,                        // Thumb
            1, 0, 15, 3, 17, 2, 20, 23, 21, 22 // Fingers
        };

        //- Left-hand matrix column bits
        const uint8_t lhColumns_[nColumns_] =
        {
            0, 1
        };

        //- Left-hand matrix row bits
        const uint8_t lhRows_[nRows_] =
        {
            2, 3, 4, 5,                        // Thumb
            10, 8, 7, 12, 6, 15, 9, 14, 13, 11 // Fingers
        };

        //- Photo-transistor stabilisation time (us)
        const uint16_t columnStabTime_ = 100;

        //- Matrix scan loop delay time (ms)
        const uint16_t loopDelayTime_ = 10;

        //- Maximum number of pressed keys stored
        static const uint8_t maxPressed_ = 16;

        //- Maximum number of pressed keys to send
        //  Fixed to 6 for USB keyboards
        static const uint8_t maxSend_ = 6;

        //- Interaface to the IO-expander in the left-hand unit
        MCP23018 leftHand_;

        Mode normalMode_;
        Mode shiftMode_;
        Mode nasMode_;
        Mode fnMode_;
        Mode mouseMode_;

        //- Current mode
        const Mode *currentMode_;

        //- Number of keys pressed
        uint8_t nPressed_;

        //- Pressed keys
        uint8_t pressedKeys_[maxPressed_];

        uint8_t modeKeyPrev_ = 0;

        //- Key codes sent from previous call
        //  Used to avoid sending the same key codes repeatedly
        KEYCODE_TYPE keyboardKeysPrev_[maxSend_] = {0, 0, 0, 0, 0, 0};

        //- Modifiers sent from previous call
        //  Used to avoid sending the same key codes repeatedly
        uint8_t modifiersPrev_ = 0;

        //- Mouse buttons from previous call
        uint8_t mouseButtonsPrev_[3] = {0, 0, 0};

        //- Set the current mode
        void set(const Mode& mode);

        //- Return the right-hand key index for the given row and column
        inline uint8_t rhKey(const uint8_t row, const uint8_t col)
        {
            return row*nColumns_ + col;
        }

        //- Return the left-hand key index for the given row and column
        inline uint8_t lhKey(const uint8_t row, const uint8_t col)
        {
            return nRows_*nColumns_ + row*nColumns_ + col;
        }

        inline uint8_t modeKey(const uint8_t keyCode)
        {
            return keyCode >= modeOffset_;
        }

        inline uint8_t shiftedKey(const uint8_t keyCode)
        {
            return keyCode >= shiftOffset_;
        }

        void debugKey(const char* hand, const uint8_t row, const uint8_t col);

        //- Scan the matrix
        void scan();

        //- Send the pressed keys
        bool send();


public:

    static const uint8_t modeOffset_ = 0xf0;
    static const uint8_t shiftOffset_ = 0x80;

    //- Number of keys in matrix
    static const uint8_t nKeys = 2*nColumns_*nRows_;


    //- Constructor
    KeyMatrix();


    // Member functions

        //- Setup pins, I2C and Serial interfaces
        void begin();

        //- Sleep to save power and the leds
        void sleep();

        //- Wake after sleep
        void wake();

        //- Scan matrix and send the pressed keys
        bool keysPressed();

        //- Loop pause to avoid overloading the USB HID interface
        void pause();
};


// -----------------------------------------------------------------------------
#endif // KeyMatrix_H
// -----------------------------------------------------------------------------
