module;

// #ifdef BUILD_PROFILER
import profiler;
// #endif // BUILD_PROFILER

#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <span>
#include <thread>
#include <unordered_map>
#include <utility>

#include <fmt/format.h>

#ifndef CLOCK_SPEED_MHZ
#define CLOCK_SPEED_MHZ 1.79
#endif // CLOCK_SPEED_MHZ

#include <fmt/format.h>

export module emulator;

// Define a macro for the profiler
#ifdef BUILD_PROFILER
struct ProfileBook
{
    std::unordered_map<std::string, double> functions;

    bool update(std::string const& func_name, double profile)
    {
        auto found = functions.find(func_name);
        if (found != end(functions))
        {
            // update it
            found->second += profile;
            return true;
        }

        functions[func_name] = profile;
        return true;
    }
};

#define ENABLE_PROFILER(cpu) \
    volatile auto profile_result_666 = profiler::FunctionProfiler<ProfileBook, std::string, double>(cpu.profiler_book)
#else
#define ENABLE_PROFILER(cpu)
#endif // BUILD_PROFILER

export namespace emulator
{
    class OpcodeNotSupported : public std::exception
    {
    public:
        OpcodeNotSupported(std::string opcode) : _error_msg{"opcode not supported: " + opcode} {}

        const char* what() const throw() override
        {
            return _error_msg.c_str();
        }

    private:
        std::string _error_msg;
    };

    struct Registers
    {
        std::uint8_t a;
        std::uint8_t x;
        std::uint8_t y;
        std::uint16_t pc;
        std::uint8_t sp{0xff};
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

        // lb-----hb
        // CZID B1VN
        Flags flags{};

        // memory (Zero page/first FF bytes, main memory, vram)
        std::array<std::uint8_t, 0xFFFF> mem{};

        // Clock speed for this particular CPU
        double clock_speed = CLOCK_SPEED_MHZ;

#ifdef BUILD_PROFILER
        // If profiling is enabled, create space for a bookeper
        std::shared_ptr<ProfileBook> profiler_book{std::make_shared<ProfileBook>()};

        std::unordered_map<std::string, double> current_profile()
        {
            if (!profiler_book)
            {
                return {};
            }
            return profiler_book->functions;
        }
#else
        std::unordered_map<std::string, double> current_profile()
        {
            return {};
        }
#endif // BUILD_PROFILER
    };
} // namespace emulator

// TODO : give this a better name
export struct InstructionConfig
{
    std::size_t bytes;
    std::size_t cycles;

    InstructionConfig(std::size_t bytes_read) : bytes{bytes_read}, cycles{0} {}

    InstructionConfig(std::size_t bytes_read, std::size_t cycles) : bytes{bytes_read}, cycles{cycles} {}
};

using Instruction = std::function<std::optional<InstructionConfig>(emulator::Cpu&, std::span<const std::uint8_t>)>;


/*
    Addressing helpers - zeropage + reg, indexed indirect, indirect indexed helpers
*/


/// @brief this function aids getting the indexed zeropage address
/// from the given argument to the opcode
/// @param cpu is the cpu object to operate on
/// @param value is the zeropage address to add the index to
/// @param index is the register to use as the index add value
inline std::uint16_t zeropage_indexed(emulator::Cpu& cpu, std::uint8_t value, std::uint8_t emulator::Registers::*index)
{
    auto const masked = ((cpu.reg).*index + value) & 0xff;
    return static_cast<std::uint16_t>(masked);
}

/// @brief this function aids in getting the absolute indexed
/// address for absolute indexed addressing opcodes.
/// @param cpu is the cpu object to operate on
/// @param lsb is the low significant bits, or the first byte after the opcodes.
/// @param hsb is the high significant bits, or the second byte after the opcodes.
/// @param index is the pointer to the register to use as the index add.
/// @return the resolved target address.
inline std::uint16_t absolute_indexed(
    emulator::Cpu& cpu, std::uint8_t lsb, std::uint8_t hsb, std::uint8_t emulator::Registers::*index)
{
    // Do we want to put these numbers as std::uint16_t?
    auto const address        = (hsb << 8) | lsb;
    auto const target_address = address + (cpu.reg).*index;
    return static_cast<std::uint16_t>(target_address & 0xffff);
}

