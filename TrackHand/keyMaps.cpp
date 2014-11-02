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
/// Title: Key maps for all modes
///  Description:
// -----------------------------------------------------------------------------

#include "KeyMatrix.h"

// Add offset to indicate key is shifted
inline KEYCODE_TYPE DH_SHIFT(const KEYCODE_TYPE& key)
{
    return KeyMatrix::shiftKeyCode(key);
}


const KEYCODE_TYPE KeyMatrix::normalKeyMap[KeyMatrix::nKeys] =
{
    // Thumb cluster
    KEY_TAB,                  // Knuckle
    KEY_BACKSPACE,            // Nail
    modeKeyNas_,              // Down
    modeKeyNasLk_,            // Down-down
    0,                        // --
    modeKeyFn_,               // Up
    KEY_SPACE,                // Pad

    0,                        // Gap

    // Finger cluster
    KEY_D,                    // 1 W
    KEY_G,                    // 1 N
    KEY_H,                    // 1 D
    KEY_F,                    // 1 E
    KEY_S,                    // 4 D
    KEY_MINUS,                // 4 E
    KEY_M,                    // 1 S
    KEY_W,                    // 2 S
    KEY_SLASH,                // 4 W
    KEY_L,                    // 4 N
    DH_SHIFT(KEY_9),          // 2 W
    KEY_C,                    // 2 N
    KEY_V,                    // 3 S
    KEY_Z,                    // 4 S
    KEY_T,                    // 2 D
    DH_SHIFT(KEY_0),          // 2 E
    KEY_N,                    // 3 D
    KEY_ENTER,                // 3 E
    KEY_B,                    // 3 W
    KEY_R,                    // 3 N


    // Thumb cluster
    modKeyAlt_,               // Knuckle
    modKeyCtrl_,              // Nail
    modeKeyShift_,            // Down
    modeKeyShiftLk_,          // Down-down
    0,                        // --
    modeKeyNorm_,             // Up
    KEY_RETURN,               // Pad

    0,                        // Gap

    // Finger cluster
    KEY_Y,                    // 1 W
    KEY_P,                    // 1 N
    KEY_U,                    // 1 D
    KEY_I,                    // 1 E
    KEY_A,                    // 4 D
    KEY_TILDE,                // 4 E
    KEY_J,                    // 2 S
    KEY_K,                    // 1 S
    KEY_DELETE,               // 4 W
    KEY_QUOTE,                // 4 N
    DH_SHIFT(KEY_1),          // 2 W
    KEY_PERIOD,               // 2 N
    KEY_SEMICOLON,            // 4 S
    KEY_Q,                    // 3 S
    KEY_E,                    // 2 D
    DH_SHIFT(KEY_2),          // 2 E
    KEY_O,                    // 3 D
    KEY_X,                    // 3 E
    KEY_ESC,                  // 3 W
    KEY_COMMA                 // 3 N
};

const KEYCODE_TYPE KeyMatrix::shiftKeyMap[KeyMatrix::nKeys] =
{
    // Thumb cluster
    DH_SHIFT(KEY_TAB),        // Knuckle
    DH_SHIFT(KEY_BACKSPACE),  // Nail
    modeKeyNas_,              // Down
    modeKeyNasLk_,            // Down-down
    0,                        // --
    modeKeyFn_,               // Up
    DH_SHIFT(KEY_SPACE),      // Pad

    0,                        // Gap

    // Finger cluster
    DH_SHIFT(KEY_D),          // 1 W
    DH_SHIFT(KEY_G),          // 1 N
    DH_SHIFT(KEY_H),          // 1 D
    DH_SHIFT(KEY_F),          // 1 E
    DH_SHIFT(KEY_S),          // 4 D
    DH_SHIFT(KEY_MINUS),      // 4 E
    DH_SHIFT(KEY_M),          // 1 S
    DH_SHIFT(KEY_W),          // 2 S
    DH_SHIFT(KEY_SLASH),      // 4 W
    DH_SHIFT(KEY_L),          // 4 N
    DH_SHIFT(KEY_LEFT_BRACE), // 2 W
    DH_SHIFT(KEY_C),          // 2 N
    DH_SHIFT(KEY_V),          // 3 S
    DH_SHIFT(KEY_Z),          // 4 S
    DH_SHIFT(KEY_T),          // 2 D
    DH_SHIFT(KEY_RIGHT_BRACE),// 2 E
    DH_SHIFT(KEY_N),          // 3 D
    DH_SHIFT(KEY_ENTER),      // 3 E
    DH_SHIFT(KEY_B),          // 3 W
    DH_SHIFT(KEY_R),          // 3 N

    // Thumb cluster
    modKeyAlt_,               // Knuckle
    modKeyCtrl_,              // Nail
    modeKeyShift_,            // Down
    modeKeyShiftLk_,          // Down-down
    0,                        // --
    modeKeyNorm_,             // Up
    DH_SHIFT(KEY_RETURN),     // Pad

    0,                        // Gap

    // Finger cluster
    DH_SHIFT(KEY_Y),          // 1 W
    DH_SHIFT(KEY_P),          // 1 N
    DH_SHIFT(KEY_U),          // 1 D
    DH_SHIFT(KEY_I),          // 1 E
    DH_SHIFT(KEY_A),          // 4 D
    DH_SHIFT(KEY_TILDE),      // 4 E
    DH_SHIFT(KEY_J),          // 2 S
    DH_SHIFT(KEY_K),          // 1 S
    KEY_DELETE,               // 4 W
    DH_SHIFT(KEY_QUOTE),      // 4 N
    DH_SHIFT(KEY_1),          // 2 W
    DH_SHIFT(KEY_PERIOD),     // 2 N
    DH_SHIFT(KEY_SEMICOLON),  // 4 S
    DH_SHIFT(KEY_Q),          // 3 S
    DH_SHIFT(KEY_E),          // 2 D
    DH_SHIFT(KEY_2),          // 2 E
    DH_SHIFT(KEY_O),          // 3 D
    DH_SHIFT(KEY_X),          // 3 E
    KEY_ESC,                  // 3 W
    DH_SHIFT(KEY_COMMA)       // 3 N
};

