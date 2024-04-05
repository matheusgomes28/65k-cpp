module;

#include <array>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <span>
#include <unordered_map>

#include <fmt/format.h>

export module emulator;

export namespace emulator
{

    struct Registers
    {
        std::uint8_t a;
        std::uint8_t x;
        std::uint8_t y;
        std::uint16_t pc;
        std::uint8_t sp;
    };

    struct Flags
    {
        bool n;
        bool v;
        bool b;
        bool d;
        bool i;
        bool z;
        bool c;
    };

    bool operator==(Flags const& lhs, Flags const& rhs)
    {
        return lhs.n == rhs.n && lhs.v == rhs.v && lhs.b == rhs.b && lhs.d == rhs.d && lhs.i == rhs.i && lhs.z == rhs.z
               && lhs.c == rhs.c;
    }

    struct Cpu
    {
        // registers (A, X, Y, SP, PC) - u8
        Registers reg{};

        // CZID B1VN
        Flags flags{};

        // memory (Zero page/first FF bytes, main memory, vram)
        std::array<std::uint8_t, 0xFFFF> mem{};
    };
} // namespace emulator

using Instruction = std::function<std::optional<std::size_t>(emulator::Cpu&, std::span<const std::uint8_t>)>;

Instruction ld_immediate(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }
        auto const value = program[cpu.reg.pc + 1];

        (cpu.reg).*reg = value;
        cpu.flags.z    = value == 0;
        cpu.flags.n    = value & 0b1000'0000;

        return std::make_optional<std::size_t>(2);
    };
}

Instruction ld_zeropage(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        // Safe to dereference cpu.mem[pos] as pos
        // will be an std::uint8_t
        auto const pos   = program[cpu.reg.pc + 1];
        auto const value = cpu.mem[pos];
        (cpu.reg).*reg   = value;
        cpu.flags.z      = value == 0;
        cpu.flags.n      = value & 0b1000'0000;

        return std::make_optional<std::size_t>(2);
    };
}

Instruction ld_zeropage_plus_reg(std::uint8_t emulator::Registers::*to, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        // TODO : Write some tests for the wrapping behaviour
        std::uint8_t const pos   = program[cpu.reg.pc + 1] + (cpu.reg).*add;
        std::uint8_t const value = cpu.mem[pos];
        (cpu.reg).*to            = value;
        cpu.flags.z              = value == 0;
        cpu.flags.n              = value & 0b1000'0000;

        return std::make_optional<std::size_t>(2);
    };
}

Instruction ld_absolute(std::uint8_t emulator::Registers::*to)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        if ((cpu.reg.pc + 2) >= program.size())
        {
            return std::nullopt;
        }


        std::uint16_t const lsb  = program[cpu.reg.pc + 1];
        std::uint16_t const hsb  = program[cpu.reg.pc + 2];
        std::uint16_t const pos  = (hsb << 8) | lsb;
        std::uint8_t const value = cpu.mem[pos];

        (cpu.reg).*to = value;
        cpu.flags.z   = value == 0;
        cpu.flags.n   = value & 0b1000'0000;
        return std::make_optional<std::size_t>(3);
    };
}

Instruction ld_absolute_plus_reg(std::uint8_t emulator::Registers::*to, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        if ((cpu.reg.pc + 2) >= program.size())
        {
            return std::nullopt;
        }

        // Do we want to put these numbers as std::uint16_t?
        std::uint16_t const lsb = program[cpu.reg.pc + 1];
        std::uint16_t const hsb = program[cpu.reg.pc + 2];

        std::uint16_t const pos  = ((hsb << 8) | lsb) + static_cast<std::uint16_t>((cpu.reg).*add);
        std::uint8_t const value = cpu.mem[pos];

        (cpu.reg).*to = value;
        cpu.flags.z   = value == 0;
        cpu.flags.n   = value & 0b1000'0000;
        return std::make_optional<std::size_t>(3);
    };
}

// This is basically zeropage + x, but an extra indirection with
// the value ad zeropage + x as a position
Instruction ld_index_indirect(std::uint8_t emulator::Registers::*to, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        // The indirection position should wrap around the zeropage
        std::uint16_t const zeropage  = program[cpu.reg.pc + 1];
        std::uint16_t const pos_index = (zeropage + static_cast<std::uint16_t>((cpu.reg).*add)) & 0xff;

        std::uint16_t const lsb  = cpu.mem[pos_index];
        std::uint16_t const hsb  = cpu.mem[pos_index + 1];
        std::uint16_t const pos  = (hsb << 8) | lsb;
        std::uint8_t const value = cpu.mem[pos];

        (cpu.reg).*to = value;
        cpu.flags.z   = value == 0;
        cpu.flags.n   = value & 0b1000'0000;
        return std::make_optional<std::size_t>(2);
    };
}

