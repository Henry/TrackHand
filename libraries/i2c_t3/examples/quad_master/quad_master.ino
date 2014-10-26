// -------------------------------------------------------------------------------------------
// Teensy3.1 I2C Quad-Master
// 18Jan14 Brian (nox771 at gmail.com)
// -------------------------------------------------------------------------------------------
//
// *******************************************************************************************
// ** Note: This is a Teensy 3.1 ONLY sketch since it requires a device with two I2C buses. **
// *******************************************************************************************
//
// It creates an I2C quad-master device which utilizes both I2C buses, and both sets of pins
// for each bus, thereby creating a quad-master device.
//
// The "Wire" bus will communicate on pins 18(SDA0)/19(SCL0) and 16(SCL0)/17(SDA0).
// The "Wire1" bus will communicate on pins 29(SCL1)/30(SDA1) and 26(SCL1)/31(SDA1).
//
// This code assumes that each bus has a target device configured with the simple I2C slave
// given in the i2c_t3/slave sketch, with each having a 256byte memory and I2C addr 0x44.
//
// The various tests are started by pulling one of the control pins low as follows:
//
// pull pin9 low to send command to slave on bus1 (pins 18/19)
// pull pin10 low to send command to slave on bus2 (pins 16/17)
// pull pin11 low to send command to slave on bus3 (pins 29/30)
// pull pin12 low to send command to slave on bus4 (pins 26/31)
//
// This example code is in the public domain.
//
// -------------------------------------------------------------------------------------------
// Slave protocol is as follows:
// -------------------------------------------------------------------------------------------
// WRITE - The I2C Master can write to the device by transmitting the WRITE command,
//         a memory address to store to, and a sequence of data to store.
//         The command sequence is:
//
// START|I2CADDR+W|WRITE|MEMADDR|DATA0|DATA1|DATA2|...|STOP
//
// where START     = I2C START sequence
//       I2CADDR+W = I2C Slave address + I2C write flag
//       WRITE     = WRITE command
//       MEMADDR   = memory address to store data to
//       DATAx     = data byte to store, multiple bytes are stored to increasing address
//       STOP      = I2C STOP sequence
// -------------------------------------------------------------------------------------------
// READ - The I2C Master can read data from the device by transmitting the READ command,
//        a memory address to read from, and then issuing a STOP/START or Repeated-START,
//        followed by reading the data.  The command sequence is:
//
// START|I2CADDR+W|READ|MEMADDR|REPSTART|I2CADDR+R|DATA0|DATA1|DATA2|...|STOP
//
// where START     = I2C START sequence
//       I2CADDR+W = I2C Slave address + I2C write flag
//       READ      = READ command
//       MEMADDR   = memory address to read data from
//       REPSTART  = I2C Repeated-START sequence (or STOP/START if single Master)
//       I2CADDR+R = I2C Slave address + I2C read flag
//       DATAx     = data byte read by Master, multiple bytes are read from increasing address
//       STOP      = I2C STOP sequence
// -------------------------------------------------------------------------------------------

#include <i2c_t3.h>
#ifdef I2C_DEBUG
    #include <rbuf.h> // linker fix
#endif

// Command definitions
#define WRITE 0x10
#define READ  0x20

// Function prototypes
void printWireStatus(void);
void printWire1Status(void);
void printStatus(i2c_status status);

void setup()
{
    pinMode(13,OUTPUT);       // LED
    pinMode(9,INPUT_PULLUP);  // Control for Bus1
    pinMode(10,INPUT_PULLUP); // Control for Bus2
    pinMode(11,INPUT_PULLUP); // Control for Bus3
    pinMode(12,INPUT_PULLUP); // Control for Bus4

    // Setup for Master mode, default pins, 400kHz, and internal pullups
    // (note: internal pullups only used here to reduce test board clutter with 4 buses, external generally better)
    //
    // Wire
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, I2C_RATE_400);
    pinMode(16,INPUT_PULLUP); // Alternate Wire pins 16/17 should be initialized as inputs with same pullup style
    pinMode(17,INPUT_PULLUP);
    // Wire1
    Wire1.begin(I2C_MASTER, 0x00, I2C_PINS_29_30, I2C_PULLUP_INT, I2C_RATE_400);
    pinMode(26,INPUT_PULLUP); // Alternate Wire1 pins 26/31 should be initialized as inputs with same pullup style
    pinMode(31,INPUT_PULLUP);

    Serial.begin(115200);
}

