#include <stdlib.h>

#include "helpers.h"
#include "qrcode.h"

QRCode qrcode_new() {
    return quirc_new();
}

void qrcode_delete(QRCode this) {
    if (this != NULL) {
        quirc_destroy(this);
    }
}

ByteArray qrcode_alloc(QRCode this, int width, int height) {
    if (quirc_resize(this, width, height)) {
        panic("Allocate QRCode failed\n");
    }
    return quirc_begin(this, NULL, NULL);
}

void qrcode_scan(QRCode this) {
    quirc_end(this);
    int count = quirc_count(this);
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