/// @brief This function aids with the fetching of an indexed indirect
/// opcode mode.
/// @param cpu the cpu with the memory to operate on
/// @param val the zeropage memory to apply the index to
/// @return the target address
inline std::uint16_t indexed_indirect(emulator::Cpu& cpu, std::uint8_t val)
{
    // (hsb << 8) + lsb convert little endian to the address
    std::uint16_t const indexed_address = static_cast<std::uint16_t>(val + cpu.reg.x);
    std::uint16_t const zp_addr         = static_cast<std::uint16_t>(indexed_address & 0xff);

    auto const lsb     = cpu.mem[zp_addr];
    auto const hsb_pos = static_cast<std::uint16_t>((zp_addr + 1) & 0xff);
    auto const hsb     = cpu.mem[hsb_pos];
    auto const addr    = static_cast<std::uint16_t>((hsb << 8) | lsb);
    return addr;
}

/// @brief This function aids with the fetching of an indirect indexed
/// opcode mode.
/// @param cpu the cpu with the memory to operate on
/// @param val the zeropage memory to fetch the indirect address
/// @return the target address
inline std::uint16_t indirect_indexed(emulator::Cpu& cpu, std::uint8_t val)
{
    auto const lsb      = cpu.mem[static_cast<std::uint8_t>(val)];
    auto const hsb_addr = static_cast<std::uint16_t>((val + 1) & 0xff);
    auto const hsb      = cpu.mem[hsb_addr];
    auto const addr     = static_cast<std::uint16_t>(((hsb << 8) | lsb) + cpu.reg.y);
    return addr;
}

// TODO : can we somehow abstract these two stack pushing functions?
/* Stack Related Functions */
std::optional<InstructionConfig> push_accumulator_to_stack(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    // TODO : can we detect stack overflows?
    // TODO : according to Masswerk, we don't set any flags

    std::uint16_t const mem_loc = static_cast<std::uint16_t>(0x0100) + static_cast<std::uint16_t>(cpu.reg.sp);
    cpu.mem[mem_loc] = cpu.reg.a;
    --cpu.reg.sp;

    return std::make_optional<InstructionConfig>(1, 0);
}

std::optional<InstructionConfig> push_status_register_to_stack(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    // TODO : can we detect stack overflows?
    // TODO : according to Masswerk, we don't set any flags

    std::uint16_t const mem_loc = static_cast<std::uint16_t>(0x0100) + static_cast<std::uint16_t>(cpu.reg.sp);
    cpu.mem[mem_loc] = cpu.reg.a;
    --cpu.reg.sp;

    return std::make_optional<InstructionConfig>(1, 0);
}
/* End of Stack Related Functions */


Instruction ld_immediate(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }
        auto const value = program[cpu.reg.pc + 1];

        (cpu.reg).*reg = value;
        cpu.flags.z    = value == 0;
        cpu.flags.n    = value & 0b1000'0000;

        return std::make_optional<InstructionConfig>(2);
    };
}

Instruction ld_zeropage(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
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

        return std::make_optional<InstructionConfig>(2);
    };
}

/// @brief this function will load the value in the given memory
/// to the address acquired from the register given plus the index
/// given.
/// @param to the destination register where the value will be loaded into.
/// @param add the register to use as the index add.
/// @return Instruction containing the number of bytes consumed from the
/// program and the cycles taken.
Instruction ld_zeropage_indexed(std::uint8_t emulator::Registers::*to, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);

        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        // TODO : Write some tests for the wrapping behaviour
        std::uint16_t const pos  = zeropage_indexed(cpu, program[cpu.reg.pc + 1], add);
        std::uint8_t const value = cpu.mem[pos];
        (cpu.reg).*to            = value;
        cpu.flags.z              = value == 0;
        cpu.flags.n              = value & 0b1000'0000;

        return std::make_optional<InstructionConfig>(2);
    };
}

Instruction ld_absolute(std::uint8_t emulator::Registers::*to)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
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
        return std::make_optional<InstructionConfig>(3);
    };
}

Instruction ld_absolute_plus_reg(std::uint8_t emulator::Registers::*to, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        if ((cpu.reg.pc + 2) >= program.size())
        {
            return std::nullopt;
        }

        // Do we want to put these numbers as std::uint16_t?
        std::uint16_t const pos  = absolute_indexed(cpu, program[cpu.reg.pc + 1], program[cpu.reg.pc + 2], add);
        std::uint8_t const value = cpu.mem[pos];

        (cpu.reg).*to = value;
        cpu.flags.z   = value == 0;
        cpu.flags.n   = value & 0b1000'0000;
        return std::make_optional<InstructionConfig>(3);
    };
}

