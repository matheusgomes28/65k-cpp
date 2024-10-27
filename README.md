# 6502 Emulator

This repository contains the source code for a **6502 Emulator**, written in **C++**. The emulator
is designed to replicate the functionality of the 6502 microprocessor, which was widely used in
early personal computers.

The 6502 emulator is provided as a static library that can be plugged into any frontend. The
main API provides the following function:

+ `emulator::execute(cpu, program)` that takes an
entire program and the initialised `Cpu` struct, and then executes the whole program,
changing the state of the given `Cpu` as expected.
+ `emulator::execute_next(cpu, program)` single steps the next isntruction on the given `cpu`.

## Getting Started

To get started with the 65k-cpp emulator, clone the repository and follow the instructions below:

The tools needed to build are:

+ `cmake` version 3.28 or above.
+ `ninja` version 1.11 or above.

```bash
git clone https://github.com/matheusgomes28/65k-cpp.git
cmake --preset unix-rel-ninja
cmake --build --preset unit-rel-ninja
```

## Plans

The goal for this project is to have support for all opcodes in ths standard 6502.

In addition, the aim to to provide a couple of front end applications that can step
through 6504 applications, showing state of flags, registers, and memory:

![cli frontend](images/end-goal-65k-emulator.png)

## Opcode support

This section contains support tables for each operation. The tickbox in the
`supported` column indicates whether or not the operation is isupported.

+ :white_check_mark: means opcode support and tests are available.
+ :ok: means opcode support is available, but tests needs writing.
+ :x: means there's no opcode support or tests.

### AND - And Memory with Accumulator

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| AND        | Immediate           | 0x29             | 2         | 2          | :white_check_mark: |
| AND        | Zero Page           | 0x25             | 2         | 3          | :white_check_mark: |
| AND        | Zero Page, X        | 0x35             | 2         | 4          | :white_check_mark: |
| AND        | Absolute            | 0x2D             | 3         | 4          | :white_check_mark: |
| AND        | Absolute, X         | 0x3D             | 3         | 4*         | :white_check_mark: |
| AND        | Absolute, Y         | 0x39             | 3         | 4*         | :white_check_mark: |
| AND        | (Indirect, X)       | 0x21             | 2         | 6          | :white_check_mark: |
| AND        | (Indirect), Y       | 0x31             | 2         | 5*         | :white_check_mark: |

Total number of `AND` instructions: `8`

### EOR - Exclusive-Or Memory with Accumulator

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| EOR        | Immediate           | 0x49             | 2         | 2          | :white_check_mark: |
| EOR        | Zero Page           | 0x45             | 2         | 3          | :white_check_mark: |
| EOR        | Zero Page, X        | 0x55             | 2         | 4          | :white_check_mark: |
| EOR        | Absolute            | 0x4D             | 3         | 4          | :white_check_mark: |
| EOR        | Absolute, X         | 0x5D             | 3         | 4*         | :white_check_mark: |
| EOR        | Absolute, Y         | 0x59             | 3         | 4*         | :white_check_mark: |
| EOR        | (Indirect, X)       | 0x41             | 2         | 6          | :white_check_mark: |
| EOR        | (Indirect), Y       | 0x51             | 2         | 5*         | :white_check_mark: |

Total number of `EOR` instructions: `8`

### ORA - Or Memory with Accumulator

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| ORA        | Immediate           | 0x09             | 2         | 2          | :white_check_mark: |
| ORA        | Zero Page           | 0x05             | 2         | 3          | :white_check_mark: |
| ORA        | Zero Page, X        | 0x15             | 2         | 4          | :white_check_mark: |
| ORA        | Absolute            | 0x0D             | 3         | 4          | :white_check_mark: |
| ORA        | Absolute, X         | 0x1D             | 3         | 4*         | :white_check_mark: |
| ORA        | Absolute, Y         | 0x19             | 3         | 4*         | :white_check_mark: |
| ORA        | (Indirect, X)       | 0x01             | 2         | 6          | :white_check_mark: |
| ORA        | (Indirect), Y       | 0x11             | 2         | 5*         | :white_check_mark: |

Total number of `ORA` instructions: `8`.

### LDA - Load Accumulator with Memory

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| LDA        | Immediate           | 0xA9             | 2         | 2          | :white_check_mark: |
| LDA        | Zero Page           | 0xA5             | 2         | 3          | :white_check_mark: |
| LDA        | Zero Page, X        | 0xB5             | 2         | 4          | :white_check_mark: |
| LDA        | Absolute            | 0xAD             | 3         | 4          | :white_check_mark: |
| LDA        | Absolute, X         | 0xBD             | 3         | 4*         | :white_check_mark: |
| LDA        | Absolute, Y         | 0xB9             | 3         | 4*         | :white_check_mark: |
| LDA        | (Indirect, X)       | 0xA1             | 2         | 6          | :white_check_mark: |
| LDA        | (Indirect), Y       | 0xB1             | 2         | 5*         | :white_check_mark: |

