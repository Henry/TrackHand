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

// | Switch | Thumb   | Column/Row/index |
// |--------+---------+------------------|
// |      1 | Knuckle | 0 0 0            |
// |      2 | Nail    | 1 0 1            |
// |      3 | Down    | 0 1 2            |
// |      4 | DOWN    | 1 1 3            |
// |      5 | Pad     | 0 3 6            |
// |      6 | Up      | 1 2 5            |

// | Finger | D       | N       | S       | E       | W       |
// |--------+---------+---------+---------+---------+---------|
// |      1 | 0 5  10 | 1 4  9  | 0 7  14 | 1 5  11 | 0 4  8  |
// |      2 | 0 11 22 | 1 9  19 | 1 7  15 | 1 11 23 | 0 9  18 |
// |      3 | 0 12 24 | 1 13 27 | 0 10 20 | 1 12 25 | 0 13 26 |
// |      4 | 0 6  12 | 1 8  17 | 1 10 21 | 1 6  13 | 0 8  16 |

// Mode and modifier key codes
const uint16_t DH_KEY_NORM  = 0xf0;
const uint16_t DH_KEY_NAS   = 0xf1;
const uint16_t DH_KEY_NASLK = 0xf2;
const uint16_t DH_KEY_FN    = 0xf3;
const uint16_t DH_KEY_SHIFT = 0xf4;
const uint16_t DH_KEY_MOUSE = 0xf5;
const uint16_t DH_KEY_CTRL  = 0xf6;
const uint16_t DH_KEY_ALT   = 0xf7;

// ***HGW need to add a SHIFT modifier key in addition to the mode

// Mouse button key codes
const uint16_t DH_MOUSE_1   = 0xf8;
const uint16_t DH_MOUSE_2   = 0xf9;
const uint16_t DH_MOUSE_3   = 0xfa;
const uint16_t DH_MOUSE_1_1 = 0xfb;

// Add bit to indicate key is shifted
#define DH_SHIFT(key) key + 0x80

const KEYCODE_TYPE normalKeyMap[KeyMatrix::nKeys] =
{
    // Thumb cluster
    DH_KEY_ALT,               // Knuckle
    KEY_BACKSPACE,            // Nail
    DH_KEY_SHIFT,             // Down
    DH_KEY_SHIFT,             // Down-down
    0,                        // --
    DH_KEY_MOUSE,             // Up
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
    DH_KEY_ALT,               // Knuckle
    KEY_BACKSPACE,            // Nail
    DH_KEY_SHIFT,             // Down
    DH_KEY_SHIFT,             // Down-down
    0,                        // --
    DH_KEY_MOUSE,             // Up
    KEY_SPACE,                // Pad

    0,                        // Gap

    // Finger cluster
    KEY_D,                    // 1 W
    KEY_G,                    // 1 N
    KEY_H,                    // 1 D
    KEY_F,                    // 1 E
    KEY_S,                    // 4 D
    KEY_MINUS,                // 4 E
    KEY_W,                    // 2 S
    KEY_M,                    // 1 S
    KEY_SLASH,                // 4 W
    KEY_L,                    // 4 N
    DH_SHIFT(KEY_9),          // 2 W
    KEY_C,                    // 2 N
    KEY_Z,                    // 4 S
    KEY_V,                    // 3 S
    KEY_T,                    // 2 D
    DH_SHIFT(KEY_0),          // 2 E
    KEY_N,                    // 3 D
    KEY_ENTER,                // 3 E
    KEY_B,                    // 3 W
    KEY_R                     // 3 N
};

