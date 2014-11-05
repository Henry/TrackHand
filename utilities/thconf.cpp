/// Copyright 2014 Henry G. Weller
// -----------------------------------------------------------------------------
//  This file is part of
/// ---     TrackHand: DataHand with Laser TrackBall
// -----------------------------------------------------------------------------
//
//  TrackHand is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  TrackHand is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//  for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with TrackHand.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
/// Title: TrackHand CLI configuration utility
///  Description:
//    Updates the configuration of the TrackHand via the USB serial device.
//    Movement resolution, scroll speed, idle timeout, and keymaps may be
//    reconfigured.
// -----------------------------------------------------------------------------

#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdlib>

#include <unistd.h>  // UNIX standard function definitions
#include <fcntl.h>   // File control definitions
#include <termios.h> // POSIX terminal control definitions
#include <getopt.h>

using std::cout;
using std::cerr;
using std::endl;

// -----------------------------------------------------------------------------

// Open the serial port and return the file descriptor
int openSerialPort(const char* ttyName)
{
    int fd = open(ttyName, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1)
    {
        // Could not open the port.
        cerr<< "openPort: Unable to open " << ttyName << " - "
            << std::strerror(errno) << endl;
        std::exit(errno);
    }
    else
    {
        fcntl(fd, F_SETFL, FNDELAY);
    }

    // cout<< "Opened port to " << ttyName << " with descriptor " << fd << endl;

    // Read the configuration of the port
    struct termios options;
    tcgetattr(fd, &options);

    // Set baud-rate to 9600
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // Ensure owner of port is unchanged and enable receiver
    options.c_cflag |= (CLOCAL | CREAD);

    // Set no parity and character size (8N1)
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    // Set raw input
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Disable software flow control
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Set raw output
    options.c_oflag &= ~OPOST;

    // Set the new options for the port
    if (tcsetattr(fd, TCSANOW, &options) == -1)
    {
        cerr<< "Error with tcsetattr - " << std::strerror(errno) << endl;
        std::exit(errno);
    }

    // Set read to return immediately
    fcntl(fd, F_SETFL, FNDELAY);

    return fd;
}


// Return the file descriptor for the given serial device name
// opening the port if it is not already open
int port(const char* ttyName)
{
    static int fd = -1;

    if (fd == -1)
    {
        fd = openSerialPort(ttyName);
    }

    return fd;
}


void getValue(const int fd, const char addr)
{
    char buf[2] = {0, addr};
    int n = write(fd, buf, 2);
    if (n < 0)
    {
        cerr<< "getValue failed to send address" << endl;
    }
    else
    {
        cout<< "getValue succeed to send address" << endl;
    }

    // Need large delay between send and receive
    usleep(50000);

    char val;
    n = read(fd, &val, 1);

    if (n == -1)
    {
        cerr<< "getValue failed to receive value - "
            << std::strerror(errno) << endl;
    }
    else
    {
        cout<< "getValue: value at address "
            << addr << " = " << int(val) << endl;
    }
}


void setValue(const int fd, const char addr, const char val)
{
    char buf[4] = {1, addr, val, addr ^ val};
    int n = write(fd, buf, 4);
    if (n < 0)
    {
        cerr<< "setValue failed" << endl;
    }
    else
    {
        cout<< "setValue succeed" << endl;
    }
}


void printUsage(std::ostream& os, int exitCode)
{
    os << "Usage: thconf [OPTION]..." << endl;
    os <<
        "  -h  --help               Print this usage information.\n"
        "  -d  --dev <name>         Name of the serial device used to communicate with the TrackHand.\n"
        "  -r  --resolution <val>   Set the pointer motion resolution in increments of 50cpi in range 1-168.\n"
        "  -s  --scroll <val>       Set the scroll divider to reduce the scroll speed.\n"
        "  -t  --timeout <val>      Time of inactivity after which power saving is enabled.\n"
        "  -k  --keymap <file>      Load a keymap from file.\n"
    ;

    std::exit(exitCode);
}


int main(int argc, char * const *argv)
{
    // Remember the name of the program, to incorporate in messages.
    const char *programName = argv[0];

    // A string listing valid short options letters.
    const char* const shortOptions = "hp:r:s:t:k:";

    // An array describing valid long options.
    const struct option longOptions[] =
    {
        { "help",         0, NULL, 'h' },
        { "dev",          1, NULL, 'd' },
        { "print",        1, NULL, 'p' },
        { "resolution",   1, NULL, 'r' },
        { "scroll",       1, NULL, 's' },
        { "timeout",      1, NULL, 't' },
        { "keymap",       1, NULL, 'k' },
        { NULL,           0, NULL, 0   }   // Required at end of array.
    };

    const char* ttyName = "/dev/ttyACM0";

    int nextOption;

    do
    {
        nextOption = getopt_long
        (
            argc,
            argv,
            shortOptions,
            longOptions,
            NULL
        );

        switch (nextOption)
        {
            case 'h':   // -h or --help
                // User has requested usage information.  Print it to standard
                // output, and exit with exit code zero (normal termination).
                printUsage(cout, 0);
                break;

            case 'd':   // -d name or --dev name
                // Set the name of the serial device used to communicate
                // with the TrackHand
                ttyName = optarg;
                break;

            case 'p':   // -p <val> or --print <val>
                getValue(port(ttyName), atoi(optarg));
                break;

            case 'r':   // -r <val> or --resolution <val>
                setValue(port(ttyName), 0, atoi(optarg));
                break;

            case 's':   // -s <val> or --scroll <val>
                setValue(port(ttyName), 1, atoi(optarg));
                break;

            case 't':   // -t <val> or --timeout <val>
                // Not implemented yet
                break;

            case 'k':   // -k <file> or --keymap <file>
                // Not implemented yet
                break;

            case '?':   // The user specified an invalid option.
                // Print usage information to standard error, and exit with exit
                // code one (indicating abnormal termination).
                printUsage(cerr, 1);

            case -1:    // Done with options.
                break;

            default:    // Something else: unexpected.
                return 1;
        }
    } while(nextOption != -1);

    close(port(ttyName));

    return 0;
}


// -----------------------------------------------------------------------------
