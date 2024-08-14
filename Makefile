# qrscan -- QR-Code Scanner
# Copyright (C) 2024 Zhang, Zepeng <redraiment@gmail.com>

PKG_CONFIG := $(shell which pkg-config)
ifeq ($(PKG_CONFIG),)
  $(error pkg-config not installed)
endif

ifneq ($(shell $(PKG_CONFIG) --exists libpng && echo $$?),0)
  $(error libpng not found)
endif

ifneq ($(shell $(PKG_CONFIG) --exists libjpeg && echo $$?),0)
  $(error libjpeg not found)
endif

CC ?= gcc
PREFIX ?= /usr/local
CFLAGS ?=
LIBS ?=

QUIRC = quirc/lib
OBJS = \
	$(QUIRC)/decode.o \
	$(QUIRC)/identify.o \
	$(QUIRC)/quirc.o \
	$(QUIRC)/version_db.o \
	src/main.o \
	src/options.o \
	src/qrcode.o \
	src/image.o \
	src/png_reader.o \
	src/jpg_reader.o
TARGET = qrscan

OS := $(shell uname -s)
ifeq ($(OS),Darwin)
	GROUP = staff
  LDFLAGS =
else
	GROUP = root
  LDFLAGS = -static
endif

.PHONY: all install uninstall clean

all: $(OBJS)
	$(CC) $^ $(LIBS) $(LDFLAGS) $(shell $(PKG_CONFIG) --libs --static libpng libjpeg) -lm -O2 -Wall -fPIC -o $(TARGET)

.c.o:
	$(CC) $< $(CFLAGS) $(shell $(PKG_CONFIG) --cflags --static libpng libjpeg) -c -I$(QUIRC) -o $@

install: all
	install -o root -g $(GROUP) -m 0755 $(TARGET) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
