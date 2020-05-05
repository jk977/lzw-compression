# LZW Compression

## About

This is an implementation of the [Lempel-Ziv-Welch compression algorithm](https://www.google.com/patents/US4558302) using variable-width codes.

## Dependencies

* A version of `gcc` with C99 support
* GNU `make`
* The POSIX C API (optional if not using `main.c`)

## Compilation

To compile a library containing the functions in `include/lzw.h`, run `make lib`. This will store `liblzw.a` in the `build` directory.

On POSIX-compliant systems, the command-line interface can be compiled with `make cli`. The resulting executable will be stored at `build/lzw`. The program usage can be viewed by running `lzw -h`.
