#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "options.h"

static bool is_png_format(String filename) {
    int length = strlen(filename);
    return length > 4 && strcasecmp(filename + length - 4, ".png") == 0;
}

static bool is_jpg_format(String filename) {
    int length = strlen(filename);
    return (length > 4 && strcasecmp(filename + length - 4, ".jpg") == 0)
        || (length > 5 && strcasecmp(filename + length - 5, ".jpeg") == 0);
}

static void options_help() {
    printf("Usage: %s [OPTIONS] [PNG|JPG|-]\n\n", PROGRAM_NAME);
    printf("  Scan a QR code in the terminal using a given image.\n\n");

    printf("Image Options:\n");
    printf("  --png, --jpg\n");
    printf("    The format of the input file, inferred from the extension by default\n");
    printf("\n");

    printf("Options:\n");
    printf("  -v, --version\n");
    printf("    Show the version and exit\n");
    printf("  -h, --help\n");
    printf("    Show this message and exit\n");
    printf("\n");

    printf("Examples:\n");
    printf("  %s image.png               # read from file\n", PROGRAM_NAME);
    printf("  cat image.jpg | %s --jpg - # read from stdin\n", PROGRAM_NAME);
    printf("\n");

    printf("%s home page: <https://github.com/redraiment/qrscan>\n", PROGRAM_NAME);
    printf("Email bug reports to: <redraiment@gmail.com>\n");
    exit(EXIT_SUCCESS);
}

static void options_version() {
    printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
    exit(EXIT_SUCCESS);
}

Options options_new(int argc, String argv[]) {
    Options this = (Options) malloc(sizeof (struct _Options));
    if (this == NULL) {
        panic("Alloc Options failed");
    } else {
        this->show_help = false;
        this->show_version = false;
        this->is_png_format = false;
        this->is_jpg_format = false;
        this->filename = NULL;
    }

    for (int index = 1; index < argc; index++) {
        if (strcmp("-h", argv[index]) == 0 || strcmp("--help", argv[index]) == 0) {
            this->show_help = true;
            break;
        } else if (strcmp("-v", argv[index]) == 0 || strcmp("--version", argv[index]) == 0) {
            this->show_version = true;
            break;
        } else if (strcmp("--png", argv[index]) == 0) {
            this->is_png_format = true;
            this->is_jpg_format = false;
        } else if (strcmp("--jpg", argv[index]) == 0) {
            this->is_png_format = false;
            this->is_jpg_format = true;
        } else if (strcmp("-", argv[index]) == 0 || (argv[index][0] != 0 && argv[index][0] != '-')) {
            this->filename = argv[index];
        } else {
            panic("Unknown argument: %s\n", argv[index]);
        }
    }

    if (this->show_help) {
        options_help();
    } else if (this->show_version) {
        options_version();
    }

    if (this->filename == NULL) {
        this->filename = "-";
    }

    if (!this->is_png_format && !this->is_jpg_format) {
        this->is_png_format = is_png_format(this->filename);
        this->is_jpg_format = is_jpg_format(this->filename);
    }
    if (!this->is_png_format && !this->is_jpg_format) {
        panic("Cannot determine image format: %s\n", this->filename);
    }

    return this;
}

void options_delete(Options this) {
    if (this != NULL) {
        free(this);
    }
}
