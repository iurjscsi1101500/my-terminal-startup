CC = gcc
CFLAGS = -Wall -Wextra -Werror -march=native -fstack-protector-strong -D_FORTIFY_SOURCE=2 -fPIE -std=c23 -O3

TARGET = main

SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

