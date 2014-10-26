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
// -----------------------------------------------------------------------------

#include "MCP23018.h"

// -----------------------------------------------------------------------------

void MCP23018::writeReg(uint8_t reg, uint8_t data)
{
    wire_.beginTransmission(i2cAddress_);
    wire_.write(reg);
    wire_.write(data);
    wire_.endTransmission();
}


void MCP23018::writeReg(uint8_t reg, uint8_t a, uint8_t b)
{
    wire_.beginTransmission(i2cAddress_);
    wire_.write(reg);
    wire_.write(a);
    wire_.write(b);
    wire_.endTransmission();
}


uint8_t MCP23018::readReg(uint8_t reg)
{
    // Set register to read from
    wire_.beginTransmission(i2cAddress_);
    wire_.write(reg);
    wire_.endTransmission();

    // Expect to read 1 byte
    uint8_t data = 0;
    wire_.requestFrom(i2cAddress_, size_t(1));

    // Read all data available
    while (wire_.available())
    {
        data = wire_.receive();
    }

    // Return the last byte received, discarding others silently
    return data;
}


void MCP23018::writeBitsReg
(
    const uint8_t reg,
    const uint8_t regState,
    const uint8_t data,
    const uint8_t mask
)
{
    writeReg(reg, regState & ((mask ^ 0xff) | (data & mask)));
}


MCP23018::MCP23018(i2c_t3& wire, uint8_t a012)
:
    wire_(wire),
    i2cAddress_((a012 & B111) | baseAddress_),
    regState_(0x0000)
{}


void MCP23018::begin(uint8_t a, uint8_t b, uint8_t pullUpsA, uint8_t pullUpsB)
{
    // Set the IO configuration of the registers
    writeReg(IODIRA, a, b);

    // Set the pull-up resistors of the registers
    writeReg(GPPUA, pullUpsA, pullUpsB);
}


void MCP23018::writeA(uint8_t a)
{
    storeA(a);
    writeReg(GPIOA, a);
}

void MCP23018::writeB(uint8_t b)
{
    storeB(b);
    writeReg(GPIOB, b);
}

void MCP23018::write(uint8_t a, uint8_t b)
{
    storeAB(a, b);
    writeReg(GPIOA, a, b);
}


void MCP23018::write(uint16_t ab)
{
    regState_ = ab;
    writeReg(GPIOA, lowByte(ab), highByte(ab));
}


uint8_t MCP23018::readA()
{
    uint8_t a = readReg(GPIOA);
    return a;
}

uint8_t MCP23018::readB()
{
    uint8_t b = readReg(GPIOB);
    return b;
}


void MCP23018::writeBitsA(uint8_t a, uint8_t mask)
{
    writeBitsReg(GPIOA, lowByte(regState_), a, mask);
}

void MCP23018::writeBitsB(uint8_t b, uint8_t mask)
{
    writeBitsReg(GPIOB, highByte(regState_), b, mask);
}


void MCP23018::writeBit(uint8_t bit, bool val)
{
    if (val)
    {
        if (bit < 8)
        {
            writeReg(GPIOA, lowByte(regState_) | (1 << bit));
        }
        else if (bit < 16)
        {
            writeReg(GPIOB, highByte(regState_) | (1 << (bit - 8)));
        }
    }
    else
    {
        if (bit < 8)
        {
            writeReg(GPIOA, lowByte(regState_) & ~(1 << bit));
        }
        else if (bit < 16)
        {
            writeReg(GPIOB, highByte(regState_) & ~(1 << (bit - 8)));
        }
    }
}


// -----------------------------------------------------------------------------
