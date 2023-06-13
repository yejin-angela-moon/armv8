CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o assembler_files/B.o assembler_files/DP.o assembler_files/SDT.o assembler_files/ioutils.o assembler_files/utilities.o
emulate: emulate.o emulate_files/B.o emulate_files/DPImm.o emulate_files/DPReg.o emuate_files/ioutils.o emulate_files/SDT.o emulate_files/utilities.o

clean:
	$(RM) *.o assemble emulate
	
