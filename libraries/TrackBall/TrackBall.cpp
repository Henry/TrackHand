// -----------------------------------------------------------------------------
/// Title: Interface for the ADNS9800 laser sensor
// -----------------------------------------------------------------------------

#include "TrackBall.h"
#include "ADNS9800_SROM_A6.h"
#include <spi4teensy3.h>
#include "EEPROMParameters.h"
#include "debug.h"

// -----------------------------------------------------------------------------

void TrackBall::adnsBurstMotion(int16_t xy[2])
{
    adnsComBegin();

    // Send adress of the register, with MSBit = 0 to indicate it's a read
    spi4teensy3::send(REG_Motion_Burst & 0x7f);

    // tSRAD
    delayMicroseconds(100);

    // Read data
    uint8_t data[6];
    spi4teensy3::receive(data, 6);

    // Construct the 16-bit x and y motion deltas
    xy[0] = (data[3] << 8) | data[2];
    xy[1] = (data[5] << 8) | data[4];

    // tSCLK-NCS for read operation is 120ns
    delayMicroseconds(1);

    adnsComEnd();

    //  tSRW/tSRR (=20us) minus tSCLK-NCS
    delayMicroseconds(19);
}


uint8_t TrackBall::adnsReadReg(uint8_t reg_addr)
{
    adnsComBegin();

    // Send adress of the register, with MSBit = 0 to indicate it's a read
    spi4teensy3::send(reg_addr & 0x7f);

    // tSRAD
    delayMicroseconds(100);

    // Read data
    uint8_t data = spi4teensy3::receive();

    // tSCLK-NCS for read operation is 120ns
    delayMicroseconds(1);
    adnsComEnd();

    // tSRW/tSRR (=20us) minus tSCLK-NCS
    delayMicroseconds(19);

    return data;
}


void TrackBall::adnsWriteReg(uint8_t reg_addr, uint8_t data)
{
    adnsComBegin();

    // Send adress of the register, with MSBit = 1 to indicate it's a write
    spi4teensy3::send(reg_addr | 0x80 );

    // Sent data
    spi4teensy3::send(data);

    // tSCLK-NCS for write operation
    delayMicroseconds(20);
    adnsComEnd();

    // tSWW/tSWR (=120us) minus tSCLK-NCS.
    delayMicroseconds(100);
    // Could be shortened, but is looks like a safe lower bound
}


void TrackBall::adnsUploadFirmware()
{
    // Send the firmware to the chip, cf p.18 of the datasheet
    debugln("Uploading firmware...");

    // Set the configuration_IV register in 3k firmware mode
    // Bit 1 = 1 for 3k mode, other bits are reserved
    adnsWriteReg(REG_Configuration_IV, 0x02);

    // Write 0x1d in SROM_enable reg for initializing
    adnsWriteReg(REG_SROM_Enable, 0x1d);

    // Wait for more than one frame period
    // Assume that the frame rate is as low as 100fps...
    // even if it should never be that low
    delay(10);

    // Write 0x18 to SROM_enable to start SROM download
    adnsWriteReg(REG_SROM_Enable, 0x18);

    // Write the SROM file (=firmware data)
    adnsComBegin();

    // Write burst destination address
    spi4teensy3::send(REG_SROM_Load_Burst | 0x80);

    delayMicroseconds(15);

    // Send all bytes of the firmware
    for (int i=0; i<firmwareLength; i++)
    {
        unsigned char c = pgm_read_byte(firmwareData + i);
        spi4teensy3::send(c);
        delayMicroseconds(15);
    }

    adnsComEnd();
}


void TrackBall::configure()
{
    // Update the movement resolution and scroll-divider
    // from the values stored in EEPROM
    setResolution(eepromGet(resolution));
    scrollDivider_ = eepromGet(scrollDivider);
}


bool TrackBall::configure(const char cmd)
{
    switch (cmd)
    {
        case 'r':
            eepromSetFromSerial(cmd, resolution);
            setResolution(eepromGet(resolution));
            return true;
            break;
        case 's':
            eepromSetFromSerial(cmd, scrollDivider);
            scrollDivider_ = eepromGet(scrollDivider);
            return true;
            break;
        case 'p':
            Serial.print("TrackBall resolution ");
            Serial.println(eepromGet(resolution));
            Serial.print("TrackBall scrollDivider ");
            Serial.println(scrollDivider_);
            return true;
            break;
    }

    return false;
}


