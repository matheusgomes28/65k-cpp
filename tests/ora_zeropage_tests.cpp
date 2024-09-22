/*
This tests will check that the behaviour of the
"ORA" zeropage opcode 0x05

This instructions will OR the value at the
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
TEST(ORAZeropageTests, NoFlagOperations)
{
    // test_case = {init acc, value, zp address}
    std::array<std::tuple<std::uint8_t, std::uint8_t, std::int16_t>, 3> const test_cases{{
        {0b0101'0101, 0b0010'1010, 0x00},
        {0b0101'0101, 0b0101'0101, 0x88},
        {0b0111'1111, 0b0000'0000, 0xff},
    }};

    for (auto const& [init_acc, value, address] : test_cases)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_acc;
        cpu.mem[static_cast<std::uint8_t>(address)] = value;

        std::array<std::uint8_t, 2> program{
            0x05,
            static_cast<std::uint8_t>(address),
        };
        ASSERT_TRUE(emulator::execute(cpu, program));

        // Registry expect
        ASSERT_EQ(cpu.reg.a, init_acc | value);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0x00);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(ORAZeropageTests, NegativeFlagOperation)
{
    // test_case = {init acc, value, zp address}
    std::array<std::tuple<std::uint8_t, std::uint8_t, std::int16_t>, 6> const test_cases{{
        {0b1000'0000, 0b0010'1010, 0x00},
        {0b1000'0000, 0b0101'0101, 0x88},
        {0b1000'0000, 0b0000'0000, 0xFF},
        {0b0010'1010, 0b1000'0000, 0x00},
        {0b0101'0101, 0b1000'0000, 0x88},
        {0b0000'0000, 0b1000'0000, 0xff},
    }};

    for (auto const& [acc, value, address] : test_cases)
    {
        emulator::Cpu cpu;
        cpu.reg.a = acc;
        cpu.mem[static_cast<std::uint8_t>(address)] = value;

        std::array<std::uint8_t, 2> program{
            0x05,
            static_cast<std::uint8_t>(address),
        };
        ASSERT_TRUE(emulator::execute(cpu, program));

        // Registry expect
        ASSERT_EQ(cpu.reg.a, acc | value);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0x00);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
    }
}

// NOLINTNEXTLINE
TEST(ORAZeropageTests, ZeroFlagOperation)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0x00;
    cpu.mem[0x88] = 0x00;

    std::array<std::uint8_t, 2> program{
        0x05,
        0x88,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

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
TEST(ORAZeropageTests, MakeSureFlagsAreSound)
{
    // Weird testcase, we should never have both Z and N
    // flags set

    for (std::int16_t acc = 0; acc < 256; ++acc)
    {
        for (std::int16_t val = 0; val < 256; ++val)
        {
            emulator::Cpu cpu;
            cpu.reg.a = static_cast<std::uint8_t>(acc);
            cpu.mem[0x88] = static_cast<std::uint8_t>(val);

            std::array<std::uint8_t, 2> program{
                0x05,
                0x88,
            };
            ASSERT_TRUE(emulator::execute(cpu, program));

            // Registry expect
            ASSERT_EQ(cpu.reg.a, val | acc);
            ASSERT_EQ(cpu.reg.x, 0x00);
            ASSERT_EQ(cpu.reg.y, 0x00);
            ASSERT_EQ(cpu.reg.sp, 0x00);
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
