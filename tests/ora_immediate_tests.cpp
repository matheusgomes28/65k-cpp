/*
This tests will check that the behaviour of the
"ORA" immediate opcode 0x09

This instructions will OR the immediate value
given to the program to the accumulator, then
store the result in the accumulator.

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
TEST(ORAImmediateTests, NoFlagOperations)
{
    // test_case = {init acc, immediate value}
    std::array<std::pair<std::uint8_t, std::uint8_t>, 3> const test_cases{{
        {0b0101'0101, 0b0010'1010},
        {0b0101'0101, 0b0101'0101},
        {0b0111'1111, 0b0000'0000},
    }};

    for (auto const& [init_acc, im_value] : test_cases)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_acc;

        std::array<std::uint8_t, 3> program{
            0x09,
            im_value,
        };
        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, init_acc | im_value);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(ORAImmediateTests, NegativeFlagOperation)
{
    // test_case = {init acc, immediate value}
    std::array<std::pair<std::uint8_t, std::uint8_t>, 6> const test_cases{{
        {0b1000'0000, 0b0010'1010},
        {0b1000'0000, 0b0101'0101},
        {0b1000'0000, 0b0000'0000},
        {0b0010'1010, 0b1000'0000},
        {0b0101'0101, 0b1000'0000},
        {0b0000'0000, 0b1000'0000},
    }};

    for (auto const& [init_acc, im_value] : test_cases)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_acc;

        std::array<std::uint8_t, 2> program{
            0x09,
            im_value,
        };
        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, init_acc | im_value);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
    }
}

// NOLINTNEXTLINE
TEST(ORAImmediateTests, ZeroFlagOperation)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0x00;

    std::array<std::uint8_t, 2> program{
        0x09,
        0x00,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(ORAImmediateTests, MakeSureFlagsAreSound)
{
    // Weird testcase, we should never have both Z and N
    // flags set

    for (std::int16_t acc = 0; acc < 256; ++acc)
    {
        for (std::int16_t val = 0; val < 256; ++val)
        {
            emulator::Cpu cpu;
            cpu.reg.a = static_cast<std::uint8_t>(acc);

            std::array<std::uint8_t, 2> program{
                0x09,
                static_cast<std::uint8_t>(val),
            };
            emulator::execute(cpu, program);

            // Registry expect
            ASSERT_EQ(cpu.reg.a, val | acc);
            ASSERT_EQ(cpu.reg.x, 0x00);
            ASSERT_EQ(cpu.reg.y, 0x00);
            ASSERT_EQ(cpu.reg.sp, 0xFF);
            ASSERT_EQ(cpu.reg.pc, 0x02);

            // Flags expect
            ASSERT_FALSE(cpu.flags.v);
            ASSERT_FALSE(cpu.flags.b);
            ASSERT_FALSE(cpu.flags.d);
            ASSERT_FALSE(cpu.flags.i);
            ASSERT_FALSE(cpu.flags.c);

            if (cpu.flags.z)
            {
                ASSERT_FALSE(cpu.flags.n);
            }
            if (cpu.flags.n)
            {
                ASSERT_FALSE(cpu.flags.z);
            }
        }
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
