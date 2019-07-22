###############################################################################
# Makefile for building baremetal C RISC-V executables
###############################################################################

# Folders
SRC:=src
INC:=inc
LNK:=link
OBJ:=obj
BIN:=bin
DMP:=dump
DEP:=dep

# Commands and flags
CC:=riscv64-unknown-elf-gcc
OBJDUMP:=riscv64-unknown-elf-objdump -S
CFLAGS=-mcmodel=medany -march=rv64gc -l -std=gnu99 -O0 -g -Og -fno-common -fno-builtin-printf -Wall -I$(INC) -Wno-unused-function -Wno-unused-variable
LDFLAGS:=-static -nostdlib -nostartfiles -lgcc
DEPFLAGS=-MT $@ -MMD -MP -MF $(DEP)/$*.d
RM=rm -rf

# Programs to compile
ALL_SRCS=$(notdir $(basename $(wildcard src/*))) # All src files in src
NON_USER_SRCS=crt syscalls # Included but not compiled srcs
PROGRAMS=$(filter-out $(NON_USER_SRCS),$(ALL_SRCS)) # Only compile srcs wanted

BINS=$(addprefix $(BIN)/,$(addsuffix .riscv,$(PROGRAMS)))
DUMPS=$(addprefix $(DMP)/,$(addsuffix .dump,$(PROGRAMS)))

.PHONY: all # all is not a file
all: $(BINS) $(DUMPS)

dumps: $(DUMPS)

# Build executable
$(BIN)/%.riscv: $(OBJ)/%.o $(OBJ)/crt.o $(OBJ)/syscalls.o $(LNK)/link.ld
	@mkdir -p $(BIN)
	$(CC) -T $(LNK)/link.ld $(DEPFLAGS) $(LDFLAGS) $< $(OBJ)/crt.o $(OBJ)/syscalls.o -o $@

# Build dump
$(DMP)/%.dump: $(BIN)/%.riscv
	@mkdir -p $(DMP)
	$(OBJDUMP) -D $< > $@

# Build object files
$(OBJ)/%.o: $(SRC)/%.S
	@mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -D__ASSEMBLY__=1 -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(OBJ)
	@mkdir -p $(DEP)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# Keep the temporary .o files
.PRECIOUS: $(OBJ)/%.o

# Remove all generated files
.PHONY: clean # clean is not a file
clean:
	$(RM) $(BIN) $(OBJ) $(DMP) $(DEP)

# Include dependencies
-include $(addprefix $(DEP)/,$(addsuffix .d,$(PROGRAMS)))