const KEYCODE_TYPE KeyMatrix::nasKeyMap[KeyMatrix::nKeys] =
{
    // Thumb cluster
    KEY_TAB,                  // Knuckle
    KEY_BACKSPACE,            // Nail
    modeKeyNas_,              // Down
    modeKeyNasLk_,            // Down-down
    0,                        // --
    modeKeyFn_,               // Up
    KEY_SPACE,                // Pad

    0,                        // Gap

    // Finger cluster
    KEY_6,                    // 1 W
    DH_SHIFT(KEY_6),          // 1 N
    KEY_7,                    // 1 D
    0,                        // 1 E
    KEY_0,                    // 4 D
    DH_SHIFT(KEY_BACKSLASH),  // 4 E
    KEY_MINUS,                // 1 S
    DH_SHIFT(KEY_8),          // 2 S
    KEY_BACKSLASH,            // 4 W
    KEY_LEFT_BRACE,           // 4 N
    DH_SHIFT(KEY_9),          // 2 W
    DH_SHIFT(KEY_7),          // 2 N
    DH_SHIFT(KEY_RIGHT_BRACE),// 3 S
    KEY_RIGHT_BRACE,          // 4 S
    KEY_8,                    // 2 D
    DH_SHIFT(KEY_0),          // 2 E
    KEY_9,                    // 3 D
    KEY_ENTER,                // 3 E
    KEY_SLASH,                // 3 W
    DH_SHIFT(KEY_LEFT_BRACE), // 3 N


    // Thumb cluster
    modKeyAlt_,               // Knuckle
    modKeyCtrl_,              // Nail
    modeKeyShift_,            // Down
    modeKeyShiftLk_,          // Down-down
    0,                        // --
    modeKeyNorm_,             // Up
    KEY_RETURN,               // Pad

    0,                        // Gap

    // Finger cluster
    DH_SHIFT(KEY_5),          // 1 W
    DH_SHIFT(KEY_4),          // 1 N
    KEY_4,                    // 1 D
    KEY_5,                    // 1 E
    KEY_1,                    // 4 D
    DH_SHIFT(KEY_TILDE),      // 4 E
    KEY_EQUAL,                // 2 S
    DH_SHIFT(KEY_EQUAL),      // 1 S
    KEY_DELETE,               // 4 W
    DH_SHIFT(KEY_QUOTE),      // 4 N
    DH_SHIFT(KEY_1),          // 2 W
    DH_SHIFT(KEY_PERIOD),     // 2 N
    KEY_COMMA,                // 4 S
    DH_SHIFT(KEY_3),          // 3 S
    KEY_3,                    // 2 D
    KEY_PERIOD,               // 2 E
    KEY_2,                    // 3 D
    KEY_QUOTE,                // 3 E
    KEY_ESC,                  // 3 W
    DH_SHIFT(KEY_COMMA)       // 3 N
};

const KEYCODE_TYPE KeyMatrix::functionKeyMap[KeyMatrix::nKeys] =
{
    // Thumb cluster
    KEY_TAB,                  // Knuckle
    mouse3_,                  // Nail
    mouse1_,                  // Down
    mouse1_1_,                // Down-down
    0,                        // --
    modeKeyNorm_,             // Up
    mouse2_,                  // Pad

    0,                        // Gap

    // Finger cluster
    KEY_LEFT,                 // 1 W
    KEY_UP,                   // 1 N
    KEY_F5,                   // 1 D
    KEY_RIGHT,                // 1 E
    KEY_F8,                   // 4 D
    0,                        // 4 E
    KEY_DOWN,                 // 1 S
    KEY_PAGE_DOWN,            // 2 S
    0,                        // 4 W
    modeKeyPrgm_,             // 4 N
    0,                        // 2 W
    KEY_PAGE_UP,              // 2 N
    KEY_PAUSE,                // 3 S
    0,                        // 4 S
    KEY_F6,                   // 2 D
    0,                        // 2 E
    KEY_F7,                   // 3 D
    KEY_ENTER,                // 3 E
    KEY_INSERT,               // 3 W
    KEY_PRINTSCREEN,          // 3 N

    // Thumb cluster
    modKeyAlt_,               // Knuckle
    modKeyCtrl_,              // Nail
    modKeyShift_,             // Down
    modeKeyShift_,            // Down-down
    0,                        // --
    modeKeyNorm_,             // Up
    KEY_RETURN,               // Pad

    0,                        // Gap

    // Finger cluster
    KEY_LEFT,                 // 1 W
    KEY_UP,                   // 1 N
    KEY_F4,                   // 1 D
    KEY_RIGHT,                // 1 E
    KEY_F1,                   // 4 D
    KEY_SCROLL_LOCK,          // 4 E
    KEY_END,                  // 2 S
    KEY_DOWN,                 // 1 S
    KEY_DELETE,               // 4 W
    KEY_F5,                   // 4 N
    DH_SHIFT(KEY_1),          // 2 W
    KEY_HOME,                 // 2 N
    KEY_F7,                   // 4 S
    KEY_F8,                   // 3 S
    KEY_F3,                   // 2 D
    KEY_ENTER,                // 2 E
    KEY_F2,                   // 3 D
    KEY_NUM_LOCK,             // 3 E
    KEY_ESC,                  // 3 W
    KEY_F6                    // 3 N
};


// -----------------------------------------------------------------------------
