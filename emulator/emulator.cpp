module;

// #ifdef BUILD_PROFILER
import profiler;
// #endif // BUILD_PROFILER

#include <array>
#include <algorithm>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <span>
#include <unordered_map>

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
    class OpcodeNotSupported: public std::exception
    {
    public:
        OpcodeNotSupported(std::string opcode)
            : _error_msg{"opcode not supported: " + opcode}
        {
        }

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

        // lb-----hb
        // CZID B1VN
        Flags flags{};

        // memory (Zero page/first FF bytes, main memory, vram)
        std::array<std::uint8_t, 0xFFFF> mem{};

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

using Instruction = std::function<std::optional<std::size_t>(emulator::Cpu&, std::span<const std::uint8_t>)>;

Instruction ld_immediate(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
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

        return std::make_optional<std::size_t>(2);
    };
}

Instruction ld_zeropage(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
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

        return std::make_optional<std::size_t>(2);
    };
}

Instruction ld_zeropage_plus_reg(std::uint8_t emulator::Registers::*to, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        ENABLE_PROFILER(cpu);

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
        return std::make_optional<std::size_t>(3);
    };
}

Instruction ld_absolute_plus_reg(std::uint8_t emulator::Registers::*to, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        ENABLE_PROFILER(cpu);
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
        return std::make_optional<std::size_t>(2);
    };
}

Instruction ld_indirect_index(std::uint8_t emulator::Registers::*to, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        ENABLE_PROFILER(cpu);
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
    ENABLE_PROFILER(cpu);
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

std::optional<std::size_t> inc_zeropage_plus_x(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }

    std::uint8_t const pos = program[cpu.reg.pc + 1] + cpu.reg.x;
    cpu.mem[pos]++;
    cpu.flags.z = cpu.mem[pos] == 0;
    cpu.flags.n = cpu.mem[pos] & 0b1000'0000;

    return std::make_optional<std::size_t>(2);
}

std::optional<std::size_t> inc_absolute(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
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

    return std::make_optional<std::size_t>(3);
}

std::optional<std::size_t> inc_absolute_plus_x(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);

    if ((cpu.reg.pc + 2) >= program.size())
    {
        return std::nullopt;
    }

    std::uint8_t const lsb      = program[cpu.reg.pc + 1];
    std::uint8_t const hsb      = program[cpu.reg.pc + 2];
    std::uint16_t const address = (static_cast<std::uint16_t>(hsb) << 8) | static_cast<std::uint16_t>(lsb);
    std::uint16_t const pos     = address + static_cast<std::uint16_t>(cpu.reg.x);

    cpu.mem[pos]++;
    cpu.flags.z = cpu.mem[pos] == 0;
    cpu.flags.n = cpu.mem[pos] & 0b1000'0000;

    return std::make_optional<std::size_t>(3);
}

std::optional<std::size_t> dec_zeropage(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
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

    return std::make_optional<std::size_t>(2);
}

Instruction inc_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> /* program */)
    {
        ENABLE_PROFILER(cpu);
        ((cpu.reg).*reg)++;
        cpu.flags.n = (cpu.reg).*reg & 0b1000'0000;
        cpu.flags.z = (cpu.reg).*reg == 0;
        return std::make_optional<std::size_t>(1);
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
        return std::make_optional<std::size_t>(1);
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
        return std::make_optional<std::size_t>(1);
    };
}

// This function sends the value stored in X to SP and
// does not set any flags.
std::optional<std::size_t> txa(emulator::Cpu& cpu, std::span<const std::uint8_t> /* program */)
{
    ENABLE_PROFILER(cpu);
    cpu.reg.sp = cpu.reg.x;
    return std::make_optional<std::size_t>(1);
}

Instruction st_indirect(std::uint8_t emulator::Registers::*from, std::uint8_t emulator::Registers::*add)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
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
        return std::optional<std::size_t>(2);
    };
}

Instruction st_zeropage(std::uint8_t emulator::Registers::*from)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        ENABLE_PROFILER(cpu);
        // LOAD Value into accumulator
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        auto const value = program[cpu.reg.pc + 1];
        cpu.mem[value]   = (cpu.reg).*from;

        return std::make_optional<std::size_t>(2);
    };
}

Instruction st_absolute(std::uint8_t emulator::Registers::*from)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
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

        return std::make_optional<std::size_t>(3);
    };
}

// Compare instructions here

/// Compares whichever register was given to the immediate
/// value in the next address in the program array
Instruction cmp_immediate_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
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

        return std::make_optional<std::size_t>(2);
    };
}

Instruction cmp_zeropage_reg(std::uint8_t emulator::Registers::*reg)
{
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
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

        return std::make_optional<std::size_t>(2);
    };
}

