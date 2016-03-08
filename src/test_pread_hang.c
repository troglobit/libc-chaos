#include "util.h"

test_pread({

    int64_t ms0 = get_now_ms();

    char buf[1];
    int n = pread(fd, buf, 1, 0);

    int64_t ms1 = get_now_ms();

    if (n < 0) {
        die("should not be err but %s", strerror(errno));
    }

    if (ms1 - ms0 < 1900) {
        die("should hang about 2000 ms but: %lld", (long long)(ms1 - ms0));
    }

});
