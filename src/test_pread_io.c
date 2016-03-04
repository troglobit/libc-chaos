#include "util.h"

test_pread({

    char buf[1];
    int n = pread(fd, buf, 1, 0);

    if (n != -1 || errno != EIO) {
        die("errno should be EIO but %s", strerror(errno));
    }
});
