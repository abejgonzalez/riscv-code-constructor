# RISC-V Baremetal Constructor

This repository is a set of files needed to construct C baremetal RISC-V binaries. This can
run on things like the [Rocket-Chip](https://github.com/freechipsproject/rocket-chip)
and [BOOM](https://boom-core.org) core. 

## Repository Layout

- `src`: Source files (\*.c) needed to build a binary
- `inc`: Header files (\*.h) needed to build a binary
- `link`: Linker file
- `Makefile`: Automate building a binary/dump files
- `dump`: Location of dump files
- `bin`: Location of binary files

## How to use

Simpliest way to add your own \*.c file is to modify the `Makefile` by the following:

    PROGRAMS=<LIST YOUR C FILES HERE WITHOUT THE .C EXTENSION>

Then you can run the following to build the files into binaries:

    `make all`

Note: You must have [riscv-tools](https://github.com/riscv/riscv-tools) installed and on
your path.
