#include "helpers.h"
#include "options.h"
#include "qrcode.h"
#include "image.h"

int main(int argc, String argv[]) {
    Options options = options_new(argc, argv);
    QRCode qrcode = qrcode_new();
    Image image = image_new(options->filename, options->is_png_format ? IMAGE_FORMAT_PNG : IMAGE_FORMAT_JPG);

    ByteArray buffer = qrcode_alloc(qrcode, image->width, image->height);
    image_read(image, buffer);
    qrcode_scan(qrcode);

    image_delete(image);
    qrcode_delete(qrcode);
    options_delete(options);

    return EXIT_SUCCESS;
}