Total number of `LDA` instructions: `8`.

### LDX - Load X Register with Memory

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| LDX        | Immediate           | 0xA2             | 2         | 2          | :white_check_mark: |
| LDX        | Zero Page           | 0xA6             | 2         | 3          | :white_check_mark: |
| LDX        | Zero Page, Y        | 0xB6             | 2         | 4          | :white_check_mark: |
| LDX        | Absolute            | 0xAE             | 3         | 4          | :white_check_mark: |
| LDX        | Absolute, Y         | 0xBE             | 3         | 4*         | :white_check_mark: |

Total number of `LDX` instructions: `5`.

### LDY - Load X Register with Memory

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| LDY        | Immediate           | 0xA0             | 2         | 2          | :white_check_mark: |
| LDY        | Zero Page           | 0xA4             | 2         | 3          | :white_check_mark: |
| LDY        | Zero Page, Y        | 0xB4             | 2         | 4          | :white_check_mark: |
| LDY        | Absolute            | 0xAC             | 3         | 4          | :white_check_mark: |
| LDY        | Absolute, Y         | 0xBC             | 3         | 4*         | :white_check_mark: |

Total number of `LDY` instructions: `5`.

### LDY - Load Y Register with Memory

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| LDY        | Immediate           | 0xA0             | 2         | 2          | :white_check_mark: |
| LDY        | Zero Page           | 0xA4             | 2         | 3          | :white_check_mark: |
| LDY        | Zero Page, Y        | 0xB4             | 2         | 4          | :white_check_mark: |
| LDY        | Absolute            | 0xAC             | 3         | 4          | :white_check_mark: |
| LDY        | Absolute, Y         | 0xBC             | 3         | 4*         | :white_check_mark: |

Total number of `LDY` instructions: `5`.

### STA - Store Accumulator in Memory

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| STA        | Zero Page           | 0x85             | 2         | 3          | :white_check_mark: |
| STA        | Zero Page, X        | 0x95             | 2         | 4          | :white_check_mark: |
| STA        | Absolute            | 0x8D             | 3         | 4          | :white_check_mark: |
| STA        | Absolute, X         | 0x9D             | 3         | 5          | :white_check_mark: |
| STA        | Absolute, Y         | 0x99             | 3         | 5          | :white_check_mark: |
| STA        | (Indirect, X)       | 0x81             | 2         | 6          | :white_check_mark: |
| STA        | (Indirect), Y       | 0x91             | 2         | 6          | :white_check_mark: |

Total number of `STA` instructions: `7`.

### STX - Store Accumulator in Memory

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| STX        | Zero Page           | 0x86             | 2         | 3          | :white_check_mark: |
| STX        | Zero Page, Y        | 0x96             | 2         | 4          | :white_check_mark: |
| STX        | Absolute            | 0x8E             | 3         | 4          | :white_check_mark: |

Total number of `STX` instructions: `3`.

### STY - Store Accumulator in Memory

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| STY        | Zero Page           | 0x84             | 2         | 3          | :white_check_mark: |
| STY        | Zero Page, Y        | 0x94             | 2         | 4          | :white_check_mark: |
| STY        | Absolute            | 0x8C             | 3         | 4          | :white_check_mark: |

Total number of `STY` instructions: `3`.

### Transfer Operations

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| TAX        | Implied             | 0xAA             | 1         | 2          | :white_check_mark: |
| TAY        | Implied             | 0xA8             | 1         | 2          | :white_check_mark: |
| TSX        | Implied             | 0xBA             | 1         | 2          | :white_check_mark: |
| TXA        | Implied             | 0x8A             | 1         | 2          | :white_check_mark: |
| TXS        | Implied             | 0x9A             | 1         | 2          | :white_check_mark: |
| TYA        | Implied             | 0x98             | 1         | 2          | :white_check_mark: |

Total number of transfer instructions: `6`.

### Stack Operations

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| PHA        | Implied             | 0x48             | 1         | 3          | :white_check_mark: |
| PHP        | Implied             | 0x08             | 1         | 3          | :white_check_mark: |
| PLA        | Implied             | 0x68             | 1         | 4          | :white_check_mark: |
| PLP        | Implied             | 0x28             | 1         | 4          | :white_check_mark: |

Total number of stack instructions: `4`.

