// -------------------------------------------------------------------------------------------
// Teensy3.0/3.1 I2C Timeout Test
// 15May13 Brian (nox771 at gmail.com)
// -------------------------------------------------------------------------------------------
//
// This creates an I2C master device which talks to the simple I2C slave device given in the
// i2c_t3/slave sketch.  It tests the various timeout functions.
//
// This code assumes the slave config has >8byte memory and I2C addr is 0x44.
// The test is started by pulling the control pin low.
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
void print_duration(uint32_t time);

void setup()
{
    pinMode(13,OUTPUT); // LED
    pinMode(12,INPUT_PULLUP); // Control for Test

    // Setup for Master mode, pins 18/19, external pullups, 400kHz
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);

    Serial.begin(115200);
}

void loop()
{
    uint8_t target1 = 0x44;
    elapsedMicros deltaT;
    uint32_t hold;

    if(digitalRead(12) == LOW)
    {
        //
        // Test1 - blocking transmit with timeout
        //
        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 8 byte to Slave 0x");
        Serial.print(target1,HEX);
        Serial.print(" (50us timeout)\n");

        Wire.beginTransmission(target1);// slave addr
        Wire.write(WRITE);              // WRITE command
        Wire.write(0);                  // memory address
        for(uint8_t i=0; i<8; i++)
            Wire.write(i);              // fill data buffer
        digitalWrite(13,HIGH);          // LED on
        deltaT = 0;
        Wire.endTransmission(I2C_STOP,50); // blocking Tx with timeout
        hold = deltaT;
        digitalWrite(13,LOW);           // LED off

        print_i2c_status();             // print I2C final status
        print_duration(hold);           // print duration

        delay(20); // delay to space out tests

        //
        // Test2 - same blocking transmit but with no timeout
        //
        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 8 byte to Slave 0x");
        Serial.print(target1,HEX);
        Serial.print(" (no timeout)\n");

        Wire.beginTransmission(target1);// slave addr
        Wire.write(WRITE);              // WRITE command
        Wire.write(0);                  // memory address
        for(uint8_t i=0; i<8; i++)
            Wire.write(i);              // fill data buffer
        digitalWrite(13,HIGH);          // LED on
        deltaT = 0;
        Wire.endTransmission(I2C_STOP); // blocking Tx no timeout
        hold = deltaT;
        digitalWrite(13,LOW);           // LED off

        print_i2c_status();             // print I2C final status
        print_duration(hold);           // print duration

        delay(20); // delay to space out tests

        //
        // Test3 - blocking receive with timeout
        //
        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C READ 8 byte from Slave 0x");
        Serial.print(target1,HEX);
        Serial.print(" (50us timeout on READ)\n");
        Serial.print("Note: resulting timeout duration is longer since this takes 2 commands WRITE then READ\n");

        Wire.beginTransmission(target1);// slave addr
        Wire.write(READ);               // WRITE command
        Wire.write(0);                  // memory address
        digitalWrite(13,HIGH);          // LED on
        deltaT = 0;
        Wire.endTransmission(I2C_STOP); // blocking Tx
        Wire.requestFrom(target1,8,I2C_STOP,50); // blocking Rx with timeout
        hold = deltaT;
        digitalWrite(13,LOW);           // LED off

        print_i2c_status();             // print I2C final status
        print_duration(hold);           // print duration

        delay(20); // delay to space out tests

        //
        // Test4 - blocking receive no timeout
        //
        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C READ 8 byte from Slave 0x");
        Serial.print(target1,HEX);
        Serial.print(" (no timeout)\n");

        Wire.beginTransmission(target1);// slave addr
        Wire.write(READ);               // WRITE command
        Wire.write(0);                  // memory address
        digitalWrite(13,HIGH);          // LED on
        deltaT = 0;
        Wire.endTransmission(I2C_STOP); // blocking Tx
        Wire.requestFrom(target1,8,I2C_STOP); // blocking Rx no timeout
        hold = deltaT;
        digitalWrite(13,LOW);           // LED off

        print_i2c_status();             // print I2C final status
        print_duration(hold);           // print duration

        delay(20); // delay to space out tests

        //
        // Test5 - non-blocking transmit with finish timeout
        //
        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 8 byte to Slave 0x");
        Serial.print(target1,HEX);
        Serial.print(" (non-blocking with 50us finish() timeout)\n");

        Wire.beginTransmission(target1);// slave addr
        Wire.write(WRITE);              // WRITE command
        Wire.write(0);                  // memory address
        for(uint8_t i=0; i<8; i++)
            Wire.write(i);              // fill data buffer
        digitalWrite(13,HIGH);          // LED on
        deltaT = 0;
        Wire.sendTransmission(I2C_STOP); // non-blocking Tx
        //
        // I2C working in background, so do other stuff here
        //
        Wire.finish(50);                // finish with timeout
        hold = deltaT;
        digitalWrite(13,LOW);           // LED off

        print_i2c_status();             // print I2C final status
        print_duration(hold);           // print duration

        delay(20); // delay to space out tests

        //
        // Test6 - non-blocking transmit with finish no timeout
        //
        Serial.print("---------------------------------------------------\n");
        Serial.print("I2C WRITE 8 byte to Slave 0x");
        Serial.print(target1,HEX);
        Serial.print(" (non-blocking with finish() no timeout)\n");

        Wire.beginTransmission(target1);// slave addr
        Wire.write(WRITE);              // WRITE command
        Wire.write(0);                  // memory address
        for(uint8_t i=0; i<8; i++)
            Wire.write(i);              // fill data buffer
        digitalWrite(13,HIGH);          // LED on
        deltaT = 0;
        Wire.sendTransmission(I2C_STOP); // non-blocking Tx
        //
        // I2C working in background, so do other stuff here
        //
        Wire.finish();                  // finish with no timeout
        hold = deltaT;
        digitalWrite(13,LOW);           // LED off

        print_i2c_status();             // print I2C final status
        print_duration(hold);           // print duration

        delay(1000); // delay to space out tests
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
    case I2C_TIMEOUT:  Serial.print("I2C Timeout\n"); break;
    default:           Serial.print("I2C busy\n"); break;
    }
}

//
// print duration
//
void print_duration(uint32_t time)
{
    Serial.print("Duration: ");
    Serial.print(time);
    Serial.print("us\n");
}
