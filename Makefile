###############################################################################
# MCU and TARGET name
MCU         := msp430f2013
TARGET      := therm430
# MODE: debug or release
MODE        :=
# COMPILER: energia or gcc
COMPILER    := gcc
# Choose the MSP430 Debug Probe: eZ430 or eZ-FET
DBG_PROBE   := eZ-FET

###############################################################################
# Toolchain
###############################################################################
ifeq ($(COMPILER), energia)
# TI compiler from energia.nu
CC          := msp430-gcc
SIZE        := msp430-size
GDB         := msp430-gdb
OBJCOPY     := msp430-objcopy
OBJDUMP     := msp430-objdump
NM          := msp430-nm
READELF     := msp430-readelf
ADDR2LINE   := msp430-addr2line
else ifeq ($(COMPILER), gcc)
# MSP430-GCC-OPENSOURCE
CC          := msp430-elf-gcc
SIZE        := msp430-elf-size
GDB         := msp430-elf-gdb
OBJCOPY     := msp430-elf-objcopy
OBJDUMP     := msp430-elf-objdump
NM          := msp430-elf-nm
READELF     := msp430-elf-readelf
ADDR2LINE   := msp430-elf-addr2line
else
$(error Invalid msp430 compiler selected. COMPILER must be either 'energia' or 'gcc'.\
	Please set it in the Makefile like this: COMPILER := gcc)
endif

ifeq ($(DBG_PROBE), eZ-FET)
# TI MSP430 library
DEBUG       := mspdebug tilib
else ifeq ($(DBG_PROBE), eZ430)
# eZ430-RF2500 devices
DEBUG       := mspdebug rf2500
else
$(error Invalid MSP Debug Probe selected. DBG_PROBE must be either 'eZ430' or 'eZ-FET'.\
	Please set it in the Makefile like this: DBG_PROBE := eZ430)
endif

FLASHER     := MSP430Flasher

###############################################################################
# Directories
###############################################################################
# Path to the msp430 toolchain
ifeq ($(COMPILER), energia)
TOOLCHAIN_ROOT ?= $HOME/.local/msp430/bin

else ifeq ($(COMPILER), gcc)
TOOLCHAIN_ROOT ?=/opt/ti

MSPGCC_INCL_DIR	:= $(TOOLCHAIN_ROOT)/include
LIB_DIRS    := $(MSPGCC_INCL_DIR)
INCLUDE_DIRS:= $(MSPGCC_INCL_DIR)

else
$(error Invalid msp430 path toolchain)
endif

# Directories
BUILD_DIR   := build
OBJ_DIR     := $(BUILD_DIR)
BIN_DIR     := bin
SRC_DIR     := src
INC_DIR     := include

ifeq ("$(wildcard $(SRC_DIR))", "")
$(error Invalid include directory: $(SRC_DIR) does not exist)
endif

ifeq ("$(wildcard $(INC_DIR))", "")
$(error Invalid include directory: $(INC_DIR) does not exist)
endif

###############################################################################
# Files
###############################################################################
# Source files
SRCS    := $(wildcard $(SRC_DIR)/*.c)
HEADERS := $(wildcard $(INC_DIR)/*.h)

# Object files
OBJS    := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

vpath %.c $(sort $(dir $(SRCS)))
vpath %.h $(sort $(dir $(HEADERS)))

# ELF file output
ELF     := $(BIN_DIR)/$(TARGET).elf

###############################################################################
# Flags
###############################################################################
WFLAGS  := -Wall -Wextra -Werror -Wshadow -Wfloat-equal -Warray-bounds
WFLAGS  += -Wdiv-by-zero -Wdouble-promotion -Wpadded#-Wbool-compare
WFLAGS  += -Wundef -Wunreachable-code -Wconversion -Wsign-conversion
WFLAGS  += -fshort-enums -pedantic -std=c99

ifeq ($(MODE),debug)
OFLAGS  := -g3 -ggdb -O0 -gdwarf-2 -DDEBUG
else
OFLAGS  := -Os -flto
endif

CFLAGS  := -mmcu=$(MCU) $(WFLAGS) $(OFLAGS)

ifeq ($(COMPILER), energia)
CFLAGS	+= -I$(INC_DIR) -MMD -MP
LDFLAGS	:= -mmcu=$(MCU)

else ifeq ($(COMPILER), gcc)
CFLAGS  += $(addprefix -I, $(INCLUDE_DIRS)) -I$(INC_DIR) -MMD -MP

LDFLAGS := -mmcu=$(MCU) $(addprefix -L, $(LIB_DIRS))
LDFLAGS += $(addprefix -I, $(INCLUDE_DIRS))

else
	$(error Invalid CFLAGS set)
endif

# Dependancies
DEPS    := $(OBJS:.o=.d)

###############################################################################
# Build
###############################################################################

# Rules
.PHONY: all convert flash objdump disass symbols nm size mode read clean check help

all: tags $(ELF) size mode

$(ELF) : $(OBJS) Makefile | $(BIN_DIR)
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	@echo

$(OBJ_DIR)/%.o : %.c Makefile | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	@mkdir -p $@

$(BUILD_DIR):
	@mkdir -p $@

-include $(DEPS)

# ctags
tags: $(SRCS) $(HEADERS)
	ctags -R $(SRCS) $(HEADERS)

# Print debug mode information at the beginning
mode:
ifeq ($(MODE), debug)
	$(info )
	$(info *** Built in DEBUG mode ***)
endif

run: all
	@$(DEBUG) "prog $(ELF)"

gdb: all
	@$(DEBUG)

convert: $(ELF)
	@$(OBJCOPY) -O ihex $(ELF) $(BIN_DIR)/$(TARGET).hex

flash: convert
	@$(FLASHER) -w $(BIN_DIR)/$(TARGET).hex -v -z [VCC]

objdump:
	@$(OBJDUMP) -h $(ELF) | less

disass:
	@$(OBJDUMP) -S $(ELF) | less

symbols:
	@$(READELF) -S $(ELF) | sort -n -k3

nm:
	@$(NM) -n $(ELF) | less

size:
	@$(SIZE) $(ELF)

read:
	@$(FLASHER) -n $(MCU) -r [$(BIN_DIR)/$(TARGET)_read.hex,MAIN]

clean:
	@rm -rf tags $(BUILD_DIR) $(BIN_DIR)

cppcheck:
	cppcheck --enable=all --error-exitcode=1 \
		 --inline-suppr \
		 --suppress=missingIncludeSystem \
		 --suppress=unmatchedSuppression \
		 --suppress=unusedFunction $(SRCS) $(INC_DIR)

help:
	@echo "make           - Build the project"
	@echo "make help      - Show this help message"
	@echo "make run       - Build and flash the project with mspdebug"
	@echo "make gdb       - Debug the project with mspdebug"
	@echo "make convert   - Convert ELF to HEX file"
	@echo "make flash     - Flash the project with MSP430Flasher"
	@echo "make objdump   - Inspect object files"
	@echo "make disass    - Disassemble the $(TARGET) binary"
	@echo "make symbols   - View the symbols in the ELF file"
	@echo "make nm        - List the symbols from $(TARGET)"
	@echo "make size      - Show the size of the $(TARGET) binary"
	@echo "make read      - Read flash memory with MSP430Flasher"
	@echo "make clean     - Clean up build files"
	@echo "make cppcheck  - Check code with cppcheck"

