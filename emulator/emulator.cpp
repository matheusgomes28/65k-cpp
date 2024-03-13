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
    std::array<std::uint8_t, 0xFFFF> mem{};
};
} // namespace emulator

std::optional<std::size_t> ld_immediate(emulator::Cpu& cpu, std::span<std::uint8_t> program, std::uint8_t emulator::Registers::*reg)
{
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }
    auto const value = program[cpu.reg.pc + 1];
    (cpu.reg).*reg = value;
    cpu.flags[1] = value == 0;
    cpu.flags[7] = value & 0b1000'0000;

    return 2;
}

std::optional<std::size_t> inc_zeropage(emulator::Cpu& cpu, std::span<std::uint8_t> program)
{
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const pos = program[cpu.reg.pc + 1];
    cpu.mem[pos]++;
    cpu.flags[1] = cpu.mem[pos] == 0;
    cpu.flags[7] = cpu.mem[pos] & 0b1000'0000;

    return 2;
}

std::optional<std::size_t> txa(emulator::Cpu& cpu)
{
    cpu.reg.a = cpu.reg.x;
    cpu.flags[1] = cpu.reg.a == 0;
    cpu.flags[7] = cpu.reg.a & 0b1000'0000;
    return 1;
}

void sta_zeropage(emulator::Cpu& cpu, std::uint8_t value)
{
    cpu.mem[value] = cpu.reg.a;
}

std::optional<std::size_t> sta_ind_y(emulator::Cpu& cpu, std::span<std::uint8_t> program)
{
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }
    auto const ind = program[cpu.reg.pc + 1];
    auto const pos = cpu.mem[ind];
    
    // TODO : this needs a bounds check here
    // can probably return nullopt
    cpu.mem[pos + cpu.reg.y] = cpu.reg.a;
    return 2;
}

std::optional<std::size_t> execute_next(emulator::Cpu& cpu, std::span<std::uint8_t> program)
{
    // Read 1 byte for the operator
    if (cpu.reg.pc >= program.size()) {
        return std::nullopt;
    }
    
    auto const command = program[cpu.reg.pc];
    std::optional<std::size_t> bytes_read = std::nullopt;
    // Read {1, 2,3} bytes for the operand
    switch (command)
    {
    case 0x00: // LDA #$XX
    { // This should take 2 cicles
        // LOAD Value into accumulator
        bytes_read = 0;
        break;
    }
    case 0x8A: // TXA
    {
        bytes_read = txa(cpu);
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
    case 0x91: // STA($XX), Y
    {
        bytes_read = sta_ind_y(cpu, program);
        break;
    }
    case 0xa0: // LDY #$XX
    { // This should take 2 cicles
        bytes_read = ld_immediate(cpu, program, &emulator::Registers::y);
        break;
    }
    case 0xa2: // LDX #$XX
    { // This should take 2 cicles
        bytes_read = ld_immediate(cpu, program, &emulator::Registers::x);
        break;
    }
    case 0xa9: // LDA #$XX
    { // This should take 2 cicles
        bytes_read = ld_immediate(cpu, program, &emulator::Registers::a);
        break;
    }
    case 0xe6:
    {
        bytes_read = inc_zeropage(cpu, program);
        break;
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