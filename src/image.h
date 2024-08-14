#ifndef QRSCAN_IMAGE_H
#define QRSCAN_IMAGE_H

#include "helpers.h"

typedef enum {
    IMAGE_FORMAT_PNG = 0,
    IMAGE_FORMAT_JPG = 1
} ImageFormat;

typedef struct _Image {
    ImageFormat format;
    File file;
    Any reader;
    int width;
    int height;
} * Image;

Image image_new(String filename, ImageFormat format);
void image_delete(Image this);
void image_read(Image this, ByteArray buffer);

#endif //QRSCAN_IMAGE_H
