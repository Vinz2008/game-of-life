#include <stdio.h>

#define ERROR(...) \
    fprintf(stderr, __VA_ARGS__); \
    exit(1)
