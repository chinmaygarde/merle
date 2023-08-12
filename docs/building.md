# Building
## Prerequisites
- `cmake`
-  `ninja`: Optional but common tools assume you have this installed.
- `ispc`
- A C++ 20 compiler and standard library.

### Platform

=== "macOS"

    [Homebrew](https://brew.sh/) is recommended on macOS.

    ```sh
    brew install cmake ninja ispc
    ```

## Run Benchmarks

To build and run all benchmarks,

```sh
make bench
```

Benchmarks use a Google Benchmarks harness at `./build/merle_benchmarks`.

## Run Playgrounds

Playgrounds are interactive demos of the various effects. To build and run all playgrounds, 

```sh
make tests
```

This builds and runs the `./build/merle_tests` binary which is a Google Test harness.