### Decrement and Increment Operations

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| DEC        | Zero Page           | 0xC6             | 2         | 5          | :white_check_mark: |
| DEC        | Zero Page, X        | 0xD6             | 2         | 6          | :ok:               |
| DEC        | Absolute            | 0xCE             | 3         | 6          | :ok:               |
| DEC        | Absolute, X         | 0xDE             | 3         | 7          | :ok:               |
| DEX        | Implied             | 0xCA             | 1         | 2          | :white_check_mark: |
| DEY        | Implied             | 0x88             | 1         | 2          | :white_check_mark: |
| INC        | Zero Page           | 0xE6             | 2         | 5          | :white_check_mark: |
| INC        | Zero Page, X        | 0xF6             | 2         | 6          | :white_check_mark: |
| INC        | Absolute            | 0xEE             | 3         | 6          | :white_check_mark: |
| INC        | Absolute, X         | 0xFE             | 3         | 7          | :white_check_mark: |
| INX        | Implied             | 0xE8             | 1         | 2          | :white_check_mark: |
| INY        | Implied             | 0xC8             | 1         | 2          | :white_check_mark: |

Total number of stack instructions: `12`.

### ADC - Add to Accumulator with Carry

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| ADC        | Immediate           | 0x69             | 2         | 2          | :x:                |
| ADC        | Zero Page           | 0x65             | 2         | 3          | :x:                |
| ADC        | Zero Page, X        | 0x75             | 2         | 4          | :x:                |
| ADC        | Absolute            | 0x6D             | 3         | 4          | :x:                |
| ADC        | Absolute, X         | 0x7D             | 3         | 4*         | :x:                |
| ADC        | Absolute, Y         | 0x79             | 3         | 4*         | :x:                |
| ADC        | (Indirect, X)       | 0x61             | 2         | 6          | :x:                |
| ADC        | (Indirect), Y       | 0x71             | 2         | 5*         | :x:                |

Total number of `ADC` instructions: `8`.

### SBC - Subtract Memory from Accumulator with Borrow

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| SBC        | Immediate           | 0xE9             | 2         | 2          | :x:                |
| SBC        | Zero Page           | 0xE5             | 2         | 3          | :x:                |
| SBC        | Zero Page, X        | 0xF5             | 2         | 4          | :x:                |
| SBC        | Absolute            | 0xED             | 3         | 4          | :x:                |
| SBC        | Absolute, X         | 0xFD             | 3         | 4*         | :x:                |
| SBC        | Absolute, Y         | 0xF9             | 3         | 4*         | :x:                |
| SBC        | (Indirect, X)       | 0xE1             | 2         | 6          | :x:                |
| SBC        | (Indirect), Y       | 0xF1             | 2         | 5*         | :x:                |

Total number of `SBC` instructions: `8`.

### ASL - Shift Left One Bit

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| ASL        | Accumulator         | 0x0A             | 1         | 2          | :white_check_mark: |
| ASL        | Zero Page           | 0x06             | 2         | 5          | :white_check_mark: |
| ASL        | Zero Page, X        | 0x16             | 2         | 6          | :white_check_mark: |
| ASL        | Absolute            | 0x0E             | 3         | 6          | :white_check_mark: |
| ASL        | Absolute, X         | 0x1E             | 3         | 7          | :white_check_mark: |

Total number of `ASL` instructions: `5`.

### LSR - Shift Right One Bit

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| LSR        | Accumulator         | 0x4A             | 1         | 2          | :white_check_mark: |
| LSR        | Zero Page           | 0x46             | 2         | 5          | :white_check_mark: |
| LSR        | Zero Page, X        | 0x56             | 2         | 6          | :white_check_mark: |
| LSR        | Absolute            | 0x4E             | 3         | 6          | :white_check_mark: |
| LSR        | Absolute, X         | 0x5E             | 3         | 7          | :white_check_mark: |

Total number of `LSR` instructions: `5`.

### ROR - Rotate Right One Bit with Carry

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| ROR        | Accumulator         | 0x6A             | 1         | 2          | :white_check_mark: |
| ROR        | Zero Page           | 0x66             | 2         | 5          | :white_check_mark: |
| ROR        | Zero Page, X        | 0x76             | 2         | 6          | :white_check_mark: |
| ROR        | Absolute            | 0x6E             | 3         | 6          | :white_check_mark: |
| ROR        | Absolute, X         | 0x7E             | 3         | 7          | :white_check_mark: |

Total number of `ROR` instructions: `5`.

### ROL - Rotate Left One Bit with Carry

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| ROL        | Accumulator         | 0x6A             | 1         | 2          | :white_check_mark: |
| ROL        | Zero Page           | 0x66             | 2         | 5          | :white_check_mark: |
| ROL        | Zero Page, X        | 0x76             | 2         | 6          | :white_check_mark: |
| ROL        | Absolute            | 0x6E             | 3         | 6          | :white_check_mark: |
| ROL        | Absolute, X         | 0x7E             | 3         | 7          | :white_check_mark: |

