// -----------------------------------------------------------------------------
/// Title: Interface for the ADNS9800 laser sensor
// -----------------------------------------------------------------------------

#include "TrackBall.h"
#include "debug.h"
#include "ADNS9800_SROM_A6.h"
#include <spi4teensy3.h>
#include <EEPROM.h>

// -----------------------------------------------------------------------------

void TrackBall::adnsBurstMotion(int *xy)
{
    uint8_t data[6];

    adnsComBegin();

    // Send adress of the register, with MSBit = 0 to indicate it's a read
    spi4teensy3::send(REG_Motion_Burst & 0x7f);

    // tSRAD
    delayMicroseconds(100);

    // Read data
    spi4teensy3::receive(data, 6);

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

    // write burst destination address
    spi4teensy3::send(REG_SROM_Load_Burst | 0x80);
    delayMicroseconds(15);

    // Send all bytes of the firmware
    unsigned char c;
    for(int i=0; i<firmwareLength; i++)
    {
        c = (unsigned char)pgm_read_byte(firmwareData + i);
        spi4teensy3::send(c);
        delayMicroseconds(15);
    }

    adnsComEnd();
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

    debugln("Optical Chip Initialized");
}


void TrackBall::sleep()
{
    adnsWriteReg(REG_Shutdown, 0xb6);

    // Switch off the SPI clock
    pinMode(SCK, OUTPUT);

    // Set the SCK pin to low to switch-off the built-in led on the same pin
    digitalWrite(SCK, LOW);
}


void TrackBall::configure()
{
    adnsWriteReg(REG_Configuration_I, EEPROM.read(REG_Configuration_I));
}


volatile bool TrackBall::moved_ = true;


void TrackBall::moved()
{
    TrackBall::moved_ = true;
}


TrackBall::TrackBall()
{}


void TrackBall::begin()
{
    // Setup SPI pins and interrupt for optical sensor
    pinMode(ncs_, OUTPUT);
    pinMode(mot_, INPUT);
    attachInterrupt(mot_, moved, FALLING);

    wake();
    // resolution(10);
}


void TrackBall::resolution(const uint8_t newRes)
{
    if (newRes != EEPROM.read(REG_Configuration_I))
    {
        EEPROM.write(REG_Configuration_I, newRes);
        digitalWrite(ncs_, LOW);
        adnsWriteReg(REG_Configuration_I, newRes);
        digitalWrite(ncs_, HIGH);
    }
}


bool TrackBall::move()
{
    if (moved_)
    {
        moved_ = false;
        int xy[2];
        adnsBurstMotion(xy);
        Mouse.move(-xy[1],-xy[0]);

        return true;
    }

    if (Serial.available() >= 3)
    {
        uint8_t regptr = Serial.read();
        uint8_t value = Serial.read();
        uint8_t check = Serial.read();
        if ((regptr ^ value) == check)
        {
            EEPROM.write(regptr, value);
            configure();
        } else
        {
            // Discard any buffered input
            usb_serial_flush_input();
        }
    }

    return false;
}


// -----------------------------------------------------------------------------
