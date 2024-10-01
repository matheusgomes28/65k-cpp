/// This file contains tests for the flags registers. More
/// specifically, we will test that the opcodes 0x38 (SEC),
/// 0xf8 (SED), and 0x78 (SEI) set the flags they are
/// supposed to, and that the "cpu.sr()" function returns the
/// correct state of the flags

import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <utility>

// NOLINTNEXTLINE
TEST(FlagsTests, SEC)
{
    emulator::Cpu cpu;
    constexpr std::array<std::uint8_t, 1> program{0x38};

    ASSERT_FALSE(cpu.flags.c);
    emulator::execute(cpu, {program.data(), program.size()});
    ASSERT_TRUE(cpu.flags.c);

    ASSERT_EQ(0b0000'0001, cpu.sr());
}

// NOLINTNEXTLINE
TEST(FlagsTests, SED)
{
    emulator::Cpu cpu;
    constexpr std::array<std::uint8_t, 1> program{0xf8};

    ASSERT_FALSE(cpu.flags.d);
    emulator::execute(cpu, {program.data(), program.size()});
    ASSERT_TRUE(cpu.flags.d);

    ASSERT_EQ(0b0000'1000, cpu.sr());
}

// NOLINTNEXTLINE
TEST(FlagsTests, SEI)
{
    emulator::Cpu cpu;
    constexpr std::array<std::uint8_t, 1> program{0x78};

    ASSERT_FALSE(cpu.flags.i);
    emulator::execute(cpu, {program.data(), program.size()});
    ASSERT_TRUE(cpu.flags.i);

    ASSERT_EQ(0b0000'0100, cpu.sr());
}

// NOLINTNEXTLINE
TEST(FlagsTests, SRGetsNegative)
{
    emulator::Cpu cpu;
    constexpr std::array<std::uint8_t, 2> program{0xa9, 0b1000'0000};

    ASSERT_FALSE(cpu.flags.n);
    emulator::execute(cpu, {program.data(), program.size()});
    ASSERT_TRUE(cpu.flags.n);

    ASSERT_EQ(0b1000'0000, cpu.sr());
}

// NOLINTNEXTLINE
TEST(FlagsTests, SRGetsZero)
{
    emulator::Cpu cpu;
    constexpr std::array<std::uint8_t, 2> program{0xa9, 0b0000'0000};

    ASSERT_FALSE(cpu.flags.z);
    emulator::execute(cpu, {program.data(), program.size()});
    ASSERT_TRUE(cpu.flags.z);

    ASSERT_EQ(0b0000'0010, cpu.sr());
}

// NOLINTNEXTLINE
TEST(FlagsTests, SRGetsOverflow)
{
    emulator::Cpu cpu;
    cpu.flags.v = true;
    ASSERT_EQ(cpu.sr(), 0b0100'0000);
}

// NOLINTNEXTLINE
TEST(FlagsTests, SRGetsBreak)
{
    emulator::Cpu cpu;
    cpu.flags.b = true;
    ASSERT_EQ(cpu.sr(), 0b0001'0000);
}