// Branching functions here
std::optional<std::size_t> bne(emulator::Cpu& cpu, std::span<const std::uint8_t> program)
{
    ENABLE_PROFILER(cpu);
    if ((cpu.reg.pc + 1) >= program.size())
    {
        return std::nullopt;
    }
    auto const offset = static_cast<std::int8_t>(program[cpu.reg.pc + 1]);
    cpu.reg.pc += cpu.flags.z ? 0 : offset;

    return std::make_optional<std::size_t>(2);
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
    return [=](emulator::Cpu& cpu, std::span<const std::uint8_t> program) -> std::optional<std::size_t>
    {
        ENABLE_PROFILER(cpu);
        if ((cpu.reg.pc + 1) >= program.size())
        {
            return std::nullopt;
        }

        auto const offset = ((cpu.flags).*flag == Value) ? static_cast<std::int8_t>(program[cpu.reg.pc + 1]) : 0;
        return std::make_optional<std::size_t>(2 + offset);
    };
}

// TODO : provide support for counting the number of cycles passed
// from the start of the program
std::array<Instruction, 256> get_instructions(emulator::Cpu& cpu)
{
    ENABLE_PROFILER(cpu);
    // Byte key indicates which function we need to call
    // to handle the specific instruction
// using Instruction = std::function<std::optional<std::size_t>(emulator::Cpu&, std::span<const std::uint8_t>)>;
    std::array<Instruction, 256> supported_instructions{};
    for (std::size_t i = 0; i < supported_instructions.size(); ++i)
    {
        supported_instructions[i] = [=](emulator::Cpu&, std::span<const std::uint8_t>) -> std::optional<std::size_t>{
            throw emulator::OpcodeNotSupported(fmt::format("{0:#x}", i));
        };
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
    supported_instructions[0x85] = st_zeropage(&emulator::Registers::a);
    supported_instructions[0x8d] = st_absolute(&emulator::Registers::a);
    supported_instructions[0x91] = st_indirect(&emulator::Registers::a, &emulator::Registers::y);
    supported_instructions[0x86] = st_zeropage(&emulator::Registers::x);
    supported_instructions[0x8e] = st_absolute(&emulator::Registers::x);
    supported_instructions[0x84] = st_zeropage(&emulator::Registers::y);
    supported_instructions[0x8c] = st_absolute(&emulator::Registers::y);
    supported_instructions[0xa9] = ld_immediate(&emulator::Registers::a);
    supported_instructions[0xa5] = ld_zeropage(&emulator::Registers::a);
    supported_instructions[0xb5] = ld_zeropage_plus_reg(&emulator::Registers::a, &emulator::Registers::x);
    supported_instructions[0xbd] = ld_absolute_plus_reg(&emulator::Registers::a, &emulator::Registers::x);
    supported_instructions[0xb9] = ld_absolute_plus_reg(&emulator::Registers::a, &emulator::Registers::y);
    supported_instructions[0xa1] = ld_index_indirect(&emulator::Registers::a, &emulator::Registers::x);
    supported_instructions[0xb1] = ld_indirect_index(&emulator::Registers::a, &emulator::Registers::y);
    supported_instructions[0xad] = ld_absolute(&emulator::Registers::a);
    supported_instructions[0xa2] = ld_immediate(&emulator::Registers::x);
    supported_instructions[0xa6] = ld_zeropage(&emulator::Registers::x);
    supported_instructions[0xb6] = ld_zeropage_plus_reg(&emulator::Registers::x, &emulator::Registers::y);
    supported_instructions[0xae] = ld_absolute(&emulator::Registers::x);
    supported_instructions[0xbe] = ld_absolute_plus_reg(&emulator::Registers::x, &emulator::Registers::y);
    supported_instructions[0xa0] = ld_immediate(&emulator::Registers::y);
    supported_instructions[0xa4] = ld_zeropage(&emulator::Registers::y);
    supported_instructions[0xb4] = ld_zeropage_plus_reg(&emulator::Registers::y, &emulator::Registers::x);
    supported_instructions[0xbc] = ld_absolute_plus_reg(&emulator::Registers::y, &emulator::Registers::x);
    supported_instructions[0xac] = ld_absolute(&emulator::Registers::y);
    supported_instructions[0xc0] = cmp_immediate_reg(&emulator::Registers::y);
    supported_instructions[0xc5] = cmp_zeropage_reg(&emulator::Registers::a);
    supported_instructions[0xe0] = cmp_immediate_reg(&emulator::Registers::x);
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

    return supported_instructions;
}

std::optional<std::size_t> execute_next(emulator::Cpu& cpu, std::span<const std::uint8_t> program,
    std::array<Instruction, 256> instructions)
{
    ENABLE_PROFILER(cpu);
    // Read 1 byte for the operator
    if (cpu.reg.pc >= program.size())
    {
        return std::nullopt;
    }

    // Find the correct function to execute here
    auto const command = program[cpu.reg.pc];
    

    auto const instruction   = instructions[command];
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

    bool execute(Cpu& cpu, std::span<const std::uint8_t> program)
    {
        ENABLE_PROFILER(cpu);
        auto const instructions = get_instructions(cpu);
        while (cpu.reg.pc < program.size())
        {
            auto maybe_increment = execute_next(cpu, program, instructions);
            if (!maybe_increment)
            {
                return false;
            }

            cpu.reg.pc += *maybe_increment;
        }

        return true;
    }
} // namespace emulator
