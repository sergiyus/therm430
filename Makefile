MCU=msp430f2003
OBJCOPY=msp430-objcopy
CC=msp430-gcc
SIZE=msp430-size
CFLAGS=-mmcu=$(MCU) -DHARDWARE -ggdb -Os -std=c99 -Wall -Wextra -Werror #-Wsign-conversion
LDFLAGS=-mmcu=$(MCU)
OBJ=main.o sd16a.o 7seg_lcd.o logic.o
TARGET=therm430
.PHONY: all clean

%.o:	%.c
	$(CC) -c -o $@ $< $(CFLAGS)

all:	$(OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET).elf $(OBJ)
	$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	$(SIZE) $(TARGET).elf

install:
	@mspdebug -n rf2500 "prog $(TARGET).elf"

clean:
	@rm -v *.elf *.hex $(OBJ)
