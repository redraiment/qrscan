#include "png_reader.h"

#define RED_COEFFICIENT 0.299
#define GREEN_COEFFICIENT 0.587

PngReader png_reader_new(File file) {
    PngReader this = (PngReader) malloc(sizeof (struct _PngReader));
    this->png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    this->info = png_create_info_struct(this->png);
    png_init_io(this->png, file);
    png_read_info(this->png, this->info);

    png_byte color_type = png_get_color_type(this->png, this->info);
    png_byte bit_depth = png_get_bit_depth(this->png, this->info);

    if (color_type | PNG_COLOR_MASK_ALPHA) {
        png_set_strip_alpha(this->png);
    }
    if (color_type | PNG_COLOR_MASK_PALETTE) {
        png_set_palette_to_rgb(this->png);
    }
    if (color_type | PNG_COLOR_MASK_COLOR) {
        png_set_rgb_to_gray(this->png, PNG_ERROR_ACTION_NONE, RED_COEFFICIENT,GREEN_COEFFICIENT);
    }
    if (bit_depth != 8) {
        png_set_expand_gray_1_2_4_to_8(this->png);
    }
    png_read_update_info(this->png, this->info);

    this->width = png_get_image_width(this->png, this->info);
    this->height = png_get_image_height(this->png, this->info);
    this->pointers = (png_bytepp) calloc(sizeof(png_bytep), this->height);

    return this;
}

void png_reader_delete(PngReader this) {
    if (this != NULL) {
        if (this->pointers != NULL) {
            free(this->pointers);
        }
        if (this->png != NULL && this->info != NULL) {
            png_destroy_read_struct(&this->png, &this->info, NULL);
        }
        free(this);
    }
}

void png_reader_read(PngReader reader, ByteArray buffer) {
    for (int y = 0; y < reader->height; y++) {
        reader->pointers[y] = buffer + y * reader->width;
    }
    png_read_image(reader->png, reader->pointers);
}
