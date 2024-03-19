# Build and run:
#   docker build -t clion/ubuntu/cpp-env:1.0 -f Dockerfile.cpp-env-ubuntu .

FROM ubuntu:20.04

RUN DEBIAN_FRONTEND="noninteractive" apt-get update && apt-get -y install tzdata

RUN apt-get update \
  && apt-get install -y build-essential \
      sudo \
      libgtest-dev \
      gcc \
      g++ \
      gdb \
      clang \
      make \
      ninja-build \
      cmake \
      autoconf \
      automake \
      libtool \
      valgrind \
      locales-all \
      dos2unix \
      rsync \
      tar \
      python \
      python-dev \
      python3-pip \
      clang-tidy \
      clang-format \
      vim \
      git \
  && apt-get clean  \
    && cd /usr/src/gtest  \
    && sudo mkdir build  \
    && cd build  \
    && sudo cmake ..  \
    && sudo make  \
    && cd lib  \
    && sudo cp *.a /usr/lib \
    && pip3 install cpplint \