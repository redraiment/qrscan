# qrscan -- QR-Code Scanner
# Copyright (C) 2024 Zhang, Zepeng <redraiment@gmail.com>

CC ?= gcc
PREFIX ?= /usr/local
CFLAGS ?=
LDFLAGS ?=

QUIRC = quirc/lib
OBJS = \
	$(QUIRC)/decode.o \
	$(QUIRC)/identify.o \
	$(QUIRC)/quirc.o \
	$(QUIRC)/version_db.o \
	src/main.o
TARGET = qrscan

.PHONY: all install uninstall clean

all: $(OBJS)
	$(CC) $^ $(LDFLAGS) -lpng -lm -O2 -Wall -fPIC -o $(TARGET)

.c.o:
	$(CC) $< $(CFLAGS) -c -I$(QUIRC) -o $@

install:
	install -o root -g root -m 0755 $(TARGET) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

clean:
	rm -f $(OBJS)
	rm -f qrscan
