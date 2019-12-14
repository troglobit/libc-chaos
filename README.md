# libc-chaos

Wraps libc functions to return errors randomly to simulate an unstable
underlaying system.

## Status

This library is still in alpha phase supporting the following functions:

- pwrite()
- pread()
- write()
- read()

## Synopsis

```
make

# The following line makes calling pread() have 20% chance to return an EIO error.

ERROR_RATE=20 LD_PRELOAD=$PWD/libchaos.so PREAD_ERROR=io path/to/your/executable
```

## Control Error Emission with Environment Variables

-   `ERROR_RATE`
defines the rate in percentage at what a function call emits an error.
`0` makes it never emit an error.

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

The MIT License

Copyright (c) 2015-2016  Zhang Yanpo (张炎泼) <drdr.xp@gmail.com>
Copyright (c) 2017-2019  Joachim Nisson <troglobit@gmail.com>