Instruction ld_indirect_index(std::uint8_t emulator::Registers::*to, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        // The indirection position should wrap around the zeropage
        std::uint16_t const zeropage = program[cpu.reg.pc + 1];
        std::uint16_t const lsb      = cpu.mem[zeropage];
        std::uint16_t const hsb      = cpu.mem[zeropage + 1];
        std::uint16_t const pos      = ((hsb << 8) | lsb) + static_cast<std::uint16_t>((cpu.reg).*add);

        std::uint8_t const value = cpu.mem[pos];

        (cpu.reg).*to = value;
        cpu.flags.z   = value == 0;
        cpu.flags.n   = value & 0b1000'0000;
        return std::make_optional<std::size_t>(2);
    };
}

std::optional<std::size_t> inc_zeropage(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    std::uint8_t const pos = program[cpu.reg.pc + 1];
    cpu.mem[pos]++;
    cpu.flags.z = cpu.mem[pos] == 0;
    cpu.flags.n = cpu.mem[pos] & 0b1000'0000;

    return std::make_optional<std::size_t>(2);
}

Instruction inc_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program)
    {
        ((cpu.reg).*reg)++;
        cpu.flags.n = (cpu.reg).*reg & 0b1000'0000;
        cpu.flags.z = (cpu.reg).*reg == 0;
        return std::make_optional<std::size_t>(1);
    };
}

Instruction dec_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program)
    {
        ((cpu.reg).*reg)--;
        cpu.flags.n = (cpu.reg).*reg & 0b1000'0000;
        cpu.flags.z = (cpu.reg).*reg == 0;
        return std::make_optional<std::size_t>(1);
    };
}

// TODO : TXS Does not set any flags
Instruction transfer_regs(std::uint8_t emulator::Registers::*from, std::uint8_t emulator::Registers::*to)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program)
    {
        (cpu.reg).*to = (cpu.reg).*from;
        cpu.flags.z   = (cpu.reg).*to == 0;
        cpu.flags.n   = (cpu.reg).*to & 0b1000'0000;
        return std::make_optional<std::size_t>(1);
    };
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
    return std::optional<std::size_t>(2);
}

std::optional<std::size_t> sta_immediate(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    // LOAD Value into accumulator
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const value = program[cpu.reg.pc + 1];
    cpu.mem[value]   = cpu.reg.a;

    return std::make_optional<std::size_t>(2);
}

/// This function will emulate the "STA" in absulute addressing
/// mode. This will store whatever value is in the accumulator
/// into the memory address obtained from the word address in the
/// program.
std::optional<std::size_t> sta_absolute(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    if ((cpu.reg.pc + 2) >= program.size())
    {
        return std::nullopt;
    }

    // (hsb << 8) + lsb convert little endian to the address
    auto const lsb            = program[cpu.reg.pc + 1];
    auto const hsb            = program[cpu.reg.pc + 2];
    cpu.mem[(hsb << 8) | lsb] = cpu.reg.a;

    return std::make_optional<std::size_t>(3);
}


// Compare instructions here

/// Compares whichever register was given to the immediate
/// value in the next address in the program array
Instruction cmp_immediate_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        auto const value      = program[cpu.reg.pc + 1];
        auto const comparison = (cpu.reg).*reg - value;
        cpu.flags.n           = comparison & 0b1000'0000; // Negative flag
        cpu.flags.z           = comparison == 0; // Negative flag
        cpu.flags.c           = (cpu.reg).*reg >= value;

        return std::make_optional<std::size_t>(2);
    };
}

Instruction cmp_zeropage_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        auto const memory     = program[cpu.reg.pc + 1];
        auto const value      = cpu.mem[memory];
        auto const comparison = (cpu.reg).*reg - value;
        cpu.flags.n           = comparison & 0b1000'0000; // Negative flag
        cpu.flags.z           = comparison == 0; // Negative flag
        cpu.flags.n           = (cpu.reg).*reg >= value;

        return std::make_optional<std::size_t>(2);
    };
}

// Branching functions here
std::optional<std::size_t> bne(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }
    auto const offset = static_cast<std::int8_t>(program[cpu.reg.pc + 1]);

    if (!cpu.flags.z)
    {
        cpu.reg.pc += offset;
    }

    return 2;
}

