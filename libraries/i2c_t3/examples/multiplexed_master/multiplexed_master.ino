// -------------------------------------------------------------------------------------------
// Teensy3.0/3.1 I2C Multiplexed Master
// 08Mar13 Brian (nox771 at gmail.com)
// -------------------------------------------------------------------------------------------
//
// This creates an I2C multiplexed master device which talks to the simple I2C slave device
// given in the i2c_t3/slave sketch.  It is different than the Master sketch because it can
// output I2C commands on either pins 18/19 or pins 16/17, and change pins on-the-fly.
//
// Note that it is using the same I2C bus on both sets of pins (I2C0).  The purpose of this
// sketch is to demonstrate how the pins can be reconfigured on-the-fly when the bus is idle.
//
// This code assumes the slave config has 256byte memory and I2C address is 0x44.
// The various tests are started by pulling one of the control pins low.
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
void print_i2c_status(void);

void setup()
{
    pinMode(13,OUTPUT);       // LED
    pinMode(12,INPUT_PULLUP); // Control for Test1
    pinMode(11,INPUT_PULLUP); // Control for Test2

    // Setup for Master mode, pins 18/19, external pullups, 400kHz
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);

    Serial.begin(115200);
}

void loop()
{
    uint8_t data;
    uint8_t target1 = 0x44; // slave1 addr
    uint8_t target2 = 0x44; // slave2 addr

    //
    // Multi-bus test
    //
    // pull pin12 low to send command to slave1 on bus1 (pins 16/17)
    // pull pin11 low to send command to slave2 on bus2 (pins 18/19)
    //

    if(digitalRead(12) == LOW)
    {
        // do a 1 byte write on bus1
        //
        Wire.pinConfigure(I2C_PINS_16_17, I2C_PULLUP_EXT); // change pins

        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 1 byte to Slave 0x");
        Serial.print(target1,HEX);
        Serial.print(" on Bus1 (pins 16/17)\n");
        Serial.print("Writing: ");

        Wire.beginTransmission(target1);        // slave addr
        Wire.write(WRITE);                      // WRITE command
        Wire.write(0);                          // memory address
        data = 5;                               // set data (equal to mem addr+5)
        Wire.write(data);
        Serial.print(data);
        Serial.print(" ");
        digitalWrite(13,HIGH);                  // LED on
        Wire.endTransmission();                 // blocking I2C Tx (when not specified I2C_STOP is implicit)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("\n");
        print_i2c_status();                     // print I2C final status

        // do a 1 byte read on bus1
        //
        Wire.beginTransmission(target1);        // slave addr
        Wire.write(READ);                       // READ command
        Wire.write(0);                          // memory address
        digitalWrite(13,HIGH);                  // LED on
        Wire.endTransmission(I2C_NOSTOP);       // blocking write   (NOSTOP triggers RepSTART
        Wire.requestFrom(target1,1,I2C_STOP);   // blocking read    on next I2C command)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("I2C READ ");
        Serial.print((uint16_t)Wire.available());
        Serial.print(" bytes from Slave 0x");
        Serial.print(target1,HEX);
        Serial.print(" on Bus1 (pins 16/17)\n");
        Serial.print("Received: ");             // print received bytes
        while(Wire.available())
        {
            Serial.print(Wire.readByte());
            Serial.print(" ");
        }
        Serial.print("\n");
        print_i2c_status();                     // print I2C final status

        delay(500); // delay to space out tests
    }

    if(digitalRead(11) == LOW)
    {
        // do a 1 byte write on bus2
        //
        Wire.pinConfigure(I2C_PINS_18_19, I2C_PULLUP_EXT); // change pins

        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 1 byte to Slave 0x");
        Serial.print(target2,HEX);
        Serial.print(" on Bus2 (pins 18/19)\n");
        Serial.print("Writing: ");

        Wire.beginTransmission(target2);        // slave addr
        Wire.write(WRITE);                      // WRITE command
        Wire.write(0);                          // memory address
        data = 5;                               // set data (equal to mem addr+5)
        Wire.write(data);
        Serial.print(data);
        Serial.print(" ");
        digitalWrite(13,HIGH);                  // LED on
        Wire.endTransmission();                 // blocking I2C Tx (when not specified I2C_STOP is implicit)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("\n");
        print_i2c_status();                     // print I2C final status

        // do a 1 byte read on bus2
        //
        Wire.beginTransmission(target2);        // slave addr
        Wire.write(READ);                       // READ command
        Wire.write(0);                          // memory address
        digitalWrite(13,HIGH);                  // LED on
        Wire.endTransmission(I2C_NOSTOP);       // blocking write   (NOSTOP triggers RepSTART
        Wire.requestFrom(target2,1,I2C_STOP);   // blocking read    on next I2C command)
        digitalWrite(13,LOW);                   // LED off

        I2C_DEBUG_WAIT;                         // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("I2C READ ");
        Serial.print((uint16_t)Wire.available());
        Serial.print(" bytes from Slave 0x");
        Serial.print(target2,HEX);
        Serial.print(" on Bus2 (pins 18/19)\n");
        Serial.print("Received: ");             // print received bytes
        while(Wire.available())
        {
            Serial.print(Wire.readByte());
            Serial.print(" ");
        }
        Serial.print("\n");
        print_i2c_status();                     // print I2C final status

        delay(500); // delay to space out tests
    }
}

//
// print I2C status
//
void print_i2c_status(void)
{
    I2C_DEBUG_WAIT; // wait for Serial to clear debug msgs (only needed if using debug)
    switch(Wire.status())
    {
    case I2C_WAITING:  Serial.print("I2C waiting, no errors\n"); break;
    case I2C_ADDR_NAK: Serial.print("Slave addr not acknowledged\n"); break;
    case I2C_DATA_NAK: Serial.print("Slave data not acknowledged\n"); break;
    case I2C_ARB_LOST: Serial.print("Bus Error: Arbitration Lost\n"); break;
    default:           Serial.print("I2C busy\n"); break;
    }
}