// This is basically zeropage + x, but an extra indirection with
// the value ad zeropage + x as a position
Instruction ld_index_indirect(std::uint8_t emulator::Registers::*to, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
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
        return std::make_optional<InstructionConfig>(2);
    };
}

Instruction ld_indirect_index(std::uint8_t emulator::Registers::*to)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        auto const pos           = indirect_indexed(cpu, program[cpu.reg.pc + 1]);
        std::uint8_t const value = cpu.mem[pos];

        (cpu.reg).*to = value;
        cpu.flags.z   = value == 0;
        cpu.flags.n   = value & 0b1000'0000;
        return std::make_optional<InstructionConfig>(2);
    };
}

std::optional<InstructionConfig> inc_zeropage(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    std::uint8_t const pos = program[cpu.reg.pc + 1];
    cpu.mem[pos]++;
    cpu.flags.z = cpu.mem[pos] == 0;
    cpu.flags.n = cpu.mem[pos] & 0b1000'0000;

    return std::make_optional<InstructionConfig>(2, 5);
}

std::optional<InstructionConfig> inc_zeropage_plus_x(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    std::uint16_t const pos = zeropage_indexed(cpu, program[cpu.reg.pc + 1], &emulator::Registers::x);

    cpu.mem[pos]++;
    cpu.flags.z = cpu.mem[pos] == 0;
    cpu.flags.n = cpu.mem[pos] & 0b1000'0000;

    return std::make_optional<InstructionConfig>(2, 6);
}

std::optional<InstructionConfig> inc_absolute(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 2) >= program.size())
    {
        return std::nullopt;
    }

    std::uint8_t const lsb  = program[cpu.reg.pc + 1];
    std::uint8_t const hsb  = program[cpu.reg.pc + 2];
    std::uint16_t const pos = (hsb << 8) | lsb;
    cpu.mem[pos]++;
    cpu.flags.z = cpu.mem[pos] == 0;
    cpu.flags.n = cpu.mem[pos] & 0b1000'0000;

    return std::make_optional<InstructionConfig>(3, 6);
}

std::optional<InstructionConfig> inc_absolute_plus_x(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);

    if ((cpu.reg.pc + 2) >= program.size())
    {
        return std::nullopt;
    }

    std::uint16_t const pos =
        absolute_indexed(cpu, program[cpu.reg.pc + 1], program[cpu.reg.pc + 2], &emulator::Registers::x);

    cpu.mem[pos]++;
    cpu.flags.z = cpu.mem[pos] == 0;
    cpu.flags.n = cpu.mem[pos] & 0b1000'0000;

    return std::make_optional<InstructionConfig>(3, 7);
}

std::optional<InstructionConfig> dec_zeropage(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    std::uint8_t const pos = program[cpu.reg.pc + 1];
    cpu.mem[pos]--;
    cpu.flags.z = cpu.mem[pos] == 0;
    cpu.flags.n = cpu.mem[pos] & 0b1000'0000;

    return std::make_optional<InstructionConfig>(2);
}

Instruction inc_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> /* program */)
    {
        ENABLE_PROFILER(cpu);
        ((cpu.reg).*reg)++;
        cpu.flags.n = (cpu.reg).*reg & 0b1000'0000;
        cpu.flags.z = (cpu.reg).*reg == 0;
        return std::make_optional<InstructionConfig>(1, 2);
    };
}

Instruction dec_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> /* program */)
    {
        ENABLE_PROFILER(cpu);
        ((cpu.reg).*reg)--;
        cpu.flags.n = (cpu.reg).*reg & 0b1000'0000;
        cpu.flags.z = (cpu.reg).*reg == 0;
        return std::make_optional<InstructionConfig>(1, 2);
    };
}

Instruction transfer_regs(std::uint8_t emulator::Registers::*from, std::uint8_t emulator::Registers::*to)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> /* program */)
    {
        ENABLE_PROFILER(cpu);
        (cpu.reg).*to = (cpu.reg).*from;
        cpu.flags.z   = (cpu.reg).*to == 0;
        cpu.flags.n   = (cpu.reg).*to & 0b1000'0000;
        return std::make_optional<InstructionConfig>(1);
    };
}

