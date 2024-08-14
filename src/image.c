#include <string.h>
#include <stdio.h>

#include "helpers.h"
#include "image.h"
#include "png_reader.h"
#include "jpg_reader.h"

Image image_new(String filename, ImageFormat format) {
    Image image = (Image) malloc(sizeof (struct _Image));
    image->format = format;

    image->file = strcmp(filename, "-") ? fopen(filename, "rb") : stdin;
    if (image->file == NULL) {
        panic("Cannot open %s\n", filename);
    }

    switch (format) {
        case IMAGE_FORMAT_PNG: {
            PngReader reader =  png_reader_new(image->file);
            image->width = reader->width;
            image->height = reader->height;
            image->reader = reader;
        } break;
        case IMAGE_FORMAT_JPG:{
            JpgReader reader =  jpg_reader_new(image->file);
            image->width = reader->width;
            image->height = reader->height;
            image->reader = reader;
        } break;
    }

    return image;
}

void image_delete(Image this) {
    if (this != NULL) {
        switch (this->format) {
            case IMAGE_FORMAT_PNG:
                png_reader_delete((PngReader) this->reader);
                break;
            case IMAGE_FORMAT_JPG:
                jpg_reader_delete((JpgReader) this->reader);
                break;
        }
        fclose(this->file);
        free(this);
    }
}

void image_read(Image this, ByteArray buffer) {
    switch (this->format) {
        case IMAGE_FORMAT_PNG:
            png_reader_read((PngReader) this->reader, buffer);
            break;
        case IMAGE_FORMAT_JPG:
            jpg_reader_read((JpgReader) this->reader, buffer);
            break;
    }
}
