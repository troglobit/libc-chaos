#include "util.h"

#define hang(fn, args...)						\
{									\
    int64_t ms0 = get_now_ms();						\
									\
    char buf[1];							\
    int n = fn(args);							\
									\
    int64_t ms1 = get_now_ms();						\
									\
    if (n < 0)								\
        die("should not be err but %s", strerror(errno));		\
									\
    if (ms1 - ms0 < 1900)						\
        die("hung %lld ms, expected ~2000 ms", (long long)(ms1 - ms0)); \
}

int test_read_hang(void)
{
	test_read_pre();
	hang(read, fd, buf, 1);
	test_readwrite_post();
}

int test_pread_hang(void)
{
	test_read_pre();
	hang(pread, fd, buf, 1, 0);
	test_readwrite_post();
}

int main(void)
{
	return  test_read_hang() ||
		test_pread_hang();
}