// This function sends the value stored in X to SP and
// does not set any flags.
std::optional<InstructionConfig> txa(emulator::Cpu& cpu, std::span<const std::uint8_t> /* program */)
{
    ENABLE_PROFILER(cpu);
    cpu.reg.sp = cpu.reg.x;
    return std::make_optional<InstructionConfig>(1);
}

Instruction st_indirect(std::uint8_t emulator::Registers::*from, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }
        auto const word_pos = program[cpu.reg.pc + 1];

        // TODO : This is unsafe
        auto const lsb = cpu.mem[word_pos];
        auto const hsb = cpu.mem[word_pos + 1];
        auto const pos = (hsb << 8) + lsb + (cpu.reg).*add;
        // TODO : What happens if we zeropage overflow?

        // TODO : this needs a bounds check here
        // can probably return nullopt
        cpu.mem[pos] = (cpu.reg).*from;
        return std::make_optional<InstructionConfig>(2, 6);
    };
}

Instruction st_zeropage(std::uint8_t emulator::Registers::*from)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        // LOAD Value into accumulator
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        auto const value = program[cpu.reg.pc + 1];
        cpu.mem[value]   = (cpu.reg).*from;

        return std::make_optional<InstructionConfig>(2, 3);
    };
}

/// @brief this function will store the value of the given register
/// to the zeropage + index memory location given by the program
/// arguments.
/// @param from is the register containing the value to be stored in memory.
/// @param index is the register used as the index (i.e. X or Y mostly).
Instruction st_zeropage_indexed(std::uint8_t emulator::Registers::*from, std::uint8_t emulator::Registers::*index)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        // LOAD Value into accumulator
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        auto const pos = zeropage_indexed(cpu, program[cpu.reg.pc + 1], index);
        cpu.mem[pos]   = (cpu.reg).*from;

        return std::make_optional<InstructionConfig>(2, 3);
    };
}

Instruction sta_absolute_indexed(std::uint8_t emulator::Registers::*index)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        // LOAD Value into accumulator
        if ((cpu.reg.pc + 2) >= program.size())
        {
            return std::nullopt;
        }

        auto const pos = absolute_indexed(cpu, program[cpu.reg.pc + 1], program[cpu.reg.pc + 2], index);
        cpu.mem[pos]   = cpu.reg.a;

        // TODO : Return correct number of cycles
        return std::make_optional<InstructionConfig>(3, 0);
    };
}

Instruction st_absolute(std::uint8_t emulator::Registers::*from)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        if ((cpu.reg.pc + 2) >= program.size())
        {
            return std::nullopt;
        }

        // (hsb << 8) + lsb convert little endian to the address
        auto const lsb            = program[cpu.reg.pc + 1];
        auto const hsb            = program[cpu.reg.pc + 2];
        cpu.mem[(hsb << 8) | lsb] = (cpu.reg).*from;

        return std::make_optional<InstructionConfig>(3, 4);
    };
}

// Compare instructions here

/// Compares whichever register was given to the immediate
/// value in the next address in the program array
Instruction cmp_immediate_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        auto const value      = program[cpu.reg.pc + 1];
        auto const comparison = (cpu.reg).*reg - value;
        cpu.flags.n           = comparison & 0b1000'0000; // Negative flag
        cpu.flags.z           = comparison == 0; // Negative flag
        cpu.flags.c           = (cpu.reg).*reg >= value;

        return std::make_optional<InstructionConfig>(2);
    };
}

Instruction cmp_zeropage_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);

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

        return std::make_optional<InstructionConfig>(2);
    };
}

// Branching functions here
std::optional<InstructionConfig> bne(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }
    auto const offset = static_cast<std::int8_t>(program[cpu.reg.pc + 1]);
    cpu.reg.pc += cpu.flags.z ? 0 : offset;

    return std::make_optional<InstructionConfig>(2);
}

// TODO : Need to add the cycles based on whether we branch on the
//        same page or if we cross the page boundary
//        - 2 cycles if the branch isn't taken
//        - 3 cycles if the branch is in the same page
//        - 4 cycles if the branch is on a different page
//        Can encode this by using a Cpu.n_cycles variable to keep the cycles number
//        OR we start returning a tuple with the (bytes_consumed, n_cycles) per ins.
template <bool Value>
Instruction branch_flag_value(bool emulator::Flags::*flag)
{
    // TODO : Do we need a return here?
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        auto const offset = ((cpu.flags).*flag == Value) ? static_cast<std::int8_t>(program[cpu.reg.pc + 1]) : 0;
        return std::make_optional<InstructionConfig>(2 + offset);
    };
}

