/*
This tests will check that the behaviour of the
"AND" zeropage opcode 0x25

This instructions will AND the value at the
zeropage location given to the program to
the accumulator, then store the result in
the accumulator.

This sets the N and Z flags depending on the value
left in the accumulator.

This instruction takes exactly two cycles.
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <utility>


// NOLINTNEXTLINE
TEST(ANDZeropageTests, NoFlagOperations)
{
    // test_case = {init acc, value, zp address}
    std::array<std::tuple<std::uint8_t, std::uint8_t, std::int16_t>, 3> const test_cases{{
        {0b0101'0101, 0b0010'1011, 0x00},
        {0b0101'0101, 0b0101'0101, 0x88},
        {0b0111'1111, 0b0111'1111, 0xff},
    }};

    for (auto const& [init_acc, value, address] : test_cases)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_acc;
        cpu.mem[static_cast<std::uint8_t>(address)] = value;

        std::array<std::uint8_t, 3> program{
            0x25,
            static_cast<std::uint8_t>(address),
        };
        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, init_acc & value);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(ANDZeropageTests, NegativeFlagOperation)
{
    // test_case = {init acc, value, zp address}
    std::array<std::tuple<std::uint8_t, std::uint8_t, std::int16_t>, 6> const test_cases{{
        {0b1000'0000, 0b1010'1010, 0x00},
        {0b1000'0000, 0b1101'0101, 0x88},
        {0b1000'0000, 0b1000'0000, 0xFF},
        {0b1010'1010, 0b1010'1010, 0x00},
        {0b1101'0101, 0b1101'0101, 0x88},
        {0b1000'0000, 0b1000'0000, 0xff},
    }};

    for (auto const& [acc, value, address] : test_cases)
    {
        emulator::Cpu cpu;
        cpu.reg.a = acc;
        cpu.mem[static_cast<std::uint8_t>(address)] = value;

        std::array<std::uint8_t, 2> program{
            0x25,
            static_cast<std::uint8_t>(address),
        };
        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, acc & value);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
    }
}

// NOLINTNEXTLINE
TEST(ANDZeropageTests, ZeroFlagOperation)
{
    // test_case = {init acc, value, zp address}
    std::array<std::tuple<std::uint8_t, std::uint8_t, std::int16_t>, 2> const test_cases{{
        {0b1000'0000, 0b0110'1010, 0x00},
        {0b1010'0101, 0b0101'1010, 0xff},
    }};

    for (auto const& [init_acc, value, zp_addr] : test_cases)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_acc;
        cpu.mem[0x88] = value;

        std::array<std::uint8_t, 2> program{
            0x25,
            0x88,
        };
        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, init_acc & value);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
    }
}
