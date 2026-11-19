CC = gcc
CFLAGS = -Wall
SRC = $(wildcard src/*.c)
TARGET = sh

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: clean
