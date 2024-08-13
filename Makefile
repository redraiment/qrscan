# qrscan -- QR-Code Scanner
# Copyright (C) 2024 Zhang, Zepeng <redraiment@gmail.com>

CC ?= gcc
PREFIX ?= /usr/local

QUIRC = quirc/lib

CFLAGS ?= -c -I$(QUIRC)
LDFLAGS ?= -lpng -lm -O2 -Wall -fPIC

OBJS = \
	$(QUIRC)/decode.o \
	$(QUIRC)/identify.o \
	$(QUIRC)/quirc.o \
	$(QUIRC)/version_db.o \
	src/main.o
TARGET = qrscan

.PHONY: all install uninstall clean

all: $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $(TARGET)

.c.o:
	$(CC) $< $(CFLAGS) -o $@

install:
	install -o root -g root -m 0755 $(TARGET) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

clean:
	rm -f $(OBJS)
	rm -f qrscan
