// -------------------------------------------------------------------------------------------
// Teensy3.0/3.1 I2C Master
// 08Mar13 Brian (nox771 at gmail.com)
// -------------------------------------------------------------------------------------------
//
// This creates an I2C master device which talks to the simple I2C slave device given in the
// i2c_t3/slave sketch.
//
// This code assumes the slave config has 256byte memory and I2C addr is 0x44.
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
    pinMode(10,INPUT_PULLUP); // Control for Test3
    pinMode(9,INPUT_PULLUP);  // Control for Test4

    Serial.begin(115200);

    // Setup for Master mode, pins 18/19, external pullups, 400kHz
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);
}

void loop()
{
    size_t addr, len;
    uint8_t data;
    uint8_t target = 0x44; // slave addr

    //
    // A sequence of different read/write techniques.
    // Pull respective control pin low to initiate sequence.
    //
    // All tests will first write values to the slave, then read back the values.
    // The readback values should match.
    //
    // The LED is turned on during I2C operations.  If it gets stuck on then the
    // ISR likely had a problem.  This can happen with excessive baud rate.
    //
    // Various forms of the Wire calls (blocking/non-blocking/STOP/NOSTOP) are
    // used in the different tests.
    //

    //
    // Test1 - single-byte reads/writes to slave
    //
    if(digitalRead(12) == LOW)
    {
        Serial.print("---------------------------------------------------\n");
        for(addr = 0; addr < 256; addr += 20)   // sweep addr in 20byte blocks
        {
            Serial.print("I2C WRITE 1 byte at MemAddr ");
            Serial.print(addr);
            Serial.print("\n");
            Serial.print("Writing: ");

            Wire.beginTransmission(target); // slave addr
            Wire.write(WRITE);              // WRITE command
            Wire.write(addr);               // memory address
            data = addr+5;                  // set data (equal to mem addr+5)
            Wire.write(data);
            Serial.print(data);
            Serial.print(" ");
            digitalWrite(13,HIGH);          // LED on
            Wire.endTransmission();         // blocking I2C Tx (when not specified I2C_STOP is implicit)
            digitalWrite(13,LOW);           // LED off

            I2C_DEBUG_WAIT;                 // wait for Serial to clear debug msgs (only needed if using debug)
            Serial.print("\n");
            print_i2c_status();             // print I2C final status
        }

        Serial.print("---------------------------------------------------\n");
        for(addr = 0; addr < 256; addr += 20)   // sweep addr in 20byte blocks
        {
            Wire.beginTransmission(target);     // slave addr
            Wire.write(READ);                   // READ command
            Wire.write(addr);                   // memory address
            digitalWrite(13,HIGH);              // LED on
            Wire.endTransmission(I2C_NOSTOP);   // blocking write   (NOSTOP triggers RepSTART
            Wire.requestFrom(target,1,I2C_STOP);// blocking read    on next I2C command)
            digitalWrite(13,LOW);               // LED off

            I2C_DEBUG_WAIT;                     // wait for Serial to clear debug msgs (only needed if using debug)
            Serial.print("I2C READ ");
            Serial.print((uint16_t)Wire.available());
            Serial.print(" bytes from MemAddr ");
            Serial.print(addr);
            Serial.print("\n");
            Serial.print("Received: ");         // print received bytes
            while(Wire.available())
            {
                Serial.print(Wire.readByte());
                Serial.print(" ");
            }
            Serial.print("\n");
            print_i2c_status();                 // print I2C final status
        }

        delay(500); // delay to space out tests
    }

    //
    // Test2 - 32byte block reads/writes to slave, sweep across memory address range
    //
    if(digitalRead(11) == LOW)
    {
        Serial.print("---------------------------------------------------\n");
        for(addr = 0; addr < 256; addr += 32)   // sweep addr in 32byte blocks
        {
            Serial.print("I2C WRITE 32 bytes at MemAddr ");
            Serial.print(addr);
            Serial.print("\n");
            Serial.print("Writing: ");

            Wire.beginTransmission(target);     // slave addr
            Wire.write(WRITE);                  // WRITE command
            Wire.write(addr);                   // memory address
            for(len = 0; len < 32; len++)       // write 32byte block
            {
                data = (addr+len)^0xFF;         // set data (equal to bit inverse mem addr)
                Wire.write(data);
                Serial.print(data);
                Serial.print(" ");
            }
            digitalWrite(13,HIGH);              // LED on
            Wire.endTransmission(I2C_STOP);     // blocking I2C Tx
            digitalWrite(13,LOW);               // LED off

            I2C_DEBUG_WAIT;                     // wait for Serial to clear debug msgs (only needed if using debug)
            Serial.print("\n");
            print_i2c_status();                 // print I2C final status
        }

        Serial.print("---------------------------------------------------\n");
        for(addr = 0; addr < 256; addr += 32)   // sweep addr in 32byte blocks
        {
            Wire.beginTransmission(target);     // slave addr
            Wire.write(READ);                   // READ command
            Wire.write(addr);                   // memory address
            digitalWrite(13,HIGH);              // LED on
            Wire.endTransmission(I2C_NOSTOP);   // blocking write   (NOSTOP triggers RepSTART
            Wire.requestFrom(target,32,I2C_STOP);// blocking read   on next I2C command)
            digitalWrite(13,LOW);               // LED off

            I2C_DEBUG_WAIT;                     // wait for Serial to clear debug msgs (only needed if using debug)
            Serial.print("I2C READ ");
            Serial.print(Wire.available());
            Serial.print(" bytes from MemAddr ");
            Serial.print(addr);
            Serial.print("\n");
            Serial.print("Received: ");         // print received bytes
            while(Wire.available())
            {
                Serial.print(Wire.readByte());
                Serial.print(" ");
            }
            Serial.print("\n");
            print_i2c_status();                 // print I2C final status
        }

        delay(500); // delay to space out tests
    }

    //
    // Test3 - max memory test, reads/writes entire slave memory at once
    //
    if(digitalRead(10) == LOW)
    {
        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 256 bytes at MemAddr 0\n");
        Serial.print("Writing: ");

        addr = 0;
        Wire.beginTransmission(target);     // slave addr
        Wire.write(WRITE);                  // WRITE command
        Wire.write(addr);                   // memory address
        for(len = 0; len < 256; len++)      // write 256byte block
        {
            data = len;                     // set data (equal to mem addr)
            Wire.write(data);
            Serial.print(data);
            Serial.print(" ");
        }
        digitalWrite(13,HIGH);              // LED on
        Wire.sendTransmission(I2C_STOP);    // non-blocking I2C Tx
        while(!Wire.done())
        {
            // since write is non-blocking, could do something else in
            // foreground here, while I2C is in background
        }
        digitalWrite(13,LOW);               // LED off

        I2C_DEBUG_WAIT;                     // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("\n");
        print_i2c_status();                 // print I2C final status

        Serial.print("---------------------------------------------------\n");
        Wire.beginTransmission(target);     // slave addr
        Wire.write(READ);                   // READ command
        Wire.write(addr);                   // memory address
        digitalWrite(13,HIGH);              // LED on
        Wire.endTransmission(I2C_NOSTOP);   // blocking write        (NOSTOP triggers RepSTART
        Wire.sendRequest(target,256,I2C_STOP);// non-blocking read   on next I2C command)
        //
        // since read is non-blocking, could do some short foreground task here,
        // then use Wire.finish() to complete Rx
        //
        Wire.finish();
        digitalWrite(13,LOW);               // LED off

        I2C_DEBUG_WAIT;                     // wait for Serial to clear debug msgs (only needed if using debug)
        Serial.print("I2C READ ");
        Serial.print(Wire.available());
        Serial.print(" bytes from MemAddr ");
        Serial.print(addr);
        Serial.print("\n");
        Serial.print("Received: ");         // print received bytes
        while(Wire.available())
        {
            Serial.print(Wire.readByte());
            Serial.print(" ");
        }
        Serial.print("\n");
        print_i2c_status();                 // print I2C final status

        delay(500); // delay to space out tests
    }

    //
    // Test4 - START/STOP/RepSTART tests - chains of different R/W tests
    //
    if(digitalRead(9) == LOW)
    {
        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 1 byte at MemAddr 0\n");
        Wire.beginTransmission(target);     // slave addr
        Wire.write(WRITE);                  // WRITE command
        Wire.write(0);                      // memory address
        Wire.write(1);                      // data
        digitalWrite(13,HIGH);              // LED on
        Wire.endTransmission(I2C_NOSTOP);   // blocking I2C Tx (NOSTOP)
        digitalWrite(13,LOW);               // LED off
        print_i2c_status();                 // print I2C final status

        Serial.print("RepSTART I2C WRITE 1 bytes at MemAddr 0\n");
        Wire.beginTransmission(target);     // slave addr
        Wire.write(WRITE);                  // WRITE command
        Wire.write(0);                      // memory address
        Wire.write(1);                      // data
        digitalWrite(13,HIGH);              // LED on
        Wire.endTransmission();             // blocking I2C Tx (implicit STOP)
        digitalWrite(13,LOW);               // LED off
        print_i2c_status();                 // print I2C final status

        Serial.print("I2C WRITE 1 byte at MemAddr 0 (to bad target addr)\n");
        Wire.beginTransmission(target+1);   // slave addr (non-existant target)
        Wire.write(WRITE);                  // WRITE command
        Wire.write(0);                      // memory address
        Wire.write(1);                      // data
        digitalWrite(13,HIGH);              // LED on
        Wire.endTransmission();             // blocking I2C Tx (implicit STOP)
        digitalWrite(13,LOW);               // LED off
        print_i2c_status();                 // print I2C final status

        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C READ 1 byte from MemAddr 0\n");
        Wire.beginTransmission(target);     // slave addr
        Wire.write(READ);                   // READ command
        Wire.write(0);                      // memory address
        digitalWrite(13,HIGH);              // LED on
        Wire.endTransmission(I2C_NOSTOP);   // blocking write   (NOSTOP - next cmd is RepSTART)
        Wire.requestFrom(target,1,I2C_NOSTOP);// blocking read (NOSTOP - next cmd is RepSTART)
        digitalWrite(13,LOW);               // LED off
        print_i2c_status();                 // print I2C final status

        Serial.print("RepSTART I2C READ 1 byte from MemAddr 0\n");
        Wire.beginTransmission(target);     // slave addr
        Wire.write(READ);                   // READ command
        Wire.write(0);                      // memory address
        digitalWrite(13,HIGH);              // LED on
        Wire.endTransmission(I2C_NOSTOP);   // blocking write   (NOSTOP - next cmd is RepSTART)
        Wire.requestFrom(target,1,I2C_STOP);// blocking read
        digitalWrite(13,LOW);               // LED off
        print_i2c_status();                 // print I2C final status

        Serial.print("I2C READ 1 byte from MemAddr 0 (using STOP/START between Tx/Rx parts)\n");
        Wire.beginTransmission(target);     // slave addr
        Wire.write(READ);                   // READ command
        Wire.write(0);                      // memory address
        digitalWrite(13,HIGH);              // LED on
        Wire.endTransmission(I2C_STOP);     // blocking write (use STOP/START between Tx/Rx)
        Wire.requestFrom(target,1,I2C_STOP);// blocking read
        digitalWrite(13,LOW);               // LED off
        print_i2c_status();                 // print I2C final status

        Serial.print("I2C READ 1 byte from MemAddr 0 (to bad target addr)\n");
        Wire.beginTransmission(target+1);   // slave addr (non-existant target)
        Wire.write(READ);                   // READ command
        Wire.write(0);                      // memory address
        digitalWrite(13,HIGH);              // LED on
        Wire.endTransmission(I2C_NOSTOP);   // blocking write   (should fail with Addr NAK)
        Wire.requestFrom(target+1,1,I2C_STOP);// blocking read (should fail with Addr NAK)
        digitalWrite(13,LOW);               // LED off
        print_i2c_status();                 // print I2C final status

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
