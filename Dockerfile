FROM debian:bookworm-slim as build

RUN apt update
RUN apt install -y wget gpg
RUN wget -O- https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB | gpg --dearmor | tee /usr/share/keyrings/oneapi-archive-keyring.gpg > /dev/null
RUN echo "deb [signed-by=/usr/share/keyrings/oneapi-archive-keyring.gpg] https://apt.repos.intel.com/oneapi all main" | tee /etc/apt/sources.list.d/oneAPI.list
RUN apt update
RUN apt install -y cmake ninja-build build-essential intel-oneapi-ispc git

COPY . /src

RUN mkdir -p /build/build
WORKDIR /src/build
RUN cmake ../ -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_ISPC_COMPILER=/opt/intel/oneapi/ispc/latest/bin/ispc -DMERLE_ENABLE_TESTS=OFF
RUN ninja
RUN ./merle_benchmarks