std::optional<std::size_t> execute_next(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    // Read 1 byte for the operator
    if (cpu.reg.pc >= program.size())
    {
        return std::nullopt;
    }

    // Byte key indicates which function we need to call
    // to handle the specific instruction
    const std::unordered_map<std::uint8_t, std::string> testing{
        {1, "Hello World"},
        {2, "Hello World"},
        {3, "Hello World"},
    };
    const std::unordered_map<std::uint8_t, Instruction> instructions{{
        {0x00, [](emulator::Cpu&, std::span<const std::uint8_t>) { return std::nullopt; }},


        // Transferring instrucitons here
        {0x8a, transfer_regs(&emulator::Registers::x, &emulator::Registers::a)}, // TXA
        {0x98, transfer_regs(&emulator::Registers::y, &emulator::Registers::a)}, // TYA
        {0xa8, transfer_regs(&emulator::Registers::a, &emulator::Registers::y)}, // TAY
        {0xaa, transfer_regs(&emulator::Registers::a, &emulator::Registers::x)}, // TAX
        {0xba, transfer_regs(&emulator::Registers::sp, &emulator::Registers::x)}, // TSX
        // TODO : The TXS does not set any arithmetic flags
        // {0x98, transfer_regs(&emulator::Registers::x, &emulator::Registers::s)}, // TXS

        // Memory storing functions here
        {0x85, sta_immediate},
        {0x8d, sta_absolute},
        {0x91, sta_ind_y},

        // TODO : Finish supporting the ld* family
        // of instructions
        {0xa9, ld_immediate(&emulator::Registers::a)},
        {0xa5, ld_zeropage(&emulator::Registers::a)},
        {0xb5, ld_zeropage_plus_reg(&emulator::Registers::a, &emulator::Registers::x)},
        {0xbd, ld_absolute_plus_reg(&emulator::Registers::a, &emulator::Registers::x)},
        {0xb9, ld_absolute_plus_reg(&emulator::Registers::a, &emulator::Registers::y)},
        {0xa1, ld_index_indirect(&emulator::Registers::a, &emulator::Registers::x)},
        {0xb1, ld_indirect_index(&emulator::Registers::a, &emulator::Registers::y)},
        {0xad, ld_absolute(&emulator::Registers::a)},
        {0xa2, ld_immediate(&emulator::Registers::x)},
        {0xa6, ld_zeropage(&emulator::Registers::x)},
        {0xb6, ld_zeropage_plus_reg(&emulator::Registers::x, &emulator::Registers::y)},
        {0xae, ld_absolute(&emulator::Registers::x)},
        {0xbe, ld_absolute_plus_reg(&emulator::Registers::x, &emulator::Registers::y)},
        {0xa0, ld_immediate(&emulator::Registers::y)},
        {0xa4, ld_zeropage(&emulator::Registers::y)},
        {0xb4, ld_zeropage_plus_reg(&emulator::Registers::y, &emulator::Registers::x)},
        {0xbc, ld_absolute_plus_reg(&emulator::Registers::y, &emulator::Registers::x)},
        {0xac, ld_absolute(&emulator::Registers::y)},

        // TODO : finish support for the cmp* family
        // of instructions
        {0xc0, cmp_immediate_reg(&emulator::Registers::y)},
        {0xc5, cmp_zeropage_reg(&emulator::Registers::a)},
        {0xe0, cmp_immediate_reg(&emulator::Registers::x)},

        // TODO : finish support for the branching
        // instructions
        {0xd0, bne},

        // TODO : finish support for the inc/dec
        // instructions
        {0xc8, inc_reg(&emulator::Registers::y)},
        {0xe8, inc_reg(&emulator::Registers::x)},
        {0xe6, inc_zeropage},
        {0x88, dec_reg(&emulator::Registers::y)},
        {0xca, dec_reg(&emulator::Registers::x)},
    }};

    // Find the correct function to execute here
    auto const command = program[cpu.reg.pc];
    auto const found   = instructions.find(command);

    if (found != end(instructions))
    {
        // Execute the function
        return found->second(cpu, program);
    }

    // Instruction not supported here
    // unsupported command
    return std::nullopt;
}

export namespace emulator
{

    bool execute(Cpu& cpu, std::span<const std::uint8_t> program)
    {
        while (cpu.reg.pc < program.size())
        {
            auto maybe_increment = execute_next(cpu, program);
            if (!maybe_increment)
            {
                return false;
            }

            cpu.reg.pc += *maybe_increment;
        }

        return true;
    }
} // namespace emulator
