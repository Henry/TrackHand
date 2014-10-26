// -------------------------------------------------------------------------------------------
// Teensy3.0/3.1 I2C Slave w/Address Range
// 08Mar13 Brian (nox771 at gmail.com)
// -------------------------------------------------------------------------------------------
//
// This creates an I2C slave device which will respond to a range of I2C addresses.  It will
// dump any data it receives to Serial.  If a Master tries to read from it, it will respond
// with zeros.
//
// This example code is in the public domain.
// -------------------------------------------------------------------------------------------

#include <i2c_t3.h>
#include <rbuf.h>

// Function prototypes
void receiveEvent(size_t len);
void requestEvent(void);
void printI2C(void);

// Print setup - this method is used instead of Serial direct in order to handle long data
//               streams which would otherwise overflow Serial's internal buffer
//
rbuf i2cPrint;               // ring buffer for holding Rx data
uint8_t i2cPrintBuf[32];     // small buffer for moving data to Serial
size_t i2cPrintBufLen;
IntervalTimer i2cPrintTimer; // timer for moving data to Serial

//
// Setup
//
void setup()
{
    pinMode(13,OUTPUT); // LED

    // Setup for Slave mode, addresses 0x08 to 0x77, pins 18/19, external pullups, 400kHz
    Wire.begin(I2C_SLAVE, 0x08, 0x77, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);

    // register events
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    // Print setup
    Serial.begin(115200);
    i2cPrintTimer.begin(printI2C, 250); // 250us/4kHz timer
}

void loop()
{
    digitalWrite(13,HIGH); // double pulse LED while waiting for I2C requests
    delay(10);             // if the LED stops the slave is probably stuck in an ISR
    digitalWrite(13,LOW);
    delay(100);
    digitalWrite(13,HIGH);
    delay(10);
    digitalWrite(13,LOW);
    delay(880);
}

//
// handle Rx Event (incoming I2C request/data)
//
void receiveEvent(size_t len)
{
    i2cPrint.putBlock("Slave 0x", 8);
    i2cPrint.putAHex(Wire.getRxAddr());
    if(Wire.available())
        i2cPrint.putBlock(" Rx: ", 5);
    else
        i2cPrint.putBlock(" Rx: None", 9);
    while(Wire.available())
    {
        i2cPrint.putAHex(Wire.readByte());
        i2cPrint.putI(' ');
    }
    i2cPrint.putI('\n');
}

//
// handle Tx Event (outgoing I2C data)
//
void requestEvent(void)
{
    i2cPrint.putBlock("Slave 0x", 8);
    i2cPrint.putAHex(Wire.getRxAddr());
    i2cPrint.putBlock(" Tx: (zeroes)\n", 14);
    // don't fill Tx buffer, it will automatically return zeroes when empty
}

//
// print I2C
//
void printI2C(void)
{
    i2cPrintBufLen = i2cPrint.len();
    if(i2cPrintBufLen)
    {
        i2cPrintBufLen = (i2cPrintBufLen > 32) ? 32 : i2cPrintBufLen; // limit to prevent Serial overflow
        i2cPrint.getBlock(i2cPrintBuf,i2cPrintBufLen);
        Serial.write(i2cPrintBuf,i2cPrintBufLen);
    }
}