/// common code for the or related opcodes
template <std::size_t T>
inline std::optional<std::size_t> ora_operation(emulator::Cpu& cpu, std::uint8_t value)
{
    cpu.reg.a   = cpu.reg.a | value;
    cpu.flags.n = 0b1000'0000 & cpu.reg.a;
    cpu.flags.z = !static_cast<bool>(cpu.reg.a);
    return std::make_optional<std::size_t>(T);
}

template <std::size_t T>
inline std::optional<std::size_t> and_operation(emulator::Cpu& cpu, std::uint8_t value)
{
    cpu.reg.a   = cpu.reg.a & value;
    cpu.flags.n = 0b1000'0000 & cpu.reg.a;
    cpu.flags.z = !static_cast<bool>(cpu.reg.a);
    return std::make_optional<std::size_t>(T);
}

template <std::size_t T>
std::optional<std::size_t> eor_operation(emulator::Cpu& cpu, std::uint8_t value)
{
    cpu.reg.a   = cpu.reg.a ^ value;
    cpu.flags.n = 0b1000'0000 & cpu.reg.a;
    cpu.flags.z = !static_cast<bool>(cpu.reg.a);
    return std::make_optional<std::size_t>(T);
}

// Logical operations
std::optional<std::size_t> eor_acc_immediate(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    return eor_operation<2>(cpu, program[cpu.reg.pc + 1]);
}

std::optional<std::size_t> eor_acc_zeropage(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const offset = program[cpu.reg.pc + 1];
    return eor_operation<2>(cpu, cpu.mem[offset]);
}

std::optional<std::size_t> eor_acc_zeropage_x(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    std::uint16_t const pos = zeropage_indexed(cpu, program[cpu.reg.pc + 1], &emulator::Registers::x);
    return eor_operation<2>(cpu, cpu.mem[pos]);
}

std::optional<std::size_t> eor_acc_absolute(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 2) >= program.size())
    {
        return std::nullopt;
    }

    // (hsb << 8) + lsb convert little endian to the address
    auto const lsb  = program[cpu.reg.pc + 1];
    auto const hsb  = program[cpu.reg.pc + 2];
    auto const addr = static_cast<std::uint16_t>((hsb << 8) | lsb);

    return eor_operation<3>(cpu, cpu.mem[addr]);
}

Instruction eor_acc_absolute_plus_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        if ((cpu.reg.pc + 2) >= program.size())
        {
            return std::nullopt;
        }

        auto const addr = absolute_indexed(cpu, program[cpu.reg.pc + 1], program[cpu.reg.pc + 2], reg);

        return eor_operation<3>(cpu, cpu.mem[addr]);
    };
}

std::optional<std::size_t> eor_acc_indexed_indirect(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const addr = indexed_indirect(cpu, program[cpu.reg.pc + 1]);
    return eor_operation<2>(cpu, cpu.mem[addr]);
}

std::optional<std::size_t> eor_acc_indirect_indexed(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const addr = indirect_indexed(cpu, program[cpu.reg.pc + 1]);
    return eor_operation<2>(cpu, cpu.mem[addr]);
}

std::optional<std::size_t> and_acc_immediate(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    return and_operation<2>(cpu, program[cpu.reg.pc + 1]);
}

std::optional<std::size_t> and_acc_zeropage(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const offset = program[cpu.reg.pc + 1];
    return and_operation<2>(cpu, cpu.mem[offset]);
}

std::optional<std::size_t> and_acc_zeropage_x(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    std::uint16_t const pos = zeropage_indexed(cpu, program[cpu.reg.pc + 1], &emulator::Registers::x);
    return and_operation<2>(cpu, cpu.mem[pos]);
}

std::optional<std::size_t> and_acc_absolute(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 2) >= program.size())
    {
        return std::nullopt;
    }

    // (hsb << 8) + lsb convert little endian to the address
    auto const lsb  = program[cpu.reg.pc + 1];
    auto const hsb  = program[cpu.reg.pc + 2];
    auto const addr = static_cast<std::uint16_t>((hsb << 8) | lsb);

    return and_operation<3>(cpu, cpu.mem[addr]);
}

