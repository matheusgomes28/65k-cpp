import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <utility>

namespace
{
    void test_flag(bool emulator::Flags::*flag, std::size_t pos)
    {
        emulator::Cpu cpu;

        // clear the flags
        cpu.flags.n = false;
        cpu.flags.v = false;
        cpu.flags.b = false;
        cpu.flags.d = false;
        cpu.flags.i = false;
        cpu.flags.z = false;
        cpu.flags.c = false;


        cpu.reg.sp                        = 0xfe;
        std::uint8_t const expected_flags = static_cast<std::uint8_t>(1 << pos);
        cpu.mem[0x01ff]                   = static_cast<std::uint8_t>(expected_flags);

        constexpr std::array<std::uint8_t, 256> program{0x28};
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.sp, 0xff);

        // Assert no flags are set
        ASSERT_EQ(make_flags(expected_flags), cpu.flags);
    }
} // namespace

// NOLINTNEXTLINE
TEST(PLPTests, PopsFlagsIndividually)
{
    test_flag(&emulator::Flags::n, 7);
    test_flag(&emulator::Flags::v, 6);
    test_flag(&emulator::Flags::b, 4);
    test_flag(&emulator::Flags::i, 3);
    test_flag(&emulator::Flags::d, 2);
    test_flag(&emulator::Flags::z, 1);
    test_flag(&emulator::Flags::c, 0);
}

// NOLINTNEXTLINE
TEST(PLPTests, PopsFlagsOverflow)
{
    emulator::Cpu cpu;

    cpu.flags.n = false;
    cpu.flags.v = false;
    cpu.flags.b = false;
    cpu.flags.d = false;
    cpu.flags.i = false;
    cpu.flags.z = false;
    cpu.flags.c = false;

    cpu.reg.sp      = 0xff;
    cpu.mem[0x0100] = 0b1111'1111;

    constexpr std::array<std::uint8_t, 256> program{0x28};
    emulator::execute(cpu, {program.data(), program.size()});

    ASSERT_EQ(cpu.reg.sp, 0x00);

    // Assert no flags are set
    ASSERT_EQ(make_flags(0b1111'1111), cpu.flags);
}
