# Compiler and flags
CC = gcc
CFLAGS = -g

# Target executable
TARGET = shell

# Source and object files
SRCS = main.c
OBJS = shell.o

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
shell.o: main.c shell.h
	$(CC) $(CFLAGS) -c main.c -o shell.o

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the shell (optional)
run: all
	./$(TARGET)
