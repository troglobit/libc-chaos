#include "util.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv) {

    unlink("t");
    system("echo 1 >t");

    int fd = open("t", O_RDWR, 0640);
    if (fd < 0) {
        die("open");
    }

    char buf[1];
    int n = pread(fd, buf, 1, 0);

    printf("n read = %d\n", n);
    printf("buf[0] = %c\n", buf[0]);

    if (buf[0] != '2') {
        die("buf[0] should be 2");
    }

    printf("passed\n");
    unlink("t");

    return 0;
}
