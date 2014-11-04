// -----------------------------------------------------------------------------
/// Title: Interface for the ADNS9800 laser sensor
///  Description:
//    This class is based on the Teensy interface for the ADNS-9800 provided by
//    John Kicklighter:
//      https://github.com/mrjohnk/ADNS-9800.git
//      https://github.com/mrjohnk/Trackball2.git
//    with updates for the Teensy-3.1 from
//      https://github.com/pepijndevos/Dwergmuis
// -----------------------------------------------------------------------------

#ifndef TrackBall_H
#define TrackBall_H

#include "WProgram.h"

// -----------------------------------------------------------------------------

class TrackBall
{
    //- Configure from parameters stored in EEPROM
    void configure();

    void adnsBurstMotion(int16_t xy[2]);
    uint8_t adnsReadReg(uint8_t reg_addr);
    void adnsWriteReg(uint8_t reg_addr, uint8_t data);
    void adnsUploadFirmware();

    inline void adnsComBegin()
    {
        digitalWrite(ncs_, LOW);
    }

    inline void adnsComEnd()
    {
        digitalWrite(ncs_, HIGH);
    }

    //- SPI device select pin
    const uint8_t ncs_ = SS;

    //- Motion interupt pin
    const uint8_t mot_ = 9;

    //- Scroll divider reduce the scroll speed relative to the pointer motion.
    uint8_t scrollDivider_ = 50;

    //- Structure representing the storage of the parameters in EEPROM
    struct parameters
    {
        uint8_t resolution;
        uint8_t scrollDivider;
    };

    //- Current scroll counter used with scrollDivider_ to reduce scroll speed
    int16_t scrollCount_ = 0;

    //- Change the resolution for movement or scroll
    void setResolution(const uint8_t res);

    //- Moved indicator has to be a static member
    //  as it is used in...
    static volatile bool moved_;

    //- The static interrupt function indicating ball motion
    static void moved();


public:

    // Constructor
    TrackBall();

    // Member functions

        //- Setup SPI and ADNS9800 interfaces
        void begin();

        //- Sleep to save power and the laser
        void sleep();

        //- Wake after sleep
        void wake();

        //- Read and update configuration from USB serial device
        bool readConfiguration();

        //- Change and save the pointer movement resolution
        void resolution(const uint8_t res);

        //- Change and save the scroll divider
        void scrollDivider(const uint8_t sdiv);

        //- If data is present move the pointer (if move = true)
        //  or scroll the screen (if move = false) and return true
        //  otherwise return false
        bool moveOrScroll(const bool moving);
};


// -----------------------------------------------------------------------------
#endif // TrackBall_H
// -----------------------------------------------------------------------------
