#ifndef QRSCAN_JPG_READER_H
#define QRSCAN_JPG_READER_H

#include "helpers.h"
#include <jpeglib.h>

typedef struct _JpgReader {
    j_decompress_ptr jpg;
    struct jpeg_error_mgr* error;
    JDIMENSION width;
    JDIMENSION height;
} * JpgReader;

JpgReader jpg_reader_new(File file);
void jpg_reader_delete(JpgReader this);
void jpg_reader_read(JpgReader this, ByteArray buffer);

#endif //QRSCAN_JPG_READER_H
