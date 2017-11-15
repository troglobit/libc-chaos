#include "util.h"

test_pwrite({

    char buf[1] = {'a'};
    int n = pwrite(fd, buf, 1, 0);

    if (n >= 0 || errno != ENOSPC) {
        dd("n write = %d\n", n);
        die("expect errno == ENOSPC but: %s", strerror(errno));
    }

});
