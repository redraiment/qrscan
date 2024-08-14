#ifndef QRSCAN_OPTIONS_H
#define QRSCAN_OPTIONS_H

#include <stdbool.h>
#include "helpers.h"

typedef struct _Options {
    bool show_help;
    bool show_version;
    bool show_count;
    bool is_png_format;
    bool is_jpg_format;
    int index;
    String filename;
} * Options;

Options options_new(int argc, String argv[]);
void options_delete(Options this);

#endif //QRSCAN_OPTIONS_H