Total number of `ROL` instructions: `5`.

### CMP - Compare Memory with Accumulator

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| CMP        | Immediate           | 0xC9             | 2         | 2          | :white_check_mark: |
| CMP        | Zero Page           | 0xC5             | 2         | 3          | :white_check_mark: |
| CMP        | Zero Page, X        | 0xD5             | 2         | 4          | :white_check_mark: |
| CMP        | Absolute            | 0xCD             | 3         | 4          | :white_check_mark: |
| CMP        | Absolute, X         | 0xDD             | 3         | 4*         | :white_check_mark: |
| CMP        | Absolute, Y         | 0xD9             | 3         | 4*         | :white_check_mark: |
| CMP        | (Indirect, X)       | 0xC1             | 2         | 6          | :white_check_mark: |
| CMP        | (Indirect), Y       | 0xD1             | 2         | 5*         | :white_check_mark: |

Total number of `CMP` instructions: `8`.

### CPX - Compare Memory with X Register

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| CPX        | Immediate           | 0xE0             | 2         | 2          | :white_check_mark: |
| CPX        | Zero Page           | 0xE4             | 2         | 3          | :white_check_mark: |
| CPX        | Absolute            | 0xEC             | 3         | 4          | :white_check_mark: |

Total number of `CPX` instructions: `3`.

### CPY - Compare Memory with Y Register

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| CPY        | Immediate           | 0xC0             | 2         | 2          | :white_check_mark: |
| CPY        | Zero Page           | 0xC4             | 2         | 3          | :white_check_mark: |
| CPY        | Absolute            | 0xCC             | 3         | 4          | :white_check_mark: |

Total number of `CPY` instructions: `3`.

### Flag Operations

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| CLC        | Implied             | 0x18             | 1         | 2          | :white_check_mark: |
| CLD        | Implied             | 0xD8             | 1         | 2          | :white_check_mark: |
| CLI        | Implied             | 0x58             | 1         | 2          | :white_check_mark: |
| CLV        | Implied             | 0xB8             | 1         | 2          | :white_check_mark: |
| SEC        | Implied             | 0x38             | 1         | 2          | :white_check_mark: |
| SED        | Implied             | 0xF8             | 1         | 2          | :white_check_mark: |
| SEI        | Implied             | 0x78             | 1         | 2          | :white_check_mark: |

Total number of flag instructions: `7`.

### Branching Operations

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| BCC        | Relative            | 0x90             | 2         | 2**        | :white_check_mark: |
| BCS        | Relative            | 0xB0             | 2         | 2**        | :white_check_mark: |
| BEQ        | Relative            | 0xF0             | 2         | 2**        | :white_check_mark: |
| BMI        | Relative            | 0x30             | 2         | 2**        | :white_check_mark: |
| BNE        | Relative            | 0xD0             | 2         | 2**        | :white_check_mark: |
| BPL        | Relative            | 0x10             | 2         | 2**        | :white_check_mark: |
| BVC        | Relative            | 0x50             | 2         | 2**        | :white_check_mark: |
| BVS        | Relative            | 0x70             | 2         | 2**        | :white_check_mark: |

Total number of branching instructions: `8`.

### Jumps and Subroutine Operations

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| JMP        | Absolute            | 0x4C             | 3         | 3          | :white_check_mark: |
| JMP        | Indirect            | 0x6C             | 3         | 5          | :ok:               |
| JSR        | Absolute            | 0x20             | 3         | 6          | :x:                |
| RTS        | Absolute            | 0x60             | 1         | 6          | :x:                |

Total number of jump and subroutine instructions: `4`.

### Interrupt Operations

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| BRK        | Implied             | 0x00             | 1         | 7          | :x:                |
| RTI        | Implied             | 0x40             | 1         | 6          | :x:                |

total number of interrupt instructions: `2`.

### Random Operations

| **Opcode** | **Addressing Mode** | **Opcode (Hex)** | **Bytes** | **Cycles** | **Supported**      |
|------------|---------------------|------------------|-----------|------------|--------------------|
| BIT        | Zeropage            | 0x24             | 2         | 3          | :white_check_mark: |
| BIT        | Absolute            | 0x2c             | 3         | 4          | :white_check_mark: |
| NOP        | Implied             | 0xEA             | 1         | 2          | :white_check_mark: |

total number of random instructions: `2`.

## Contributing

Feel free to submit issues or pull requests. Contributions are welcome!

## License

This project is licensed under the MIT License.
