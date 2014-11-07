// -----------------------------------------------------------------------------
/// Title: Interface for the ADNS9800 laser sensor
///  Description:
//    Class based on the Teensy interface for the ADNS-9800 provided by
//    John Kicklighter:
//      https://github.com/mrjohnk/ADNS-9800.git
//      https://github.com/mrjohnk/Trackball2.git
//    with updates for the Teensy-3.1 from
//      https://github.com/pepijndevos/Dwergmuis
//
//    Added support for low-power sleep mode.
// -----------------------------------------------------------------------------

#ifndef ADNS9800_H
#define ADNS9800_H

#include "WProgram.h"

// Registers
#define REG_Product_ID                           0x00
#define REG_Revision_ID                          0x01
#define REG_Motion                               0x02
#define REG_Delta_X_L                            0x03
#define REG_Delta_X_H                            0x04
#define REG_Delta_Y_L                            0x05
#define REG_Delta_Y_H                            0x06
#define REG_SQUAL                                0x07
#define REG_Pixel_Sum                            0x08
#define REG_Maximum_Pixel                        0x09
#define REG_Minimum_Pixel                        0x0a
#define REG_Shutter_Lower                        0x0b
#define REG_Shutter_Upper                        0x0c
#define REG_Frame_Period_Lower                   0x0d
#define REG_Frame_Period_Upper                   0x0e
#define REG_Configuration_I                      0x0f
#define REG_Configuration_II                     0x10
#define REG_Frame_Capture                        0x12
#define REG_SROM_Enable                          0x13
#define REG_Run_Downshift                        0x14
#define REG_Rest1_Rate                           0x15
#define REG_Rest1_Downshift                      0x16
#define REG_Rest2_Rate                           0x17
#define REG_Rest2_Downshift                      0x18
#define REG_Rest3_Rate                           0x19
#define REG_Frame_Period_Max_Bound_Lower         0x1a
#define REG_Frame_Period_Max_Bound_Upper         0x1b
#define REG_Frame_Period_Min_Bound_Lower         0x1c
#define REG_Frame_Period_Min_Bound_Upper         0x1d
#define REG_Shutter_Max_Bound_Lower              0x1e
#define REG_Shutter_Max_Bound_Upper              0x1f
#define REG_LASER_CTRL0                          0x20
#define REG_Observation                          0x24
#define REG_Data_Out_Lower                       0x25
#define REG_Data_Out_Upper                       0x26
#define REG_SROM_ID                              0x2a
#define REG_Lift_Detection_Thr                   0x2e
#define REG_Configuration_V                      0x2f
#define REG_Configuration_IV                     0x39
#define REG_Power_Up_Reset                       0x3a
#define REG_Shutdown                             0x3b
#define REG_Inverse_Product_ID                   0x3f
#define REG_Motion_Burst                         0x50
#define REG_SROM_Load_Burst                      0x62
#define REG_Pixel_Burst                          0x64

// -----------------------------------------------------------------------------

class ADNS9800
{
    //- Firmware size
    static const unsigned short firmwareLength_ = 3070;

    //- Firmware date
    static const uint8_t firmwareData_[firmwareLength_];


protected:

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

    //- Moved indicator has to be a static member
    //  as it is used in...
    static volatile bool moved_;

    //- The static interrupt function indicating ball motion
    static void moved();


public:

    // Constructor
    ADNS9800();

    // Member functions

        //- Setup SPI and ADNS9800 interfaces
        void begin();

        //- Change the resolution for movement or scroll
        void setResolution(const uint8_t res);

        //- Sleep to save power and the laser
        void sleep();

        //- Wake after sleep
        void wake();
};


// -----------------------------------------------------------------------------
#endif // ADNS9800_H
// -----------------------------------------------------------------------------
