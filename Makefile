MCU=msp430f2003
OBJCOPY=/home/mb/ti/energia-1.8.10E23/hardware/tools/msp430/bin/msp430-objcopy
CC=/home/mb/ti/energia-1.8.10E23/hardware/tools/msp430/bin/msp430-gcc
CFLAGS=-mmcu=$(MCU) -DHARDWARE -ggdb -Os  -Wall -Wextra -Werror
LDFLAGS=-mmcu=$(MCU)
OBJ=main.o ports.o clocks.o timer.o sd16a.o 7seg_lcd.o
TARGET=therm430
.PHONY: all clean

%.o:	%.c
	$(CC)  -c -o $@ $< $(CFLAGS)
all:	$(OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET).elf  $(OBJ)
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
install:
	@mspdebug -n rf2500 "prog $(TARGET).elf"
clean:
	@rm -v *.elf *.hex $(OBJ)