void loop()
{
    // slave addr - note: using identical slave addresses is intentional, as this
    //                    is a common situation with many off-the-shelf devices
    uint8_t target = 0x44;
    uint8_t data, len;

    //
    // Quad-bus test
    //
    // pull pin9 low to send command to slave on bus1 (pins 18/19)
    // pull pin10 low to send command to slave on bus2 (pins 16/17)
    // pull pin11 low to send command to slave on bus3 (pins 29/30)
    // pull pin12 low to send command to slave on bus4 (pins 26/31)
    //

    if(digitalRead(9) == LOW)
    {
        // do a 32 byte write on bus1
        //
        Wire.pinConfigure(I2C_PINS_18_19, I2C_PULLUP_INT); // change pins

        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 32 bytes to Slave 0x");
        Serial.print(target,HEX);
        Serial.print(" on Bus1 (pins 18/19)\n");
        Serial.print("Writing: ");

        Wire.beginTransmission(target);         // slave addr
        Wire.write(WRITE);                      // WRITE command
        Wire.write(0);                          // memory address
        for(len = 0; len < 32; len++)           // write 32byte block
        {
            data = (10+len);                    // set data
            Wire.write(data);
            Serial.print(data);
            Serial.print(" ");
        }
        Wire.write(data);
        digitalWrite(13,HIGH);                  // LED on
        Wire.endTransmission();                 // blocking I2C Tx (when not specified I2C_STOP is implicit)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("\n");
        printWireStatus();                      // print I2C final status

        // do a 32 byte read on bus1
        //
        Wire.beginTransmission(target);         // slave addr
        Wire.write(READ);                       // READ command
        Wire.write(0);                          // memory address
        digitalWrite(13,HIGH);                  // LED on
        Wire.endTransmission(I2C_NOSTOP);       // blocking write (NOSTOP triggers RepSTART on next I2C command)
        Wire.requestFrom(target,32,I2C_STOP);   // blocking read (STOP is sent to release bus to idle state)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("I2C READ ");
        Serial.print((uint16_t)Wire.available());
        Serial.print(" bytes from Slave 0x");
        Serial.print(target,HEX);
        Serial.print(" on Bus1 (pins 18/19)\n");
        Serial.print("Received: ");             // print received bytes
        while(Wire.available())
        {
            Serial.print(Wire.readByte());
            Serial.print(" ");
        }
        Serial.print("\n");
        printWireStatus();                      // print I2C final status

        delay(500);                             // delay to space out tests
    }

    if(digitalRead(10) == LOW)
    {
        // do a 32 byte write on bus2
        //
        Wire.pinConfigure(I2C_PINS_16_17, I2C_PULLUP_INT); // change pins

        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 32 bytes to Slave 0x");
        Serial.print(target,HEX);
        Serial.print(" on Bus2 (pins 16/17)\n");
        Serial.print("Writing: ");

        Wire.beginTransmission(target);         // slave addr
        Wire.write(WRITE);                      // WRITE command
        Wire.write(0);                          // memory address
        for(len = 0; len < 32; len++)           // write 32byte block
        {
            data = (20+len);                    // set data
            Wire.write(data);
            Serial.print(data);
            Serial.print(" ");
        }
        Wire.write(data);
        digitalWrite(13,HIGH);                  // LED on
        Wire.endTransmission();                 // blocking I2C Tx (when not specified I2C_STOP is implicit)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("\n");
        printWireStatus();                      // print I2C final status

        // do a 32 byte read on bus2
        //
        Wire.beginTransmission(target);         // slave addr
        Wire.write(READ);                       // READ command
        Wire.write(0);                          // memory address
        digitalWrite(13,HIGH);                  // LED on
        Wire.endTransmission(I2C_NOSTOP);       // blocking write (NOSTOP triggers RepSTART on next I2C command)
        Wire.requestFrom(target,32,I2C_STOP);   // blocking read (STOP is sent to release bus to idle state)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("I2C READ ");
        Serial.print((uint16_t)Wire.available());
        Serial.print(" bytes from Slave 0x");
        Serial.print(target,HEX);
        Serial.print(" on Bus2 (pins 16/17)\n");
        Serial.print("Received: ");             // print received bytes
        while(Wire.available())
        {
            Serial.print(Wire.readByte());
            Serial.print(" ");
        }
        Serial.print("\n");
        printWireStatus();                      // print I2C final status

        delay(500);                             // delay to space out tests
    }

    if(digitalRead(11) == LOW)
    {
        // do a 32 byte write on bus3
        //
        Wire1.pinConfigure(I2C_PINS_29_30, I2C_PULLUP_INT); // change pins

        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 32 bytes to Slave 0x");
        Serial.print(target,HEX);
        Serial.print(" on Bus3 (pins 29/30)\n");
        Serial.print("Writing: ");

        Wire1.beginTransmission(target);        // slave addr
        Wire1.write(WRITE);                     // WRITE command
        Wire1.write(0);                         // memory address
        for(len = 0; len < 32; len++)           // write 32byte block
        {
            data = (30+len);                    // set data
            Wire1.write(data);
            Serial.print(data);
            Serial.print(" ");
        }
        Wire1.write(data);
        digitalWrite(13,HIGH);                  // LED on
        Wire1.endTransmission();                // blocking I2C Tx (when not specified I2C_STOP is implicit)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("\n");
        printWire1Status();                     // print I2C final status

        // do a 32 byte read on bus3
        //
        Wire1.beginTransmission(target);        // slave addr
        Wire1.write(READ);                      // READ command
        Wire1.write(0);                         // memory address
        digitalWrite(13,HIGH);                  // LED on
        Wire1.endTransmission(I2C_NOSTOP);      // blocking write (NOSTOP triggers RepSTART on next I2C command)
        Wire1.requestFrom(target,32,I2C_STOP);  // blocking read (STOP is sent to release bus to idle state)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("I2C READ ");
        Serial.print((uint16_t)Wire1.available());
        Serial.print(" bytes from Slave 0x");
        Serial.print(target,HEX);
        Serial.print(" on Bus3 (pins 29/30)\n");
        Serial.print("Received: ");             // print received bytes
        while(Wire1.available())
        {
            Serial.print(Wire1.readByte());
            Serial.print(" ");
        }
        Serial.print("\n");
        printWire1Status();                     // print I2C final status

        delay(500);                             // delay to space out tests
    }

    if(digitalRead(12) == LOW)
    {
        // do a 32 byte write on bus4
        //
        Wire1.pinConfigure(I2C_PINS_26_31, I2C_PULLUP_INT); // change pins

        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 32 bytes to Slave 0x");
        Serial.print(target,HEX);
        Serial.print(" on Bus4 (pins 26/31)\n");
        Serial.print("Writing: ");

        Wire1.beginTransmission(target);        // slave addr
        Wire1.write(WRITE);                     // WRITE command
        Wire1.write(0);                         // memory address
        for(len = 0; len < 32; len++)           // write 32byte block
        {
            data = (40+len);                    // set data
            Wire1.write(data);
            Serial.print(data);
            Serial.print(" ");
        }
        Wire1.write(data);
        digitalWrite(13,HIGH);                  // LED on
        Wire1.endTransmission();                // blocking I2C Tx (when not specified I2C_STOP is implicit)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("\n");
        printWire1Status();                     // print I2C final status

        // do a 32 byte read on bus4
        //
        Wire1.beginTransmission(target);        // slave addr
        Wire1.write(READ);                      // READ command
        Wire1.write(0);                         // memory address
        digitalWrite(13,HIGH);                  // LED on
        Wire1.endTransmission(I2C_NOSTOP);      // blocking write (NOSTOP triggers RepSTART on next I2C command)
        Wire1.requestFrom(target,32,I2C_STOP);  // blocking read (STOP is sent to release bus to idle state)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("I2C READ ");
        Serial.print((uint16_t)Wire1.available());
        Serial.print(" bytes from Slave 0x");
        Serial.print(target,HEX);
        Serial.print(" on Bus4 (pins 26/31)\n");
        Serial.print("Received: ");             // print received bytes
        while(Wire1.available())
        {
            Serial.print(Wire1.readByte());
            Serial.print(" ");
        }
        Serial.print("\n");
        printWire1Status();                     // print I2C final status

        delay(500);                             // delay to space out tests
    }
}

//
// print I2C status
//
void printWireStatus(void) { printStatus(Wire.status()); }
void printWire1Status(void) { printStatus(Wire1.status()); }
void printStatus(i2c_status status)
{
    I2C_DEBUG_WAIT; // wait for Serial to clear debug msgs (only needed if using debug)
    switch(status)
    {
    case I2C_WAITING:  Serial.print("I2C waiting, no errors\n"); break;
    case I2C_ADDR_NAK: Serial.print("Slave addr not acknowledged\n"); break;
    case I2C_DATA_NAK: Serial.print("Slave data not acknowledged\n"); break;
    case I2C_ARB_LOST: Serial.print("Bus Error: Arbitration Lost\n"); break;
    default:           Serial.print("I2C busy\n"); break;
    }
}

