#!/bin/bash

TARGET=vainfo
LD_FLAGS=-lva -lX11 -lva-x11
CFLAGS=-g -Wall
CC=gcc

$(TARGET):vainfo.o
	$(CC) -o $(TARGET) $(LD_FLAGS) vainfo.o

*.o:*.c
	$(CC) -c $(C_FLAGS) $< $@

clean:
	rm -rf *.o $(TARGET)

