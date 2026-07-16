# Dockerfile

FROM ubuntu:26.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    software-properties-common \
    && apt update && apt install -y \
    g++-16 \
    cmake \
    git \
    make \
    bison \
    flex \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-16 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-16 100 \
    && rm -rf /var/lib/apt/lists/*

ENV CC=/usr/bin/gcc-16
ENV CXX=/usr/bin/g++-16

WORKDIR /usr/src/ebnfparser

COPY . .

RUN cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build

CMD ["ctest", "--test-dir", "build", "--output-on-failure"]
