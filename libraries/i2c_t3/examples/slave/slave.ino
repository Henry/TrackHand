// -------------------------------------------------------------------------------------------
// Teensy3.0/3.1 I2C Slave
// 08Mar13 Brian (nox771 at gmail.com)
// -------------------------------------------------------------------------------------------
//
// This creates an I2C slave device with simple read/write commands and a small
// addressable memory.
//
// This example code is in the public domain.
//
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
void receiveEvent(size_t len);
void requestEvent(void);

// Memory
#define MEM_LEN 256
uint8_t mem[MEM_LEN];
uint8_t cmd;
size_t addr;

//
// Setup
//
void setup()
{
    pinMode(13,OUTPUT); // LED

    // Setup for Slave mode, address 0x44, pins 18/19, external pullups, 400kHz
    Wire.begin(I2C_SLAVE, 0x44, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);

    // init vars
    cmd = 0;
    addr = 0;
    for(size_t i=0; i < MEM_LEN; i++)
        mem[i] = 0;

    // register events
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
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
    if(Wire.available())
    {
        // grab command
        cmd = Wire.readByte();
        switch(cmd)
        {
        case WRITE:
            addr = Wire.readByte();                // grab addr
            while(Wire.available())
                if(addr < MEM_LEN)                 // drop data beyond mem boundary
                    mem[addr++] = Wire.readByte(); // copy data to mem
                else
                    Wire.readByte();               // drop data if mem full
            break;

        case READ:
            addr = Wire.readByte();                // grab addr
            break;
        }
    }
}

//
// handle Tx Event (outgoing I2C data)
//
void requestEvent(void)
{
    switch(cmd)
    {
    case READ:
        Wire.write(&mem[addr], MEM_LEN-addr); // fill Tx buffer (from addr location to end of mem)
        break;
    }
}

//
// print I2C status
//
void print_i2c_status(void)
{
    switch(Wire.status())
    {
    case I2C_WAITING:  Serial.print("I2C waiting, no errors\n"); break;
    case I2C_ADDR_NAK: Serial.print("Slave addr not acknowledged\n"); break;
    case I2C_DATA_NAK: Serial.print("Slave data not acknowledged\n"); break;
    case I2C_ARB_LOST: Serial.print("Bus Error: Arbitration Lost\n"); break;
    default:           Serial.print("I2C busy\n"); break;
    }
}
