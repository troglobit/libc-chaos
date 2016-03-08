#define _GNU_SOURCE

#include "chaos.h"

#include <dlfcn.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef ssize_t (*chaos_pwrite_t)(int fd, const void *buf, size_t count, off_t offset);
typedef ssize_t (*chaos_pread_t)(int fd, void *buf, size_t count, off_t offset);

static int if_emit();
static int get_env_error_rate();

ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset) {

    ssize_t ret;
    const char *pwrite_error = getenv(PWRITE_ERROR);
    chaos_pwrite_t ori_pwrite = (chaos_pwrite_t)dlsym(RTLD_NEXT, "pwrite");
    int emit = if_emit();

    if (emit) {

        if (pwrite_error != NULL) {

            if (strcmp(pwrite_error, ERR_HANG) == 0) {

                char *hang = getenv(HANG_TIME);
                if (hang == NULL) {
                    hang = "1";
                }

                int hang_sec = atoi(hang);
                sleep(hang_sec);
                emit = 0;
                goto run;
            }

            if (strcmp(pwrite_error, ERR_NO_SPACE) == 0) {
                ret = -1;
                errno = ENOSPC;
                goto exit;
            }

            if (strcmp(pwrite_error, ERR_IO) == 0) {
                ret = -1;
                errno = EIO;
                goto exit;
            }

            if (strcmp(pwrite_error, ERR_INTR) == 0) {
                ret = -1;
                errno = EINTR;
                goto exit;
            }
        }
    }

run:

    ret = ori_pwrite(fd, buf, count, offset);

exit:

    return ret;
}

ssize_t pread(int fd, void *buf, size_t count, off_t offset) {

    ssize_t ret;
    const char *pread_error = getenv(PREAD_ERROR);
    chaos_pread_t ori_pread = (chaos_pread_t)dlsym(RTLD_NEXT, "pread");
    char *b = buf;
    int emit = if_emit();

    if (emit) {

        if (pread_error != NULL) {

            if (strcmp(pread_error, ERR_HANG) == 0) {

                char *hang = getenv(HANG_TIME);
                if (hang == NULL) {
                    hang = "1";
                }

                int hang_sec = atoi(hang);
                sleep(hang_sec);
                emit = 0;
                goto run;
            }

            if (strcmp(pread_error, ERR_IO) == 0) {
                ret = -1;
                errno = EIO;
                goto exit;
            }

            if (strcmp(pread_error, ERR_INTR) == 0) {
                ret = -1;
                errno = EINTR;
                goto exit;
            }
        }
    }

run:

    ret = ori_pread(fd, buf, count, offset);

    if (emit) {

        if (pread_error != NULL) {

            if (strcmp(pread_error, ERR_WRONG_BYTE) == 0) {
                if (ret > 0) {
                    int n = rand();
                    b[n % ret] += 1;
                }
            }
        }
    }

exit:

    return ret;
}

static int if_emit() {
    int rate = get_env_error_rate();
    return ((rand() % 100) < rate);
}

static int get_env_error_rate() {

    const char *error_rate = getenv("ERROR_RATE");
    int rate = 10; /* percentage */

    if (error_rate != NULL) {
       rate = atoi(error_rate);
    }

    if (rate > 100) {
        rate = 100;
    }

    if (rate < 0) {
        rate = 0;
    }
    return rate;
}