void TrackBall::setResolution(const uint8_t res)
{
    digitalWrite(ncs_, LOW);
    adnsWriteReg(REG_Configuration_I, res);
    digitalWrite(ncs_, HIGH);
}


void TrackBall::resolution(const uint8_t res)
{
    eepromSet(resolution, res);
    setResolution(res);
}


void TrackBall::scrollDivider(const uint8_t sdiv)
{
    eepromSet(scrollDivider, sdiv);
    scrollDivider_ = sdiv;
}


volatile bool TrackBall::moved_ = false;


void TrackBall::moved()
{
    TrackBall::moved_ = true;
}


TrackBall::TrackBall(const ptrdiff_t eepromStart)
:
    eepromStart_(eepromStart)
{}


void TrackBall::begin()
{
    // Setup SPI pins and interrupt for optical sensor
    pinMode(ncs_, OUTPUT);
    pinMode(mot_, INPUT);
    attachInterrupt(mot_, moved, FALLING);

    wake();
}


void TrackBall::sleep()
{
    adnsWriteReg(REG_Shutdown, 0xb6);

    // Switch off the SPI clock
    pinMode(SCK, OUTPUT);

    // Set the SCK pin to low to switch-off the built-in led on the same pin
    digitalWrite(SCK, LOW);
}


void TrackBall::wake()
{
    // 48MHz / 24 = 2MHz = fSCLK
    spi4teensy3::init(5,1,1);

    // Ensure that the serial port is reset
    adnsComEnd();
    adnsComBegin();
    adnsComEnd();

    // Force reset
    adnsWriteReg(REG_Power_Up_Reset, 0x5a);

    // Wait for it to reboot
    delay(50);

    // Read registers 0x02 to 0x06 (and discard the data)
    adnsReadReg(REG_Motion);
    adnsReadReg(REG_Delta_X_L);
    adnsReadReg(REG_Delta_X_H);
    adnsReadReg(REG_Delta_Y_L);
    adnsReadReg(REG_Delta_Y_H);

    // Upload the firmware
    adnsUploadFirmware();
    delay(10);

    // Enable laser (bit 0 = 0b), in normal mode (bits 3,2,1 = 000b)
    // reading the actual value of the register is important because the real
    // default value is different from what is said in the datasheet, and if you
    // change the reserved bytes (like by writing 0x00...) it would not work.
    uint8_t laser_ctrl0 = adnsReadReg(REG_LASER_CTRL0);
    adnsWriteReg(REG_LASER_CTRL0, laser_ctrl0 & 0xf0 );

    delay(100);

    configure();

    debugln("ADNS-9800 Initialized");
}


// Clip a 16bit integer to an 8bit integer
inline int8_t clip8(int16_t y)
{
    if (y > INT8_MAX) return INT8_MAX;
    if (y < INT8_MIN) return INT8_MIN;
    return y;
}


bool TrackBall::moveOrScroll(const bool moving)
{
    if (moved_)
    {
        // Reset the interrupt flag
        moved_ = false;

        // Read the ball motion from the ADNS-9800
        int16_t xy[2];
        adnsBurstMotion(xy);

        if (moving)
        {
            // Clip the movement to -128 to 127 per call
            // It would be possible to accumulate the overflow
            // and apply subsequently but limiting the speed by clipping as
            // is done here might be better anyway.
            Mouse.move(clip8(-xy[1]), clip8(-xy[0]));
        }
        else
        {
            // Reset scroll counter if direction changes
            if
            (
                (xy[0] > 0 && scrollCount_ > 0)
             || (xy[0] < 0 && scrollCount_ < 0)
            )
            {
                scrollCount_ = 0;
            }

            // Accumulate the scroll motion
            scrollCount_ -= xy[0];

            // Divide and clip the scroll motion before sending
            Mouse.scroll(clip8(scrollCount_/scrollDivider_));

            // Reduce the scroll count according to that sent
            // ignoring the clipping to limit scroll-speed
            scrollCount_ %= scrollDivider_;
        }

        return true;
    }

    return false;
}


// -----------------------------------------------------------------------------
