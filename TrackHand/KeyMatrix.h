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

// Add offset to indicate key is a mode or modifier
#define DH_MODE(key) key + KeyMatrix::modeOffset_

// -----------------------------------------------------------------------------

class KeyMatrix
{
    // Private data

        //- Number of matrix columns
        static const uint8_t nColumns_ = 2;

        //- Number of matrix rows
        static const uint8_t nRows_ = 14;

        //- Number of keys in matrix
        static const uint8_t nKeys = 2*nColumns_*nRows_;

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

        //- Key-code offset to indicate key selects mode or modifier
        static const uint8_t modeOffset_ = 0xf0;

        //- Key-code offset to indicate key is shifted
        static const uint8_t shiftOffset_ = 0x80;

        // Mode key codes
        static const KEYCODE_TYPE modeKeyNorm_   = DH_MODE(0x0);
        static const KEYCODE_TYPE modeKeyShift_  = DH_MODE(0x1);
        static const KEYCODE_TYPE modeKeyShiftLk_= DH_MODE(0x2);
        static const KEYCODE_TYPE modeKeyNas_    = DH_MODE(0x3);
        static const KEYCODE_TYPE modeKeyNasLk_  = DH_MODE(0x4);
        static const KEYCODE_TYPE modeKeyFn_     = DH_MODE(0x5);
        static const KEYCODE_TYPE modeKeyMouse_  = DH_MODE(0x6);

        // Modifier key codes
        static const KEYCODE_TYPE modKeyShift_   = DH_MODE(0x7);
        static const KEYCODE_TYPE modKeyCtrl_    = DH_MODE(0x8);
        static const KEYCODE_TYPE modKeyAlt_     = DH_MODE(0x9);

        // Mouse button key codes
        static const KEYCODE_TYPE mouse1_        = DH_MODE(0xa);
        static const KEYCODE_TYPE mouse2_        = DH_MODE(0xb);
        static const KEYCODE_TYPE mouse3_        = DH_MODE(0xc);
        static const KEYCODE_TYPE mouse1_1_      = DH_MODE(0xd);

        // Keyboard programming mode
        static const KEYCODE_TYPE modeKeyPrgm_   = DH_MODE(0xe);

        static const KEYCODE_TYPE normalKeyMap[nKeys];
        static const KEYCODE_TYPE shiftKeyMap[nKeys];
        static const KEYCODE_TYPE nasKeyMap[nKeys];
        static const KEYCODE_TYPE functionKeyMap[nKeys];

        //- Maximum number of pressed keys stored
        static const uint8_t maxPressed_ = 16;

        //- Maximum number of pressed keys to send
        //  Fixed to 6 for USB keyboards
        static const uint8_t maxSend_ = 6;


        //- Interaface to the IO-expander in the left-hand unit
        MCP23018 leftHand_;

        // Keyboard modes
        Mode normalMode_;
        Mode shiftMode_;
        Mode nasMode_;
        Mode fnMode_;
        Mode mouseMode_;

        //- Photo-transistor stabilisation time (us)
        const uint16_t columnStabTime_ = 100;

        //- Matrix scan loop delay time (ms)
        const uint16_t loopDelayTime_ = 10;

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

        //- Add offset to key-code to indicate key is shifted
        static inline KEYCODE_TYPE shiftKeyCode(const KEYCODE_TYPE& key)
        {
            return key + KeyMatrix::shiftOffset_;
        }
};


// -----------------------------------------------------------------------------
#endif // KeyMatrix_H
// -----------------------------------------------------------------------------
