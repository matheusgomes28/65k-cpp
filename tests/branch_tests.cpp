/*
These tests will check the behaviour for the branching
instructions.

The branching instructions are
    - 0x90: branch on carry flag clear (c==0)
    - 0xb0: branch on carry flag set (c==1)
    - 0xf0: branch on result zero (z==1)
    - 0xd0: branch on result not zero (z==0)
    - 0x30: branch on result minus (n==1)
    - 0x10: branch on result plus (n==0)
    - 0x50: branch on overflow clear (v==0)
    - 0x70: branch on overflow set (v==1)

Each instruction takes a varying number of cycles. Two
acycles if the branch isn't taken, three cycles if the
branch is taken on the same page, four cycles if the
branch happens on a different page.


TODO : We still don't have a way to cound how many cycles
       each instruction takes for different boundaries. For
       these branching instructions, we need to know if
       cycles on branching for different pages is actually
       working fine.
*/

import emulator;

#include "common.h"

#include <array>
#include <utility>

#include <gtest/gtest.h>

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnCarrySetWhenUnset)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0xb0, 0x00}, 0x02},
        {{0xb0, 0x01}, 0x03},
        {{0xb0, 0x0a}, 0x0c},
        {{0xb0, 0xff}, 0x01},
    }};

    for (auto const& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.c = false;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnCarrySetWhenSet)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0xb0, 0x00}, 0x02},
        {{0xb0, 0x01}, 0x03},
        {{0xb0, 0x0a}, 0x0c},
        {{0xb0, 0xff}, 0x01},
    }};

    for (auto& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.c = true;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, expected_pc);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0001));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnCarryClearWhenSet)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0x90, 0x00}, 0x02},
        {{0x90, 0x01}, 0x03},
        {{0x90, 0x0a}, 0x0c},
        {{0x90, 0xff}, 0x01},
    }};

    for (auto const& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.c = true;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0001));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnCarryClearWhenUnset)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0x90, 0x00}, 0x02},
        {{0x90, 0x01}, 0x03},
        {{0x90, 0x0a}, 0x0c},
        {{0x90, 0xff}, 0x01},
    }};

    for (auto& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.c = false;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, expected_pc);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnZeroSetWhenUnset)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0xf0, 0x00}, 0x02},
        {{0xf0, 0x01}, 0x03},
        {{0xf0, 0x0a}, 0x0c},
        {{0xf0, 0xff}, 0x01},
    }};

    for (auto const& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.z = false;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnZeroSetWhenSet)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0xf0, 0x00}, 0x02},
        {{0xf0, 0x01}, 0x03},
        {{0xf0, 0x0a}, 0x0c},
        {{0xf0, 0xff}, 0x01},
    }};

    for (auto& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.z = true;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, expected_pc);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnZeroClearWhenSet)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0xd0, 0x00}, 0x02},
        {{0xd0, 0x01}, 0x03},
        {{0xd0, 0x0a}, 0x0c},
        {{0xd0, 0xff}, 0x01},
    }};

    for (auto const& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.z = true;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnZeroClearWhenUnset)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0xd0, 0x00}, 0x02},
        {{0xd0, 0x01}, 0x03},
        {{0xd0, 0x0a}, 0x0c},
        {{0xd0, 0xff}, 0x01},
    }};

    for (auto& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.z = false;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, expected_pc);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnNegativeSetWhenUnset)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0x30, 0x00}, 0x02},
        {{0x30, 0x01}, 0x03},
        {{0x30, 0x0a}, 0x0c},
        {{0x30, 0xff}, 0x01},
    }};

    for (auto const& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.n = false;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnNegativeSetWhenSet)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0x30, 0x00}, 0x02},
        {{0x30, 0x01}, 0x03},
        {{0x30, 0x0a}, 0x0c},
        {{0x30, 0xff}, 0x01},
    }};

    for (auto& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.n = true;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, expected_pc);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnNegativeClearWhenSet)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0x10, 0x00}, 0x02},
        {{0x10, 0x01}, 0x03},
        {{0x10, 0x0a}, 0x0c},
        {{0x10, 0xff}, 0x01},
    }};

    for (auto const& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.n = true;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnNegativeClearWhenUnset)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0x10, 0x00}, 0x02},
        {{0x10, 0x01}, 0x03},
        {{0x10, 0x0a}, 0x0c},
        {{0x10, 0xff}, 0x01},
    }};

    for (auto& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.n = false;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, expected_pc);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnOverflowSetWhenUnset)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0x70, 0x00}, 0x02},
        {{0x70, 0x01}, 0x03},
        {{0x70, 0x0a}, 0x0c},
        {{0x70, 0xff}, 0x01},
    }};

    for (auto const& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.n = false;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnOverflowSetWhenSet)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0x70, 0x00}, 0x02},
        {{0x70, 0x01}, 0x03},
        {{0x70, 0x0a}, 0x0c},
        {{0x70, 0xff}, 0x01},
    }};

    for (auto& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.v = true;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, expected_pc);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0100'0000));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnOverflowClearWhenSet)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0x50, 0x00}, 0x02},
        {{0x50, 0x01}, 0x03},
        {{0x50, 0x0a}, 0x0c},
        {{0x50, 0xff}, 0x01},
    }};

    for (auto const& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.v = true;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0100'0000));
    }
}

// NOLINTNEXTLINE
TEST(BranchingTests, BranchOnOverflowClearWhenUnset)
{
    // TestData = (program, expected reg.pc)
    using TestData = std::pair<std::array<std::uint8_t, 2>, std::uint16_t>;
    constexpr std::array<TestData, 4> programs{{
        {{0x50, 0x00}, 0x02},
        {{0x50, 0x01}, 0x03},
        {{0x50, 0x0a}, 0x0c},
        {{0x50, 0xff}, 0x01},
    }};

    for (auto& [program, expected_pc] : programs)
    {
        emulator::Cpu cpu;
        cpu.flags.v = false;

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, expected_pc);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}
