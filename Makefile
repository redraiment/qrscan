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

CFLAGS += -I$(QUIRC) $(shell $(PKG_CONFIG) --cflags --static libpng libjpeg)
LDFLAGS += -lz -lm
ifeq ($(shell uname -s),Darwin)
	GROUP = staff
  LDFLAGS += $(shell $(PKG_CONFIG) --variable=libdir --static libpng)/libpng.a
  LDFLAGS += $(shell $(PKG_CONFIG) --variable=libdir --static libjpeg)/libjpeg.a
else
	GROUP = root
  LDFLAGS += -static $(shell $(PKG_CONFIG) --libs --static libpng libjpeg)
endif

.PHONY: all install uninstall clean

all: $(OBJS)
	$(CC) $^ $(LDFLAGS) -O2 -Wall -fPIC -o $(TARGET)

.c.o:
	$(CC) $< $(CFLAGS) -c -o $@

install: all
	install -o root -g $(GROUP) -m 0755 $(TARGET) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
