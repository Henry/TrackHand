###-----------------------------------------------------------------------------
##  This file is part of
### ---     TrackHand: DataHand with Laser TrackBall
###-----------------------------------------------------------------------------
### Title: Makefile
###  Description:
###    Teensy project Makefile based on
###    https://github.com/apmorton/teensy-template
###    Compile only: make
###    Compile and upload: make load
###-----------------------------------------------------------------------------
PROGRAM = TrackHand

# Set to 1 (INITIALIZE=1 on the make command line, compile and load
# to initialize parameters in EEPROM.  Reset to 0 recompile and reload.
INITIALIZE = 0

# The name of your project (used to name the compiled .hex file)
TARGET = $(PROGRAM)

# The teensy version to use, 30 or 31
TEENSY = 31

# Set to 24000000, 48000000, or 96000000 to set CPU core speed
TEENSY_CORE_SPEED = 48000000

# configurable options
OPTIONS = -DUSB_SERIAL_HID -DLAYOUT_US_ENGLISH -DINITIALIZE=$(INITIALIZE)
# -DDEBUG

# directory to build in
BUILDDIR = $(abspath $(CURDIR)/build)

###-----------------------------------------------------------------------------
### Location of Teensyduino utilities, Toolchain, and Arduino Libraries.
### To use this makefile without Arduino, copy the resources from these
### locations and edit the pathnames.  The rest of Arduino is not needed.
###-----------------------------------------------------------------------------

# path location for Teensy Loader, teensy_post_compile and teensy_reboot
TOOLSPATH = $(CURDIR)/tools

# path location for Teensy 3 core
COREPATH = teensy3

# path location for Arduino libraries
LIBRARYPATH = libraries

# path location for the arm-none-eabi compiler
COMPILERPATH = $(TOOLSPATH)/arm-none-eabi/bin

###-----------------------------------------------------------------------------
### Settings below this point usually do not need to be edited
###-----------------------------------------------------------------------------

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall -g -Os -mcpu=cortex-m4 -mthumb -nostdlib -MMD $(OPTIONS) \
    -DF_CPU=$(TEENSY_CORE_SPEED) -I$(PROGRAM) -I$(COREPATH)

# compiler options for C++ only
CXXFLAGS = -std=gnu++0x -felide-constructors -fno-exceptions -fno-rtti -Wno-narrowing -Wno-overflow

# compiler options specific to teensy version
ifeq ($(TEENSY), 30)
    CPPFLAGS += -D__MK20DX128__
    TEENSY_MC = mk20dx128
    LDSCRIPT = $(COREPATH)/$(TEENSY_MC).ld
else
    ifeq ($(TEENSY), 31)
        CPPFLAGS += -D__MK20DX256__
	TEENSY_MC = mk20dx256
        LDSCRIPT = $(COREPATH)/$(TEENSY_MC).ld
    else
        $(error Invalid setting for TEENSY)
    endif
endif

# set arduino define if given
ifdef ARDUINO
	CPPFLAGS += -DARDUINO=$(ARDUINO)
endif

# linker options
LDFLAGS = -Os -Wl,--gc-sections -mcpu=cortex-m4 -mthumb -T$(LDSCRIPT)

# additional libraries to link
LIBS = -lm

# names for the compiler programs
CC = $(abspath $(COMPILERPATH))/arm-none-eabi-gcc
CXX = $(abspath $(COMPILERPATH))/arm-none-eabi-g++
OBJCOPY = $(abspath $(COMPILERPATH))/arm-none-eabi-objcopy
SIZE = $(abspath $(COMPILERPATH))/arm-none-eabi-size

# automatically create lists of the sources and objects
LC_FILES := $(wildcard $(LIBRARYPATH)/*/*.c)
LCPP_FILES := $(wildcard $(LIBRARYPATH)/*/*.cpp)
TC_FILES := $(wildcard $(COREPATH)/*.c)
TCPP_FILES := $(wildcard $(COREPATH)/*.cpp)
C_FILES := $(wildcard $(PROGRAM)/*.c)
CPP_FILES := $(wildcard $(PROGRAM)/*.cpp)

# include paths for libraries
L_INC := $(foreach lib,$(filter %/, $(wildcard $(LIBRARYPATH)/*/)), -I$(lib))

SOURCES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) \
    $(TC_FILES:.c=.o) $(TCPP_FILES:.cpp=.o) $(LC_FILES:.c=.o) $(LCPP_FILES:.cpp=.o)

OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))

all: hex thconf

build: $(TARGET).elf

hex: $(TARGET).hex

post_compile: $(TARGET).hex
	@$(abspath $(TOOLSPATH))/teensy_post_compile -file="$(basename $<)" \
	-path=$(CURDIR) -tools="$(abspath $(TOOLSPATH))"

reboot:
	@-$(abspath $(TOOLSPATH))/teensy_reboot

load: $(TARGET).hex
	teensy_loader_cli -mmcu=$(TEENSY_MC) -w $(TARGET).hex

upload: post_compile reboot

$(BUILDDIR)/%.o: %.c Makefile
	@echo "[CC]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CPPFLAGS) $(L_INC) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.cpp Makefile
	@echo "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(L_INC) -o "$@" -c "$<"

$(TARGET).elf: $(OBJS) $(LDSCRIPT) Makefile
	@echo "[LD]\t$@"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS)

%.hex: %.elf Makefile
	@echo "[HEX]\t$@"
	@$(SIZE) "$<"
	@$(OBJCOPY) -O ihex -R .eeprom "$<" "$@"

thconf: utilities/thconf.cpp Makefile
	@echo "[CXX]\t$<"
	@g++ $(CXXFLAGS) -o "$@" "$<"

# Compiler generated dependency info
-include $(OBJS:.o=.d)

# Update the initialize.h according to the INITIALIZE option
ifeq ($(shell grep $(INITIALIZE) $(PROGRAM)/initialize.h), )
    $(shell sed -i s/[01]/$(INITIALIZE)/ $(PROGRAM)/initialize.h)
endif

clean:
	@echo Cleaning...
	@rm -rf "$(BUILDDIR)"
	@rm -f "$(TARGET).elf" "$(TARGET).hex" thconf

###-----------------------------------------------------------------------------
