#ifndef QRSCAN_QRCODE_H
#define QRSCAN_QRCODE_H

#include <quirc.h>

typedef struct quirc* QRCode;

QRCode qrcode_new();
void qrcode_delete(QRCode this);
ByteArray qrcode_alloc(QRCode this, int width, int height);
void qrcode_scan(QRCode this);

#endif //QRSCAN_QRCODE_H
