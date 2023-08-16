# Build
## Prerequisites
- `cmake`
-  `ninja`: Optional but common tools assume you have this installed.
- [`ispc`](https://ispc.github.io/downloads.html)
- A C++ 20 compiler and standard library.

### Platform

=== "macOS"

    [Homebrew](https://brew.sh/) is recommended on macOS.

    ```sh
    brew install cmake ninja ispc
    ```
=== "Linux (Debian)"

    Full instructions to install [ISPC for Debian based distributions are here](https://github.com/ispc/ispc#intel-oneapi-distribution).

    First, install ISPC from Intel.
    ```sh
    wget -O- https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB | gpg --dearmor | sudo tee /usr/share/keyrings/oneapi-archive-keyring.gpg > /dev/null
    echo "deb [signed-by=/usr/share/keyrings/oneapi-archive-keyring.gpg] https://apt.repos.intel.com/oneapi all main" | sudo tee /etc/apt/sources.list.d/oneAPI.list
    sudo apt-get update
    sudo apt-get install intel-oneapi-ispc
    ```

    Add the following to your `.rc`. If you don't, you'll need to specify the `-DCMAKE_ISPC_COMPILER` to point to the path of `ispc` to when invoking `CMake`. The default rules in the `Makefile` won't do this for you!

    ``` sh
    source /opt/intel/oneapi/ispc/latest/env/vars.sh
    ```

    Now, install all other dependencies.

    ```sh
    sudo apt install -y cmake ninja-build build-essential git
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
