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

int qrcode_count(QRCode this) {
    quirc_end(this);
    return quirc_count(this);
}

void qrcode_scan(QRCode this, int index) {
    quirc_end(this);
    int count = quirc_count(this);
    if (1 <= index && index <= count) {
        count = index;
        index--;
    } else if (index != 0) {
        panic("Option index(%d) out of bound [1, %d]\n", index, count);
    }

    for (; index < count; index++) {
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
