# the compiler
CC = g++

# compiler flags:
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -Wall

# the build target executable:
TARGET = client

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp -pthread

clean:
	$(RM) $(TARGET)
