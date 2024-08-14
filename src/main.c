#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <png.h>
#include <quirc.h>

#define echo(...) fprintf(stderr, __VA_ARGS__)
#define panic(...) do { \
    echo(__VA_ARGS__);                          \
    exit(EXIT_FAILURE);                         \
  } while(0)
#define PNG_RGB_TO_GRAY_RED_COEFFICIENT 0.299
#define PNG_RGB_TO_GRAY_GREEN_COEFFICIENT 0.587

typedef char* String;
typedef FILE* File;

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
} * Image;

typedef struct quirc* QRCode;

Image Image_new(String filename) {
  Image this = (Image)calloc(sizeof(struct _Image), 1);
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
    png_set_rgb_to_gray(this->png, PNG_ERROR_ACTION_NONE, PNG_RGB_TO_GRAY_RED_COEFFICIENT, PNG_RGB_TO_GRAY_GREEN_COEFFICIENT);
  }
  if (bit_depth != 8) {
    png_set_expand_gray_1_2_4_to_8(this->png);
  }
  png_read_update_info(this->png, this->info);

  this->width = png_get_image_width(this->png, this->info);
  this->height = png_get_image_height(this->png, this->info);
  this->color_type = png_get_color_type(this->png, this->info);
  this->bit_depth = png_get_bit_depth(this->png, this->info);
  this->pointers = (png_bytepp)calloc(sizeof(png_bytep), this->height);

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

QRCode QRCode_new(Image image) {
  QRCode this = quirc_new();

  if (quirc_resize(this, image->width, image->height) < 0) {
    panic("Failed to allocate video memory\n");
  }

  uint8_t* pixels = quirc_begin(this, NULL, NULL);
  for (int y = 0; y < image->height; y++) {
    image->pointers[y] = pixels + y * image->width;
  }
  png_read_image(image->png, image->pointers);
  quirc_end(this);

  return this;
}

void QRCode_delete(QRCode this) {
  quirc_destroy(this);
}

int QRCode_count(QRCode this) {
  return quirc_count(this);
}

void QRCode_scan(QRCode this) {
  int count = QRCode_count(this);
  for (int index = 0; index < count; index++) {
    struct quirc_code code;
    quirc_extract(this, index, &code);

    struct quirc_data data;
    quirc_decode_error_t error = quirc_decode(&code, &data);

    if (error == QUIRC_ERROR_DATA_ECC) {
      quirc_flip(&code);
      error = quirc_decode(&code, &data);
    }
    
    if (!error) {
      printf("%s\n", data.payload);
    } else {
      panic("%s\n", quirc_strerror(error));
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    panic("Usage: %s <png>\n", argv[0]);
  }

  Image image = Image_new(argv[1]);
  QRCode qrcode = QRCode_new(image);
  QRCode_scan(qrcode);
  QRCode_delete(qrcode);
  Image_delete(image);

  return EXIT_SUCCESS;
}
