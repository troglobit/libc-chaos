# libc-chaos

This is an MIT licensed `LD_PRELOAD` hack library that introduces random
errors in certain libc I/O functions your application uses.

The main purpose is to test how your application handles an unreliable
file system.  It can also be used to locate hard-to-find bugs that only
happen once in a blue moon by introducing unexpected delays.

## Status

This library is still in alpha phase supporting the following functions:

- pwrite()
- pread()
- write()
- read()
- writev()
- readv()


## Examples

Make calling `pread()` have 20% chance to return an `EIO` error:

```
ERROR_RATE=20 LD_PRELOAD=$PWD/libchaos.so PREAD_ERROR=io path/to/your/executable
```

Introduce a 3 sec delay in all `writev()` calls:

```
HANG_TIME=3 WRITEV_ERROR="hang" LD_PRELOAD=$PWD/libchaos.so path/to/your/executable
```


## Control Error Emission with Environment Variables

-   `ERROR_RATE`
defines the rate in percentage at what a function call emits an error.
`0` makes it never emit an error.

-   `HANG_TIME`
defines the delay a function should always introduce before returning.

-   `LD_PRELOAD=XXX`
should be kept what it is. Never touch it unless you know what you do.

-   `PWRITE_ERROR`
defines what error to emit when calling `pwrite()`.
Its value could be one of:
    -   `nospace` emits an `ENOSPC` error.
    -   `io` emits an `EIO` error.
    -   `intr` emits an `EINTR` error.

-   `PREAD_ERROR`
defines what error to emit when calling `pread()`.
Its value could be one of:
    -   `wrong_byte` change 1 byte in the buffer that is read, before
        returning to caller.
    -   `io` emits an `EIO` error.
    -   `intr` emits an `EINTR` error.


## Copyright and License

Copyright (c) 2015-2016  Zhang Yanpo (张炎泼) <drdr.xp@gmail.com>  
Copyright (c) 2017-2019  Joachim Nisson <troglobit@gmail.com>

This library was initially written by 张炎泼 (Zhang Yanpo) who
graciously released the software under the MIT license.  Later
Joachim Nilsson picked it up and added a few more APIs.