Instruction and_acc_absolute_plus_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        if ((cpu.reg.pc + 2) >= program.size())
        {
            return std::nullopt;
        }

        auto const addr = absolute_indexed(cpu, program[cpu.reg.pc + 1], program[cpu.reg.pc + 2], reg);

        return and_operation<3>(cpu, cpu.mem[addr]);
    };
}

std::optional<std::size_t> and_acc_indexed_indirect(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const addr = indexed_indirect(cpu, program[cpu.reg.pc + 1]);
    return and_operation<2>(cpu, cpu.mem[addr]);
}

std::optional<std::size_t> and_acc_indirect_indexed(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const addr = indirect_indexed(cpu, program[cpu.reg.pc + 1]);
    return and_operation<2>(cpu, cpu.mem[addr]);
}

std::optional<std::size_t> or_acc_immediate(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    return ora_operation<2>(cpu, program[cpu.reg.pc + 1]);
}

std::optional<std::size_t> or_acc_zeropage(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const offset = program[cpu.reg.pc + 1];
    return ora_operation<2>(cpu, cpu.mem[offset]);
}

std::optional<std::size_t> or_acc_zeropage_x(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    std::uint16_t const pos = zeropage_indexed(cpu, program[cpu.reg.pc + 1], &emulator::Registers::x);
    return ora_operation<2>(cpu, cpu.mem[pos]);
}

std::optional<std::size_t> or_acc_absolute(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 2) >= program.size())
    {
        return std::nullopt;
    }

    // (hsb << 8) + lsb convert little endian to the address
    auto const lsb  = program[cpu.reg.pc + 1];
    auto const hsb  = program[cpu.reg.pc + 2];
    auto const addr = static_cast<std::uint16_t>((hsb << 8) | lsb);

    return ora_operation<3>(cpu, cpu.mem[addr]);
}

Instruction or_acc_absolute_plus_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<InstructionConfig>
    {
        ENABLE_PROFILER(cpu);
        if ((cpu.reg.pc + 2) >= program.size())
        {
            return std::nullopt;
        }

        auto const addr = absolute_indexed(cpu, program[cpu.reg.pc + 1], program[cpu.reg.pc + 2], reg);

        return ora_operation<3>(cpu, cpu.mem[addr]);
    };
}

std::optional<std::size_t> or_acc_indexed_indirect(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const addr = indexed_indirect(cpu, program[cpu.reg.pc + 1]);
    return ora_operation<2>(cpu, cpu.mem[addr]);
}

std::optional<std::size_t> or_acc_indirect_index(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    auto const addr = indirect_indexed(cpu, program[cpu.reg.pc + 1]);
    return ora_operation<2>(cpu, cpu.mem[addr]);
}

