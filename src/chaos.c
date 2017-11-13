#define _GNU_SOURCE
#include "chaos.h"

#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define xstr(s) str(s)
#define str(s) #s

typedef ssize_t (*chaos_pwrite_t)(int fd, const void *buf, size_t count, off_t offset);
typedef ssize_t (*chaos_pread_t)(int fd, void *buf, size_t count, off_t offset);

static int if_emit();
static int error_rate();

static int pre_wrap(char *error_type, int isread)
{
     errno = 0;
     if (!error_type)
	  return 0;

     if (!strcmp(error_type, ERR_HANG)) {
	  int sec;
	  char *hang;

	  hang = getenv(HANG_TIME);
	  if (!hang)
	       hang = "1";

	  sec = atoi(hang);
	  sleep(sec);

	  return 0;
     }

     if (!strcmp(error_type, ERR_IO)) {
	  errno = EIO;
	  return -1;
     }

     if (!strcmp(error_type, ERR_INTR)) {
	  errno = EINTR;
	  return -1;
     }

     if (isread)
	  return 0;

     /*** Only write() errors below this point ************************/

     if (!strcmp(error_type, ERR_NO_SPACE)) {
	  errno = ENOSPC;
	  return -1;
     }

     return 0;
}

static void post_wrap(const char *error_type, int isread, char *buf, ssize_t len)
{
    (void)isread;

     if (!error_type)
	  return;

     if (!strcmp(error_type, ERR_WRONG_BYTE)) {
	  if (len <= 0)
	       return;

	  buf[rand() % len] += 1;
     }
}

#define write_wrapper(fn, mode, args...)			\
     {								\
	  ssize_t ret;						\
	  ssize_t (*sysc)() = dlsym(RTLD_NEXT, xstr(fn));	\
	  							\
	  if (if_emit()) {					\
	       ret = pre_wrap(getenv(mode), 0);			\
	       if (ret)						\
		    return ret;					\
	  }							\
	  							\
	  return sysc(args);					\
     }

#define read_wrapper(fn, mode, args...)				\
     {								\
	  int emit;						\
	  ssize_t ret;						\
	  ssize_t (*sysc)() = dlsym(RTLD_NEXT, xstr(fn));	\
								\
	  emit = if_emit();					\
	  if (emit) {						\
	       ret = pre_wrap(getenv(mode), 1);			\
	       if (ret)						\
		    return ret;					\
	  }							\
								\
	  ret = sysc(args);					\
								\
	  if (emit)						\
	       post_wrap(getenv(mode), 1, buf, ret);		\
								\
	  return ret;						\
     }

ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
     write_wrapper(pwrite, PWRITE_ERROR, fd, buf, count, offset);

ssize_t pread(int fd, void *buf, size_t count, off_t offset)
     read_wrapper(pread, PREAD_ERROR, fd, buf, count, offset);

ssize_t write(int fd, const void *buf, size_t count)
     write_wrapper(write, WRITE_ERROR, fd, buf, count);

ssize_t read(int fd, void *buf, size_t count)
     read_wrapper(read, READ_ERROR, fd, buf, count);

static int if_emit(void)
{
    return ((rand() % 100) < error_rate());
}

static int error_rate(void)
{
    int rate = 10; /* percentage */
    const char *error_rate;

    error_rate = getenv("ERROR_RATE");
    if (error_rate)
       rate = atoi(error_rate);

    if (rate > 100)
        rate = 100;
    if (rate < 0)
        rate = 0;

    return rate;
}
