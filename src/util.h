
#include <string.h>

#define die(fmt, ...)                                                         \
    do {                                                                      \
        fprintf(stderr, fmt "\n", ##__VA_ARGS__);                             \
        exit(1);                                                              \
    } while (0)