// TODO : provide support for counting the number of cycles passed
// from the start of the program
std::array<Instruction, 256> get_instructions()
{
    // TODO : Goal is to have around all ~154 instructions supported

    // Byte key indicates which function we need to call
    // to handle the specific instruction
    // using Instruction = std::function<std::optional<std::size_t>(emulator::Cpu&, std::span<const std::uint8_t>)>;
    std::array<Instruction, 256> supported_instructions{};
    for (std::size_t i = 0; i < supported_instructions.size(); ++i)
    {
        supported_instructions[i] = [=](emulator::Cpu&, std::span<const std::uint8_t>) -> std::optional<std::size_t>
        { throw emulator::OpcodeNotSupported(fmt::format("{0:#x}", i)); };
    }
    // make sure that all default elemets of supported_instructions
    // are functions that will raise an error

    // Supported instructions
    supported_instructions[0x00] = [](emulator::Cpu&, std::span<const std::uint8_t>) { return std::nullopt; };
    supported_instructions[0x8a] = transfer_regs(&emulator::Registers::x, &emulator::Registers::a);
    supported_instructions[0x98] = transfer_regs(&emulator::Registers::y, &emulator::Registers::a);
    supported_instructions[0xa8] = transfer_regs(&emulator::Registers::a, &emulator::Registers::y);
    supported_instructions[0xaa] = transfer_regs(&emulator::Registers::a, &emulator::Registers::x);
    supported_instructions[0xba] = transfer_regs(&emulator::Registers::sp, &emulator::Registers::x);
    supported_instructions[0x9a] = txa;

    // STA instructions
    supported_instructions[0x85] = st_zeropage(&emulator::Registers::a);
    supported_instructions[0x8d] = st_absolute(&emulator::Registers::a);
    supported_instructions[0x91] = st_indirect(&emulator::Registers::a, &emulator::Registers::y);
    supported_instructions[0x95] = st_zeropage_indexed(&emulator::Registers::a, &emulator::Registers::x);
    supported_instructions[0x99] = sta_absolute_indexed(&emulator::Registers::y);
    supported_instructions[0x9d] = sta_absolute_indexed(&emulator::Registers::x);

    // STX Instructions
    supported_instructions[0x86] = st_zeropage(&emulator::Registers::x);
    supported_instructions[0x8e] = st_absolute(&emulator::Registers::x);
    supported_instructions[0x96] = st_zeropage_indexed(&emulator::Registers::x, &emulator::Registers::y);

    // STY Isntructions
    supported_instructions[0x84] = st_zeropage(&emulator::Registers::y);
    supported_instructions[0x8c] = st_absolute(&emulator::Registers::y);
    supported_instructions[0x94] = st_zeropage_indexed(&emulator::Registers::y, &emulator::Registers::x);

    supported_instructions[0xa9] = ld_immediate(&emulator::Registers::a);
    supported_instructions[0xa5] = ld_zeropage(&emulator::Registers::a);
    supported_instructions[0xb5] = ld_zeropage_indexed(&emulator::Registers::a, &emulator::Registers::x);
    supported_instructions[0xbd] = ld_absolute_plus_reg(&emulator::Registers::a, &emulator::Registers::x);
    supported_instructions[0xb9] = ld_absolute_plus_reg(&emulator::Registers::a, &emulator::Registers::y);
    supported_instructions[0xa1] = ld_index_indirect(&emulator::Registers::a, &emulator::Registers::x);
    supported_instructions[0xb1] = ld_indirect_index(&emulator::Registers::a);
    supported_instructions[0xad] = ld_absolute(&emulator::Registers::a);
    supported_instructions[0xa2] = ld_immediate(&emulator::Registers::x);
    supported_instructions[0xa6] = ld_zeropage(&emulator::Registers::x);
    supported_instructions[0xb6] = ld_zeropage_indexed(&emulator::Registers::x, &emulator::Registers::y);
    supported_instructions[0xae] = ld_absolute(&emulator::Registers::x);
    supported_instructions[0xbe] = ld_absolute_plus_reg(&emulator::Registers::x, &emulator::Registers::y);
    supported_instructions[0xa0] = ld_immediate(&emulator::Registers::y);
    supported_instructions[0xa4] = ld_zeropage(&emulator::Registers::y);
    supported_instructions[0xb4] = ld_zeropage_indexed(&emulator::Registers::y, &emulator::Registers::x);
    supported_instructions[0xbc] = ld_absolute_plus_reg(&emulator::Registers::y, &emulator::Registers::x);
    supported_instructions[0xac] = ld_absolute(&emulator::Registers::y);

    // This is the immediate mode compares to registers
    supported_instructions[0xc9] = cmp_immediate_reg(&emulator::Registers::a); // TODO : test
    supported_instructions[0xc0] = cmp_immediate_reg(&emulator::Registers::y);
    supported_instructions[0xe0] = cmp_immediate_reg(&emulator::Registers::x);

    // TODO : support for compare zeropage simple
    supported_instructions[0xc5] = cmp_zeropage_reg(&emulator::Registers::a);
    supported_instructions[0xe4] = cmp_zeropage_reg(&emulator::Registers::x); // TODO : test
    supported_instructions[0xc4] = cmp_zeropage_reg(&emulator::Registers::y); // TODO : test

    supported_instructions[0xf0] = branch_flag_value<true>(&emulator::Flags::z);
    supported_instructions[0xd0] = branch_flag_value<false>(&emulator::Flags::z);
    supported_instructions[0x30] = branch_flag_value<true>(&emulator::Flags::n);
    supported_instructions[0x10] = branch_flag_value<false>(&emulator::Flags::n);
    supported_instructions[0xb0] = branch_flag_value<true>(&emulator::Flags::c);
    supported_instructions[0x90] = branch_flag_value<false>(&emulator::Flags::c);
    supported_instructions[0x70] = branch_flag_value<true>(&emulator::Flags::v);
    supported_instructions[0x50] = branch_flag_value<false>(&emulator::Flags::v);

    supported_instructions[0xe6] = inc_zeropage;
    supported_instructions[0xf6] = inc_zeropage_plus_x;
    supported_instructions[0xee] = inc_absolute;
    supported_instructions[0xfe] = inc_absolute_plus_x;
    supported_instructions[0xc8] = inc_reg(&emulator::Registers::y);
    supported_instructions[0xe8] = inc_reg(&emulator::Registers::x);

    supported_instructions[0xc6] = dec_zeropage;
    supported_instructions[0x88] = dec_reg(&emulator::Registers::y);
    supported_instructions[0xca] = dec_reg(&emulator::Registers::x);

    // ORA opcodes
    supported_instructions[0x05] = or_acc_zeropage;
    supported_instructions[0x09] = or_acc_immediate;
    supported_instructions[0x15] = or_acc_zeropage_x;
    supported_instructions[0x0d] = or_acc_absolute;
    supported_instructions[0x1d] = or_acc_absolute_plus_reg(&emulator::Registers::x);
    supported_instructions[0x19] = or_acc_absolute_plus_reg(&emulator::Registers::y);
    supported_instructions[0x01] = or_acc_indexed_indirect;
    supported_instructions[0x11] = or_acc_indirect_index;

    // AND opcodes
    supported_instructions[0x21] = and_acc_indexed_indirect;
    supported_instructions[0x25] = and_acc_zeropage;
    supported_instructions[0x29] = and_acc_immediate;
    supported_instructions[0x2d] = and_acc_absolute;
    supported_instructions[0x31] = and_acc_indirect_indexed;
    supported_instructions[0x35] = and_acc_zeropage_x;
    supported_instructions[0x39] = and_acc_absolute_plus_reg(&emulator::Registers::y);
    supported_instructions[0x3d] = and_acc_absolute_plus_reg(&emulator::Registers::x);

    // EOR opcodes
    supported_instructions[0x49] = eor_acc_immediate;
    supported_instructions[0x45] = eor_acc_zeropage;
    supported_instructions[0x55] = eor_acc_zeropage_x;
    supported_instructions[0x4d] = eor_acc_absolute;
    supported_instructions[0x5d] = eor_acc_absolute_plus_reg(&emulator::Registers::x);
    supported_instructions[0x59] = eor_acc_absolute_plus_reg(&emulator::Registers::y);
    supported_instructions[0x41] = eor_acc_indexed_indirect;
    supported_instructions[0x51] = eor_acc_indirect_indexed;

    // Stack-related opcodes
    supported_instructions[0x48] = push_accumulator_to_stack;

    return supported_instructions;
}

