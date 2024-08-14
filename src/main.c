#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <png.h>

#include "helpers.h"
#include "options.h"
#include "qrcode.h"

#define PNG_RGB_TO_GRAY_RED_COEFFICIENT 0.299
#define PNG_RGB_TO_GRAY_GREEN_COEFFICIENT 0.587

typedef struct _Image {
    String filename;
    File file;
    png_structp png;
    png_infop info;

    png_uint_32 width;
    png_uint_32 height;
    png_byte color_type;
    png_byte bit_depth;

    png_bytepp pointers;
} *Image;


Image Image_new(String filename) {
    Image this = (Image) calloc(sizeof(struct _Image), 1);
    this->filename = filename;
    this->file = strcmp(filename, "-") ? fopen(filename, "rb") : stdin;
    if (this->file == NULL) {
        panic("Cannot open %s\n", this->filename);
    }

    this->png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    this->info = png_create_info_struct(this->png);
    png_init_io(this->png, this->file);
    png_read_info(this->png, this->info);

    png_uint_32 width = png_get_image_width(this->png, this->info);
    png_uint_32 height = png_get_image_height(this->png, this->info);
    png_byte color_type = png_get_color_type(this->png, this->info);
    png_byte bit_depth = png_get_bit_depth(this->png, this->info);

    if (color_type | PNG_COLOR_MASK_ALPHA) {
        png_set_strip_alpha(this->png);
    }
    if (color_type | PNG_COLOR_MASK_PALETTE) {
        png_set_palette_to_rgb(this->png);
    }
    if (color_type | PNG_COLOR_MASK_COLOR) {
        png_set_rgb_to_gray(this->png, PNG_ERROR_ACTION_NONE, PNG_RGB_TO_GRAY_RED_COEFFICIENT,
                            PNG_RGB_TO_GRAY_GREEN_COEFFICIENT);
    }
    if (bit_depth != 8) {
        png_set_expand_gray_1_2_4_to_8(this->png);
    }
    png_read_update_info(this->png, this->info);

    this->width = png_get_image_width(this->png, this->info);
    this->height = png_get_image_height(this->png, this->info);
    this->color_type = png_get_color_type(this->png, this->info);
    this->bit_depth = png_get_bit_depth(this->png, this->info);
    this->pointers = (png_bytepp) calloc(sizeof(png_bytep), this->height);

    return this;
}

void Image_delete(Image this) {
    if (this != NULL) {
        if (this->pointers != NULL) {
            free(this->pointers);
        }
        if (this->png != NULL && this->info != NULL) {
            png_destroy_read_struct(&this->png, &this->info, NULL);
        }
        if (this->file != NULL) {
            fclose(this->file);
        }
        free(this);
    }
}

int main(int argc, String argv[]) {
    Options options = options_new(argc, argv);
    Image image = Image_new(options->filename);
    QRCode qrcode = qrcode_new();

    ByteArray pixels = qrcode_alloc(qrcode, image->width, image->height);
    for (int y = 0; y < image->height; y++) {
        image->pointers[y] = pixels + y * image->width;
    }
    png_read_image(image->png, image->pointers);

    qrcode_scan(qrcode);

    qrcode_delete(qrcode);
    Image_delete(image);
    options_delete(options);

    return EXIT_SUCCESS;
}
