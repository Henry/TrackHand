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
/// Title: EEPROM configuration parameter management
///  Description:
//    Getters and setters for configuration parameters stored in EEPROM.
// -----------------------------------------------------------------------------

#ifndef EEPROMParameters_H
#define EEPROMParameters_H

// -----------------------------------------------------------------------------

template<typename Type>
Type eepromRead(const uint address)
{
    Type val;
    eeprom_read_block(&val, (uint8_t*)address, sizeof(val));
    return val;
}


template<typename Type>
void eepromStore(const uint address, const Type val)
{
    if (val != eepromRead<Type>(address))
    {
        eeprom_write_block(&val, (uint8_t*)address, sizeof(val));
    }
}


template<typename Type>
bool eepromStoreFromSerial
(
    const char cmd,
    const char* propName,
    const uint address
)
{
    const uint8_t n = 2*sizeof(Type);

    if (Serial.available() >= n)
    {
        Type value;
        Serial.readBytes((char *)(&value), sizeof(Type));
        Serial.println(value);
        Type check;
        Serial.readBytes((char *)(&check), sizeof(Type));
        Serial.println(check);

        // Simple parity check of the data provided
        if ((cmd ^ value) == check)
        {
            eepromStore(address, value);
            Serial.print("TrackHand: setting ");
            Serial.print(propName);
            Serial.print(" to ");
            Serial.print(value);
            Serial.println(" successful");

            return true;
        }
        else
        {
            // Discard any buffered input
            usb_serial_flush_input();
            Serial.print("TrackHand: setting ");
            Serial.print(propName);
            Serial.println(" failed");
        }
    }

    return false;
}



template<typename Type>
Type eepromRead(const uint address, const Type)
{
    return eepromRead<Type>(address);
}

template<typename Type>
bool eepromStoreFromSerial
(
    const char cmd,
    const char* propName,
    const uint address,
    const Type
)
{
    return eepromStoreFromSerial<Type>(cmd, propName, address);
}

#define PROP_ADDR(x)                                                           \
    eepromStart_ + offsetof(parameters, x)

#define eepromGet(property)                                                    \
    eepromRead(PROP_ADDR(property), ((parameters*)(NULL))->property)

#define eepromSet(property, val)                                               \
    eepromStore(PROP_ADDR(property), val)

#define eepromSetFromSerial(cmd, property)                                     \
    eepromStoreFromSerial                                                      \
    (                                                                          \
        cmd,                                                                   \
        #property,                                                             \
        PROP_ADDR(property),                                                   \
        ((parameters*)(NULL))->property                                        \
    )


// -----------------------------------------------------------------------------
#endif // EEPROMParameters_H
// -----------------------------------------------------------------------------
