#----------------------------------------------------------------------------
# Template written by Fernando Nóbel Santos Navarro, UPV.
#
# Released to the Public Domain
#----------------------------------------------------------------------------
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make program = Download the hex file to the device, using avrdude in the BBB.
#
# make monitor = Open a serial monitor with screen in the BBB.
#
# make dir = Create the project folder tree.
#                
# To rebuild project do "make clean" then "make all".
#----------------------------------------------------------------------------

# Target .hex file name (without extension).
TARGET = main

#---------------- AVR Options -----------------------------------------------

# Microcontroller name.
MCU = atmega328p

# Processor frequency.
F_CPU = 8000000

#---------------- Define commands and programs ------------------------------

# Programs.
CC = avr-gcc
CPP = avr-g++
AR = avr-ar
OBJ_COPY = avr-objcopy

#---------------- Compiler Options C++ --------------------------------------

# Compile flags.
GENERAL_FLAGS = -c -g -Os -Wall -ffunction-sections -fdata-sections -mmcu=$(MCU) -DF_CPU=$(F_CPU)L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=106
CPP_FLAGS = $(GENERAL_FLAGS) -fno-exceptions -std=c++11
CC_FLAGS  = $(GENERAL_FLAGS)

# Source code.
SRC = $(wildcard src/*.c) $(wildcard lib/*.c)
CPPSRC = $(wildcard src/*.cpp) $(wildcard lib/*.cpp)

# Object list.
OBJ = $(addprefix build/, $(CPPSRC:.cpp=.o)) $(addprefix build/, $(SRC:.c=.o))

# Dependencies list.
DEP = $(OBJ:.o=.d)

# Where to search the .h files.
INCLUDE_FILES = -I src -I lib

#----------------- BBB options and commands ---------------------------------

# Digtial pin used to reset the avr (gpio49 = P9_23).
RST_PIN = 49

# Serial line used to program the BBB (UART2 -> RX = P9_22 TX = P9_21 /dev/ttyO2).
UART = BB-UART2
UART_DEV = /dev/ttyO2

# Configure BBB command.
CONF_BBB  = "echo $(UART) > /sys/devices/platform/bone_capemgr/slots" 
CONF_BBB += "&& echo $(RST_PIN) > /sys/class/gpio/export"
CONF_BBB += "&& echo "out" > /sys/class/gpio/gpio$(RST_PIN)/direction"
CONF_BBB += "&& echo 1 > /sys/class/gpio/gpio$(RST_PIN)/value"

# Reset the AVR.
RST_BBB = "echo 0 > /sys/class/gpio/gpio$(RST_PIN)/value"
RST_BBB += "&& sleep 0.1"
RST_BBB += "&& echo 1 > /sys/class/gpio/gpio$(RST_PIN)/value"

# Program the AVR using avrdude in the BBB.
PROG_BBB  = $(RST_BBB)
PROG_BBB += "&& avrdude -c arduino -p m328p -v -b 57600 -P $(UART_DEV) -U flash:w:/root/$(TARGET).hex"

# Serial monitor options.
# Usually 300, 1200, 9600 or 19200. This affects transmission as well as receive speed.
BAUD_RATE = 9600

# PWM Init command

PWM_BBB  = "echo BB-PWM1 > /sys/devices/platform/bone_capemgr/slots"
PWM_BBB += "&& cd /sys/class/pwm"
PWM_BBB += "&& cd pwmchip0"
PWM_BBB += "&& echo 0 > export"
PWM_BBB += "&& cd pwm0"
PWM_BBB += "&& echo 1000000000 > period"
PWM_BBB += "&& echo 800000000 > duty_cycle"
PWM_BBB += "&& echo 1 > enable"

# PWM Options command

PWM_OP_BBB  = "cd /sys/class/pwm"
PWM_OP_BBB += "&& cd pwmchip0"
PWM_OP_BBB += "&& cd pwm0"
PWM_OP_BBB += "&& echo 800000 > duty_cycle"
PWM_OP_BBB += "&& echo 1000000 > period"


#============================================================================

# Compile the code.
all: hex 
program: uploadBBB programBBB cleanBBB
monitor: screenBBB
run: all program monitor

# Make the directory structe for the project.
dir:
	mkdir -p bin
	mkdir -p build
	mkdir -p build/lib
	mkdir -p build/src
	mkdir -p doc
	mkdir -p scripts
	mkdir -p lib
	mkdir -p src
	
# Actualize the Makefile to the latest version of GitHub
actualize:
	wget https://raw.githubusercontent.com/FernandoNobel/ATmega328-HelloWorld/master/Makefile -O ./Makefile

#---------------- Build Rules -----------------------------------------------

# pull in dependency info for *existing* .o files
-include $(DEP)

# Rule for buildind objects from src directory.
build/src/%.o: src/%.c
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $< -o $@

build/src/%.o: src/%.cpp
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $< -o $@

# Rule for buildind objects from lib directory.
build/lib/%.o: lib/%.c
	$(CC) $(CC_FLAGS) $(INCLUDE_FILES) $< -o $@

build/lib/%.o: lib/%.cpp
	$(CPP) $(CPP_FLAGS) $(INCLUDE_FILES) $< -o $@

# Make the .hex file.
hex: $(OBJ)
	$(CC) -Os -Wl,--gc-sections -mmcu=$(MCU) -o bin/$(TARGET).elf $(OBJ) -lm 
	$(OBJ_COPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 bin/$(TARGET).elf bin/$(TARGET).eep 
	$(OBJ_COPY) -O ihex -R .eeprom bin/$(TARGET).elf bin/$(TARGET).hex 

# Clean all generated files during compilation.
clean:
	rm -f bin/*
	rm -f build/src/*
	rm -f build/lib/*

#---------------- BBB functions ---------------------------------------------

# Upload the .hex file to the BBB.
uploadBBB:
	sshpass -p 1234 scp -q bin/$(TARGET).hex root@192.168.7.2:/root/$(TARGET).hex

# Reset the AVR
resestBBB: 
	sshpass -p 1234 ssh -q root@192.168.7.2 $(RST_BBB)
# Program the AVR with the BBB.
programBBB:
	sshpass -p 1234 ssh -q root@192.168.7.2 $(PROG_BBB)

# Clean all files passed to the BBB.
cleanBBB:
	sshpass -p 1234 ssh -q root@192.168.7.2 "rm /root/$(TARGET).hex"	

# Configure the overlays of the BBB for programming the AVR. 
configureBBB:
	sshpass -p 1234 ssh -q root@192.168.7.2 $(CONF_BBB)

# Serial monitor to communicate with the ATmega.
screenBBB:
	sshpass -p 1234 ssh -qt root@192.168.7.2 "picocom -b $(BAUD_RATE) -r -l $(UART_DEV) --echo --omap crlf --imap lfcrlf"

# Shutdown properly the BBB.
shutdownBBB:
	sshpass -p 1234 ssh -qt root@192.168.7.2 "shutdown -h now & exit"

# Confugre pin P9_14 for pwm.
pwmInitBBB:
	sshpass -p 1234 ssh -q root@192.168.7.2 $(PWM_BBB)

pwmOptionsBBB:
	sshpass -p 1234 ssh -q root@192.168.7.2 $(PWM_OP_BBB)
