#ifndef QRSCAN_OPTIONS_H
#define QRSCAN_OPTIONS_H

#include <stdbool.h>
#include "helpers.h"

typedef struct _Options {
    bool show_help;
    bool show_version;
    bool is_png_format;
    bool is_jpg_format;
    String filename;
} * Options;

extern Options options_new(int argc, String argv[]);
extern void options_delete(Options this);

#endif //QRSCAN_OPTIONS_H