const KEYCODE_TYPE shiftKeyMap[KeyMatrix::nKeys] =
{
    // Thumb cluster
    DH_KEY_ALT,               // Knuckle
    DH_SHIFT(KEY_BACKSPACE),  // Nail
    DH_KEY_SHIFT,             // Down
    DH_KEY_SHIFT,             // Down-down
    0,                        // --
    DH_KEY_MOUSE,             // Up
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
    DH_SHIFT(KEY_R),           // 3 N

    // Thumb cluster
    DH_KEY_ALT,               // Knuckle
    DH_SHIFT(KEY_BACKSPACE),  // Nail
    DH_KEY_SHIFT,             // Down
    DH_KEY_SHIFT,             // Down-down
    0,                        // --
    DH_KEY_MOUSE,             // Up
    DH_SHIFT(KEY_SPACE),      // Pad

    0,                        // Gap

    // Finger cluster
    DH_SHIFT(KEY_D),          // 1 W
    DH_SHIFT(KEY_G),          // 1 N
    DH_SHIFT(KEY_H),          // 1 D
    DH_SHIFT(KEY_F),          // 1 E
    DH_SHIFT(KEY_S),          // 4 D
    DH_SHIFT(KEY_MINUS),      // 4 E
    DH_SHIFT(KEY_W),          // 2 S
    DH_SHIFT(KEY_M),          // 1 S
    DH_SHIFT(KEY_SLASH),      // 4 W
    DH_SHIFT(KEY_L),          // 4 N
    DH_SHIFT(KEY_LEFT_BRACE), // 2 W
    DH_SHIFT(KEY_C),          // 2 N
    DH_SHIFT(KEY_Z),          // 4 S
    DH_SHIFT(KEY_V),          // 3 S
    DH_SHIFT(KEY_T),          // 2 D
    DH_SHIFT(KEY_RIGHT_BRACE),// 2 E
    DH_SHIFT(KEY_N),          // 3 D
    DH_SHIFT(KEY_ENTER),      // 3 E
    DH_SHIFT(KEY_B),          // 3 W
    DH_SHIFT(KEY_R)           // 3 N
};

const KEYCODE_TYPE mouseKeyMap[KeyMatrix::nKeys] =
{
    // Thumb cluster
    DH_KEY_ALT,               // Knuckle
    DH_MOUSE_3,               // Nail
    DH_MOUSE_1,               // Down
    DH_MOUSE_1_1,             // Down-down
    0,                        // --
    DH_KEY_NORM,              // Up
    DH_MOUSE_2,               // Pad

    0,                        // Gap

    // Finger cluster
    KEY_LEFT,                 // 1 W
    KEY_UP,                   // 1 N
    KEY_ENTER,                // 1 D
    KEY_RIGHT,                // 1 E
    DH_SHIFT(KEY_S),          // 4 D
    DH_SHIFT(KEY_MINUS),      // 4 E
    KEY_DOWN,                 // 1 S
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
    DH_SHIFT(KEY_R),           // 3 N

    // Thumb cluster
    DH_KEY_ALT,               // Knuckle
    DH_MOUSE_3,               // Nail
    DH_MOUSE_1,               // Down
    DH_MOUSE_1_1,             // Down-down
    0,                        // --
    DH_KEY_NORM,              // Up
    DH_MOUSE_2,               // Pad

    0,                        // Gap

    // Finger cluster
    KEY_LEFT,                 // 1 W
    KEY_UP,                   // 1 N
    KEY_ENTER,                // 1 D
    KEY_RIGHT,                // 1 E
    DH_SHIFT(KEY_S),          // 4 D
    DH_SHIFT(KEY_MINUS),      // 4 E
    DH_SHIFT(KEY_W),          // 2 S
    KEY_DOWN,                 // 1 S
    DH_SHIFT(KEY_SLASH),      // 4 W
    DH_SHIFT(KEY_L),          // 4 N
    DH_SHIFT(KEY_LEFT_BRACE), // 2 W
    DH_SHIFT(KEY_C),          // 2 N
    DH_SHIFT(KEY_Z),          // 4 S
    DH_SHIFT(KEY_V),          // 3 S
    DH_SHIFT(KEY_T),          // 2 D
    DH_SHIFT(KEY_RIGHT_BRACE),// 2 E
    DH_SHIFT(KEY_N),          // 3 D
    DH_SHIFT(KEY_ENTER),      // 3 E
    DH_SHIFT(KEY_B),          // 3 W
    DH_SHIFT(KEY_R)           // 3 N
};


// -----------------------------------------------------------------------------
