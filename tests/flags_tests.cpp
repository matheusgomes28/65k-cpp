// This file contains the tests for opcodes
// setting and clearning flags

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

// NOLINTNEXTLINE
TEST(FlagsTests, CLC)
{
    emulator::Cpu cpu;
    constexpr std::array<std::uint8_t, 1> program{0x18};

    cpu.flags.n = true;
    cpu.flags.v = true;
    cpu.flags.b = true;
    cpu.flags.d = true;
    cpu.flags.i = true;
    cpu.flags.z = true;
    cpu.flags.c = true;

    emulator::execute(cpu, {program.data(), program.size()});
    ASSERT_FALSE(cpu.flags.c);
    ASSERT_EQ(0b1101'1110, cpu.sr());
}

// NOLINTNEXTLINE
TEST(FlagsTests, CLD)
{
    emulator::Cpu cpu;
    constexpr std::array<std::uint8_t, 1> program{0xd8};

    cpu.flags.n = true;
    cpu.flags.v = true;
    cpu.flags.b = true;
    cpu.flags.d = true;
    cpu.flags.i = true;
    cpu.flags.z = true;
    cpu.flags.c = true;

    emulator::execute(cpu, {program.data(), program.size()});
    ASSERT_FALSE(cpu.flags.d);
    ASSERT_EQ(0b1101'0111, cpu.sr());
}

// NOLINTNEXTLINE
TEST(FlagsTests, CLI)
{
    emulator::Cpu cpu;
    constexpr std::array<std::uint8_t, 1> program{0x58};

    cpu.flags.n = true;
    cpu.flags.v = true;
    cpu.flags.b = true;
    cpu.flags.d = true;
    cpu.flags.i = true;
    cpu.flags.z = true;
    cpu.flags.c = true;

    emulator::execute(cpu, {program.data(), program.size()});
    ASSERT_FALSE(cpu.flags.i);
    ASSERT_EQ(0b1101'1011, cpu.sr());
}

// NOLINTNEXTLINE
TEST(FlagsTests, CLV)
{
    emulator::Cpu cpu;
    constexpr std::array<std::uint8_t, 1> program{0xb8};

    cpu.flags.n = true;
    cpu.flags.v = true;
    cpu.flags.b = true;
    cpu.flags.d = true;
    cpu.flags.i = true;
    cpu.flags.z = true;
    cpu.flags.c = true;

    emulator::execute(cpu, {program.data(), program.size()});
    ASSERT_FALSE(cpu.flags.v);
    ASSERT_EQ(0b1001'1111, cpu.sr());
}
