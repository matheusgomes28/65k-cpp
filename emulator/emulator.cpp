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
    std::uint16_t pc;
    std::uint8_t sp;
};

constexpr std::size_t FLAG_N = 7;
constexpr std::size_t FLAG_V = 6;
constexpr std::size_t FLAG_B = 4;
constexpr std::size_t FLAG_D = 3;
constexpr std::size_t FLAG_I = 2;
constexpr std::size_t FLAG_Z = 1;
constexpr std::size_t FLAG_C = 0;

struct Cpu {
    // registers (A, X, Y, SP, PC) - u8
    Registers reg{};

    // CZID B1VN
    std::bitset<8> flags{};

    // memory (Zero page/first FF bytes, main memory, vram)
    std::array<std::uint8_t, 0xFFFF> mem{};
    int id;
    

    inline static int next_id = 0;

    Cpu() : id{next_id}
    {
        Cpu::next_id++;
    }

    Cpu(Cpu& cpu) : id{next_id + 10}
    {
        Cpu::next_id++;
    }

    Cpu(Cpu&& cpu) : id{next_id + 100}
    {
        Cpu::next_id++;
    }
};
} // namespace emulator

std::optional<std::size_t> ld_immediate(emulator::Cpu& cpu, std::span<const std::uint8_t> program, std::uint8_t emulator::Registers::*reg)
{
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }
    auto const value = program[cpu.reg.pc + 1];

    if (value != 0)
    {
        std::cout << fmt::format("storing {:d} to accumulator\n", value);
    }
    (cpu.reg).*reg = value;
    cpu.flags[1] = value == 0;
    cpu.flags[7] = value & 0b1000'0000;

    return 2;
}

std::optional<std::size_t> inc_zeropage(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
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

std::optional<std::size_t> inc_reg(emulator::Cpu& cpu, std::uint8_t emulator::Registers::*reg)
{
    ((cpu.reg).*reg)++;
    cpu.flags[7] = (cpu.reg).*reg & 0b1000'0000;
    cpu.flags[1] = (cpu.reg).*reg == 0;
    return 1;
}

std::optional<std::size_t> dec_reg(emulator::Cpu& cpu, std::uint8_t emulator::Registers::*reg)
{
    ((cpu.reg).*reg)--;
    cpu.flags[7] = (cpu.reg).*reg & 0b1000'0000;
    cpu.flags[1] = (cpu.reg).*reg == 0;
    return 1;
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

void sta_absolute(emulator::Cpu& cpu, std::uint16_t addr)
{
    if (cpu.reg.a != 0) {
        std::cout << fmt::format("Storing {:d} to mem addres {:d}\n", cpu.reg.a, addr);
    }
    cpu.mem[addr] = cpu.reg.a;
}

std::optional<std::size_t> sta_ind_y(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }
    auto const word_pos = program[cpu.reg.pc + 1];


    // TODO : This is unsafe
    auto const lsb = cpu.mem[word_pos];
    auto const hsb = cpu.mem[word_pos + 1];
    auto const pos = (hsb << 8) + lsb + cpu.reg.y;
    
    // TODO : this needs a bounds check here
    // can probably return nullopt
    cpu.mem[pos] = cpu.reg.a;
    return 2;
}

// Compare instructions here

std::optional<std::size_t> cmp_immediate_reg(emulator::Cpu& cpu, std::span<const std::uint8_t> program, std::uint8_t emulator::Registers::*reg)
{
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const value = program[cpu.reg.pc + 1];    
    auto const comparison = (cpu.reg).*reg - value;
    cpu.flags[emulator::FLAG_N] = comparison & 0b1000'0000; // Negative flag
    cpu.flags[emulator::FLAG_Z] = comparison == 0; // Negative flag
    cpu.flags[emulator::FLAG_C] = (cpu.reg).*reg >= value;
    return 2;
}

std::optional<std::size_t> cmp_zeropage_reg(emulator::Cpu& cpu, std::span<const std::uint8_t> program, std::uint8_t emulator::Registers::*reg)
{
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const memory = program[cpu.reg.pc + 1];    
    auto const value = cpu.mem[memory];
    auto const comparison = (cpu.reg).*reg - value;
    cpu.flags[emulator::FLAG_N] = comparison & 0b1000'0000; // Negative flag
    cpu.flags[emulator::FLAG_Z] = comparison == 0; // Negative flag
    cpu.flags[emulator::FLAG_C] = (cpu.reg).*reg >= value;
    return 2;
}

// Branching functions here
std::optional<std::size_t> bne(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }
    auto const offset = static_cast<std::int8_t>(program[cpu.reg.pc + 1]);

    if (!cpu.flags[emulator::FLAG_Z])
    {
        cpu.reg.pc += offset;
    }

    return 2;
}

std::optional<std::size_t> execute_next(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
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
    case 0x00: // BREAK
    { // This should take 2 cicles
        // LOAD Value into accumulator
        return std::nullopt;
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
    case 0x8D: // STA $XX
    { // This should take 3 cycles
        // LOAD Value into accumulator
        // LOAD Value into accumulator
        if ((cpu.reg.pc + 2) >= program.size())
        {
            return std::nullopt;
        }
        
        // (hsb << 8) + lsb convert little endian to the address
        auto const lsb = program[cpu.reg.pc + 1];
        auto const hsb = program[cpu.reg.pc + 2];
        sta_absolute(cpu, (hsb << 8) + lsb);
        bytes_read = 3;
        break;
        //std::cout << fmt::format("running command {:x} with {:x}\n", command, operand);
    }
    case 0x88: // DEY
    {
        bytes_read = dec_reg(cpu, &emulator::Registers::y);
        break;
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
    case 0xc0: // CPY
    {
        bytes_read = cmp_immediate_reg(cpu, program, &emulator::Registers::y);
        break;
    }
    case 0xc5: // CMP
    {
        bytes_read = cmp_zeropage_reg(cpu, program, &emulator::Registers::a);
        break;
    }
    case 0xc8: // INY
    {
        bytes_read = inc_reg(cpu, &emulator::Registers::y);
        break;
    }
    case 0xca: // DEX
    {
        bytes_read = dec_reg(cpu, &emulator::Registers::x);
        break;
    }
    case 0xD0: // BNE
    {
        bytes_read = bne(cpu, program);
        break;
    }
    case 0xe0: // CPX
    {
        bytes_read = cmp_immediate_reg(cpu, program, &emulator::Registers::x);
        break;
    }
    case 0xe6:
    {
        bytes_read = inc_zeropage(cpu, program);
        break;
    }
    case 0xe8: // INX
    {
        bytes_read = inc_reg(cpu, &emulator::Registers::x);
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

bool execute(Cpu& cpu, std::span<const std::uint8_t> program)
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