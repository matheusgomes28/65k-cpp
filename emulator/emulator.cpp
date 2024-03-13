module;
#include <array>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <optional>
#include <span>

#include <fmt/format.h>

export module emulator;

export namespace emulator {

struct Registers {
    std::uint8_t a;
    std::uint8_t x;
    std::uint8_t y;
    std::uint8_t pc;
    std::uint8_t sp;
};

struct Cpu {
    // registers (A, X, Y, SP, PC) - u8
    Registers reg{};

    // CZID B1VN
    std::bitset<8> flags{};

    // memory (Zero page/first FF bytes, main memory, vram)
    std::array<std::uint8_t, 0xFF> mem{};
};
} // namespace emulator

void lda_immediate(emulator::Cpu& cpu, std::uint8_t val)
{
    cpu.reg.a = val;
    cpu.flags[1] = val == 0;
    cpu.flags[7] = val & 0b1000'0000;
}

void sta_zeropage(emulator::Cpu& cpu, std::uint8_t value)
{
    cpu.mem[value] = cpu.reg.a;
}

std::optional<std::size_t> execute_next(emulator::Cpu& cpu, std::span<std::uint8_t> program)
{
    // Read 1 byte for the operator
    if (cpu.reg.pc >= program.size()) {
        return std::nullopt;
    }
    
    auto const command = program[cpu.reg.pc];
    auto bytes_read = 0;
    // Read {1, 2,3} bytes for the operand
    switch (command)
    {
    case 0x00: // LDA #$XX
    { // This should take 2 cicles
        // LOAD Value into accumulator
        bytes_read = 0;
        break;
    }
    case 0xa9: // LDA #$XX
    { // This should take 2 cicles
        // LOAD Value into accumulator
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }
        
        auto const value = program[cpu.reg.pc + 1];
        std::cout << fmt::format("running command {:x} with {:x}\n", command, value);
        lda_immediate(cpu, value);
        bytes_read = 2;
        break;
    }
    case 0x85: // STA $XX
    { // This should take 3 cycles
        // LOAD Value into accumulator
        // LOAD Value into accumulator
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }
        
        auto const value = program[cpu.reg.pc + 1];
        sta_zeropage(cpu, value);
        bytes_read = 2;
        break;
        //std::cout << fmt::format("running command {:x} with {:x}\n", command, operand);
    }
    default:
        // unsupported command
        return std::nullopt;
        break;
    }

    return bytes_read;
}


export namespace emulator {

bool execute(Cpu& cpu, std::span<std::uint8_t> program)
{
    while (cpu.reg.pc < program.size()) {
        auto maybe_increment = execute_next(cpu, program);
        if (!maybe_increment) {
            return false;
        }

        cpu.reg.pc += *maybe_increment;
    }

    return true;
}

} // namespace emulator