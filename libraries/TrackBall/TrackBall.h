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
    void configure();

    void adnsBurstMotion(int *xy);
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

    const uint8_t ncs_ = SS;
    const uint8_t mot_ = 9;

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

        //- Change the resolution
        void resolution(const uint8_t newResolution);

        //- Move the pointer if data is present and return true
        //  otherwise return false
        bool move();
};


// -----------------------------------------------------------------------------
#endif // TrackBall_H
// -----------------------------------------------------------------------------
