# Folders
SRC:=src
INC:=inc
LNK:=link
OBJ:=obj
BIN:=bin
DMP:=dump
DEP:=dep

TAPEOUT_FLAGS?= -D REFCLK=250000000UL -D DEBUG
#TAPEOUT_FLAGS= -D REFCLK=10000000UL

# Commands and flags
CC:=riscv64-unknown-elf-gcc
OBJDUMP:=riscv64-unknown-elf-objdump -S
OBJCOPY := riscv64-unknown-elf-objcopy
CFLAGS=$(TAPEOUT_FLAGS) -mcmodel=medany -march=rv64gc -std=c11 -Os -g -Og -fno-common -fno-builtin-printf -Wall -I$(INC) -Wno-unused-function -Wno-unused-variable -Wunused-but-set-variable
LDFLAGS:=-static -nostdlib -nostartfiles
RM=rm -rf

NAME?=beagleTest

SRCS := $(SRC)/crt.S $(SRC)/syscalls.c $(SRC)/beagleTest.c $(SRC)/sd.c $(SRC)/gpt.c $(SRC)/tty.c
LDSCRIPT := $(LNK)/link.ld

.PHONY: all
all: $(BIN)/$(NAME).riscv $(DMP)/$(NAME).dump

$(BIN)/$(NAME).riscv: $(SRCS) $(HDRS) $(LDSCRIPT)
	@mkdir -p $(BIN)
	$(CC) -T $(LDSCRIPT) $(LDFLAGS) $(CFLAGS) $(SRCS) -o $@

$(DMP)/%.dump: $(BIN)/%.riscv
	@mkdir -p $(DMP)
	$(OBJDUMP) -D $< > $@

.PHONY: clean
clean:
	$(RM) $(BIN) $(DMP)
