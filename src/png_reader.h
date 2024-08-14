#ifndef QRSCAN_PNG_READER_H
#define QRSCAN_PNG_READER_H

#include <png.h>

#include "helpers.h"

typedef struct _PngReader {
    png_structp png;
    png_infop info;
    png_uint_32 width;
    png_uint_32 height;
    png_bytepp pointers;
} * PngReader;

PngReader png_reader_new(File file);
void png_reader_delete(PngReader this);
void png_reader_read(PngReader this, ByteArray buffer);

#endif //QRSCAN_PNG_READER_H
