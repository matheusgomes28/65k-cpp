# Version: 0.13

# Base ubuntu 20.04
FROM ubuntu:jammy-20240911.1

# Environment variables needed for the
# build system
ENV TZ=Europe/London
ENV DEBIAN_FRONTEND="noninteractive"

# Identify the maintainer of an image
LABEL maintainer="matheusgarcia28@gmail.com"

# Install system tools
RUN apt-get update \
    && apt-get install -y --no-install-recommends --no-install-suggests \
        curl \
        git \
        libgl1-mesa-dev \
        libx11-dev \
        libxcursor-dev \
        libxi-dev \
        libxinerama-dev \
        libxrandr-dev \
        pkg-config \
        python3 \
        python3-pip \
        python3-setuptools \
        tree \
    && apt-get clean \
    && apt-get autoremove -y \
    && apt-get autoremove --purge -y \
    && rm -rf /var/lib/apt/lists/*

# Install the LLVM 17 tools
RUN apt-get update \
    && apt-get install -y --no-install-recommends --no-install-suggests \
        gnupg \
        lsb-release \
        software-properties-common\
        wget \
    && wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh 17 \
    && apt-get install -y --no-install-recommends --no-install-suggests \
        clang-format-17 \
        clang-tidy-17 \
        clang-tools-17 \
        libclang-rt-17-dev \
    && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-17 10 \
    && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-17 10 \
    && update-alternatives --install /usr/bin/clang-scan-deps clang-scan-deps /usr/bin/clang-scan-deps-17 10 \
    && update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-17 10 \
    && update-alternatives --install /usr/bin/run-clang-tidy run-clang-tidy /usr/bin/run-clang-tidy-17 10 \
    && rm llvm.sh \
    && apt-get autoremove -y \
        gnupg \
        lsb-release \
        software-properties-common\
        wget \
    && apt-get clean \
    && apt-get autoremove -y \
    && apt-get autoremove --purge -y \
    && rm -rf /var/lib/apt/lists/*

# Upgrade pip
RUN pip3 install --upgrade --no-cache --ignore-installed pip

# Install Conan dependencies
RUN pip3 install --no-cache --ignore-installed cmake==3.28 \
    && pip3 install --no-cache -q --ignore-installed ninja==1.11.1 \
    && update-alternatives --install /usr/bin/ninja ninja /usr/local/bin/ninja 10

