CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
emulate: emulate.o emulate_files/B.o emulate_files/DPImm.o emulate_files/DPReg.o emulate_files/ioutils.o emulate_files/SDT.o emulate_files/utilities.o

clean:
	$(RM) *.o assemble emulate
	

