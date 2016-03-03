#include "util.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(int argc, char **argv) {

    int fd = open("t", O_CREAT | O_EXCL | O_SYNC | O_RDWR | O_TRUNC, 0640);
    if (fd < 0) {
        printf("error open\n");
        exit(1);
    }

    char buf[1] = {'a'};
    int n = pwrite(fd, buf, 1, 0);

    printf("n write = %d\n", n);

    if (n >= 0) {
        die("expect n == -1 but: %d", n);
    }
    if (errno != ENOSPC) {
        die("expect errno == ENOSPC but: %s", strerror(errno));
    }

    printf("passed\n");
    unlink("t");

    return 0;
}
