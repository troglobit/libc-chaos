# libc-chaos

Wrap libc function and emit error return randomly, in order to simulate a unstable underlaying system.

# Status

This library is still in alpha phase.

# Synopsis

```
make

# The following line makes calling pread() have 20% chance to return an EIO error.

ERROR_RATE=20 LD_PRELOAD=$PWD/libchaos.so PREAD_ERROR=io path/to/your/executable
```

# Control Error Emission with Environment Variables

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


# Author

Zhang Yanpo (张炎泼) <drdr.xp@gmail.com>

# Copyright and License

The MIT License (MIT)

Copyright (c) 2015 Zhang Yanpo (张炎泼) <drdr.xp@gmail.com>
