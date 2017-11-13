
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#define dd(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

#define die(fmt, ...)                                                         \
    do {                                                                      \
        fprintf(stderr, fmt "\n", ##__VA_ARGS__);                             \
        exit(1);                                                              \
    } while (0)


#define test_pread(statement)                                                 \
    int main(int argc, char **argv) {                                         \
                                                                              \
        unlink("t");                                                          \
        system("echo 1 >t");                                                  \
                                                                              \
        int fd = open("t", O_RDWR, 0640);                                     \
        if (fd < 0) {                                                         \
            die("open");                                                      \
        }                                                                     \
                                                                              \
        errno = 0;                                                            \
        statement                                                             \
                                                                              \
        dd("passed");                                                         \
        unlink("t");                                                          \
                                                                              \
        return 0;                                                             \
    }

#define test_pwrite(statement)                                                \
    int main(int argc, char **argv) {                                         \
                                                                              \
        int fd = open("t", O_CREAT | O_EXCL | O_SYNC | O_RDWR | O_TRUNC,      \
                      0640);                                                  \
        if (fd < 0) {                                                         \
            die("error open");                                                \
        }                                                                     \
                                                                              \
        statement                                                             \
                                                                              \
        dd("passed");                                                         \
        unlink("t");                                                          \
                                                                              \
        return 0;                                                             \
    }

static inline int64_t get_now_ms() {
    struct timeval t0;
    gettimeofday(&t0, NULL);
    return t0.tv_sec * 1000 + t0.tv_usec / 1000;
}
