# Created by: WestleyR
# email: westleyr@nym.hush.com
# Date: Nov 9, 2018
# https://github.com/WestleyR/pac
# Version-1.0.0
#
# The Clear BSD License
#
# Copyright (c) 2019 WestleyR
# All rights reserved.
#
# This software is licensed under a Clear BSD License.
#

TARGET = pac

PREFIX = /usr/local

CC = gcc

CFLAGS = -Wall

SRC = src/main-pac.c

.PHONY:
all: $(TARGET)

.PHONY:
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

.PHONY:
static: $(SRC)
	$(CC) -static $(CFLAGS) $(SRC) -o $(TARGET)

.PHONY:
install: $(TARGET)
	mkdir -p $(PREFIX)/bin
	cp -f $(TARGET) $(PREFIX)/bin

.PHONY:
clean:
	rm -f $(TARGET)

.PHONY:
uninstall: $(PREFIX)/bin/$(TARGET)
	rm -f $(PREFIX)/bin/$(TARGET)

