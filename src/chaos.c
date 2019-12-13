/*
 * Copyright (c) 2015 Zhang Yanpo (张炎泼) <drdr.xp@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#define _GNU_SOURCE
#include "chaos.h"

#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/uio.h>
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

static void postv_wrap(const char *error_type, int isread, const struct iovec *iov, int iovcnt, ssize_t len)
{
    (void)isread;

     if (!error_type)
	  return;

     if (!strcmp(error_type, ERR_WRONG_BYTE)) {
	  int i;

	  if (len <= 0)
	       return;

	  for (i = 0; i < iovcnt; i++) {
	       char *buf;

	       if (iov[i].iov_len <= (size_t)len) {
		    len -= iov[i].iov_len;
		    continue;
	       }

	       buf = (char *)iov[i].iov_base;
	       buf[rand() % len] += 1;
	       break;
	  }
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

#define readv_wrapper(fn, mode, args...)			\
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
	       postv_wrap(getenv(mode), 1, iov, iovcnt, ret);	\
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

ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
     write_wrapper(writev, WRITEV_ERROR, fd, iov, iovcnt);

ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
     readv_wrapper(readv, READV_ERROR, fd, iov, iovcnt);

#define rol32(data,shift) ((data) >> (shift)) | ((data) << (32 - (shift)))

static int if_emit(void)
{
    struct timeval tv;
    unsigned int seed;

    gettimeofday(&tv, NULL);
    seed = 1000000 * tv.tv_sec + tv.tv_usec;
//  seed = time(NULL) ^ gethostid();
    seed = rol32(seed, seed);
    srand(seed);

    return ((rand() % 100) < error_rate());
}

static int error_rate(void)
{
    int rate = 10; /* percentage */
    const char *env;

    env = getenv("ERROR_RATE");
    if (env)
       rate = atoi(env);

    if (rate > 100)
        rate = 100;
    if (rate < 0)
        rate = 0;

    return rate;
}
