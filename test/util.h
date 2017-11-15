
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

extern char *__progname;

#define dd(fmt, args...)                        		\
    printf("%s: " fmt "\n", __progname, ##args)

#define die(fmt, args...)					\
    do {							\
        fprintf(stderr, "%s: " fmt "\n", __progname, ##args);	\
        exit(1);						\
    } while (0)

#define test_read_pre()                         \
    unlink("t");                                \
    system("echo 1 >t");                        \
                                                \
    int fd = open("t", O_RDWR, 0640);           \
    if (fd < 0)                                 \
        die("open");                            \
                                                \
    errno = 0

#define test_write_pre()                        \
    int fd = open("t", O_CREAT | O_EXCL |       \
        O_SYNC  | O_RDWR | O_TRUNC, 0640);      \
    if (fd < 0)                                 \
        die("error open")

#define test_readwrite_post()                   \
    close(fd);                                  \
    dd("passed");                               \
    unlink("t");                                \
                                                \
    return 0

#define test_pread(statement)                   \
    int main(void)                              \
    {                                           \
        test_read_pre();                        \
        statement;                              \
        test_readwrite_post();                  \
    }

#define test_pwrite(statement)                  \
    int main(void)                              \
    {                                           \
        test_write_pre();                       \
        statement;                              \
        test_readwrite_post();                  \
    }

static inline int64_t get_now_ms(void)
{
    struct timeval t0;

    gettimeofday(&t0, NULL);
    return t0.tv_sec * 1000 + t0.tv_usec / 1000;
}
