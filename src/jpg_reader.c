#include <stdbool.h>
#include <string.h>

#include "jpg_reader.h"

JpgReader jpg_reader_new(File file) {
    JpgReader this = (JpgReader)malloc(sizeof(struct _JpgReader));
    this->jpg = (j_decompress_ptr) malloc(sizeof (struct jpeg_decompress_struct));
    this->error = (struct jpeg_error_mgr*) malloc(sizeof(struct jpeg_error_mgr));
    this->jpg->err = jpeg_std_error(this->error);

    jpeg_create_decompress(this->jpg);
    jpeg_stdio_src(this->jpg, file);

    jpeg_read_header(this->jpg, true);
    if (this->jpg->jpeg_color_space != JCS_GRAYSCALE) {
        this->jpg->out_color_space = JCS_GRAYSCALE;
        jpeg_calc_output_dimensions(this->jpg);
    }
    jpeg_start_decompress(this->jpg);

    this->width = this->jpg->output_width;
    this->height = this->jpg->output_height;

    return this;
}

void jpg_reader_delete(JpgReader this) {
    if (this != NULL) {
        jpeg_finish_decompress(this->jpg);
        jpeg_destroy_decompress(this->jpg);

        free(this->jpg);
        free(this->error);
        free(this);
    }
}

void jpg_reader_read(JpgReader this, ByteArray buffer) {
    const int step = 1;
    JSAMPARRAY rows = (this->jpg->mem->alloc_sarray)((j_common_ptr)this->jpg, JPOOL_IMAGE, this->width, step);
    while (this->jpg->output_scanline < this->height) {
        jpeg_read_scanlines(this->jpg, rows, step);
        memcpy(buffer + (this->jpg->output_scanline - 1) * this->width, rows[0], this->width);
    }
}
