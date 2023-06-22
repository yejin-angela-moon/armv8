# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lSDL2

# Source files
SRCS = iq_fit.c board.c piece.c utilities.c
OBJS = $(SRCS:.c=.o)

# Executable
TARGET = game

# Build rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Object file rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

# Default target
all: clean $(TARGET)
