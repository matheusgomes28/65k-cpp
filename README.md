# 65k-cpp

## Overview
This repository contains the source code for a 6502 Emulator, written in C++. The emulator is designed to replicate the functionality of the 6502 microprocessor, which was widely used in early personal computers.

## Features
- **Emulation of 6502 instructions**: Accurate emulation of the full 6502 instruction set.
- **Support for multiple platforms**: Can be run on various operating systems with C++ compatibility.
- **Extensibility**: Easy to extend and integrate into other projects. Perfect for people who want to learn how emulators work.

## Getting Started
To get started with the 65k-cpp emulator, clone the repository and follow the instructions below:

The tools needed to build are:
    - `conan2` for getting the third party libraries.
    - `cmake` version 3.28 or above.

```bash
git clone https://github.com/matheusgomes28/65k-cpp.git
conan install . --output-folder="conan/deb" -sbuild_type=Debug --build=missing
cmake --preset unix-rel-ninja
cmake --build --preset unit-rel-ninja
```

You can run the emulator with `./build/unix-rel-ninja/emulator/emulator file.bin`. Replacing
`file.bin` with your compiled `6502` binary.
