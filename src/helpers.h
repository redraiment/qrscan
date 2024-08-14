#ifndef QRSCAN_TYPINGS_H
#define QRSCAN_TYPINGS_H

#include <stdlib.h>
#include <stdio.h>

#define PROGRAM_NAME "qrscan"
#define PROGRAM_VERSION "1.0.0"

#define panic(...) do { \
    fprintf(stderr, __VA_ARGS__); \
    exit(EXIT_FAILURE); \
  } while(0)

typedef char* String;

#endif //QRSCAN_TYPINGS_H
