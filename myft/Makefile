SRC = ./main.c ./myft.c

BIN = ./myft

CLEAN_TARGET = *.o

CC := gcc
RM = rm -rf
CFLAGS = -Wall -O2 -I ./ -o
LDFLAGS =
LDSHFLAGS =

#############################
	#$(CC) $(CFLAGS) $(BIN) $(SRC)

.PHONY: all clean

all:
	gcc myft.c main.c -o myft

clean:
	$(RM) $(CLEAN_TARGET)


