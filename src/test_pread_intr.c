#include "util.h"

test_pread({

    char buf[1];
    int n = pread(fd, buf, 1, 0);

    if (n != -1 || errno != EINTR) {
        die("errno should be EINTR but %s", strerror(errno));
    }

});