std::optional<InstructionConfig> execute_next(
    emulator::Cpu& cpu, std::span<const std::uint8_t> program, std::array<Instruction, 256> instructions)
{
    ENABLE_PROFILER(cpu);
    // Read 1 byte for the operator
    if (cpu.reg.pc >= program.size())
    {
        return std::nullopt;
    }

    // Find the correct function to execute here
    auto const command = program[cpu.reg.pc];


    auto const instruction = instructions[command];
    try
    {
        return instruction(cpu, program);
    }
    catch (emulator::OpcodeNotSupported const& e)
    {
        std::cout << e.what() << std::endl;
        return std::nullopt;
    }
}


export namespace emulator
{

    std::size_t execute(Cpu& cpu, std::span<const std::uint8_t> program)
    {
        std::size_t n_cycles = 0;
        ENABLE_PROFILER(cpu);
        auto const instructions = get_instructions();
        while (cpu.reg.pc < program.size())
        {
            auto const time_now  = std::chrono::high_resolution_clock::now();
            auto maybe_increment = execute_next(cpu, program, instructions);
            if (!maybe_increment)
            {
                return false;
            }

            cpu.reg.pc += maybe_increment->bytes;

            // TODO : wait for the time the instruction
            // should actually take here
            double const cycles_taken = maybe_increment->cycles;
            n_cycles += cycles_taken;

            double const cycles_per_second = cpu.clock_speed * 1'000'000;
            double const time_to_wait_s    = cycles_taken / cycles_per_second;

            // TODO : use a high resolution clock to wait for
            // the right amount of time
            auto const time_then         = std::chrono::high_resolution_clock::now();
            auto const cpp_time_overhead = time_then - time_now;
            auto const wait_duration =
                std::chrono::nanoseconds{static_cast<std::size_t>(time_to_wait_s * 1'000'000'000)};
            std::this_thread::sleep_for(wait_duration - cpp_time_overhead);
        }

        return n_cycles;
    }
} // namespace emulator
