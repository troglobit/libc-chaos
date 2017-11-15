#include "util.h"

test_pread({

    char buf[1];
    int n = pread(fd, buf, 1, 0);

    if (buf[0] != '2') {
        dd("n read = %d", n);
        dd("buf[0] = %c", buf[0]);
        die("buf[0] should be 2");
    }
});
