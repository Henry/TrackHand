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
/// Title: Interface to the MCP23018 IO-expander
///  Description:
//    Incomplete but sufficient interface to the MCP23018 IO-expander
//    using the i2c_t3
//      http://forum.pjrc.com/threads/21680-New-I2C-library-for-Teensy3
//    rather than the Wire I2C comms library as the later
//    proved unreliable on the Teensy-3.1.
// -----------------------------------------------------------------------------

#ifndef MCP23018_H
#define MCP23018_H

#include <WProgram.h>
#include <i2c_t3.h>

// -----------------------------------------------------------------------------

class MCP23018
{
    //- Base I2C address
    static const uint8_t baseAddress_ = B0100000;

    // MCP23018 registers

    static const uint8_t IODIRA = 0x0;
    static const uint8_t IODIRB = 0x1;
    static const uint8_t GPPUA = 0x0C;
    static const uint8_t GPPUB = 0x0D;
    static const uint8_t GPIOA = 0x12;
    static const uint8_t GPIOB = 0x13;
    static const uint8_t OLATA = 0x14;
    static const uint8_t OLATB = 0x15;

    // The I2C connection to communicate over
    i2c_t3& wire_;

    //- I2C address of this instance
    const uint8_t i2cAddress_;

    //- Current register state
    uint16_t regState_;

    //- Write data to register reg
    void writeReg(uint8_t reg, uint8_t data);

    //- Write a to register reg A and b to register reg B
    void writeReg(uint8_t reg, uint8_t a, uint8_t b);

    //- Read and return byte from register reg
    uint8_t readReg(uint8_t reg);

    //- Write bits specified by mask from data to register reg
    void writeBitsReg
    (
        const uint8_t reg,
        const uint8_t regState,
        const uint8_t data,
        const uint8_t mask
    );

    inline void storeA(uint8_t a)
    {
        regState_ = (regState_ & 0xff00) | a;
    }

    inline void storeB(uint8_t b)
    {
        regState_ = (regState_ & 0x00ff) | (b << 8);
    }

    inline void storeAB(uint8_t a, uint8_t b)
    {
        regState_ = b << 8 | a;
    }


public:

    //- Construct to communicate over the given I2C connection
    //  using the 3 bits of the I2C address
    MCP23018(i2c_t3& wire, uint8_t a012);

    //- Complete the setup of the MCP23018 after Wire.begin()
    //  Supply the IO mask for the A and B ports
    //      input:   1
    //      output:  0
    //  and the pull-up resistor mask for the A and B ports
    //      enable:  1
    //      disable: 0
    void begin(uint8_t a, uint8_t b, uint8_t pullUpsA, uint8_t pullUpsB);

    //- Write byte a to port A
    void writeA(uint8_t a);

    //- Write byte b to port B
    void writeB(uint8_t b);

    //- Write byte a to port A and byte b to port B
    void write(uint8_t a, uint8_t b);

    //- Write word ab to ports A and B
    void write(uint16_t a);

    //- Read and return byte from port A
    uint8_t readA();

    //- Read and return byte from port B
    uint8_t readB();

    //- Read and return output latches (previous data) from Port A
    uint8_t readLatchA();

    //- Read and return output latches (previous data) from Port B
    uint8_t readLatchB();

    //- Write bits specified by mask from a to register A
    void writeBitsA(uint8_t a, uint8_t mask);

    //- Write bits specified by mask from b to register B
    void writeBitsB(uint8_t b, uint8_t mask);

    //- Write val to bit
    void writeBit(uint8_t bit, bool val);
};


// -----------------------------------------------------------------------------
#endif // MCP23018_H
// -----------------------------------------------------------------------------
