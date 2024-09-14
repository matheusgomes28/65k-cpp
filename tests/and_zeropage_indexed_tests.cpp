/*
This tests will check that the behaviour of the
"AND" zeropage opcode 0x35

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
TEST(ANDZeropageIndexTests, WrapAroundTests)
{
    // test_case = {init acc, value, zp address, init x, exp mem}
    std::array<std::tuple<std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t>, 4> const test_cases{{
        {0b0101'0101, 0b0110'1010, 0x00, 0x0a, 0x0a},
        {0b0101'0101, 0b0101'0101, 0x88, 0x0a, 0x92},
        {0b0111'1111, 0b0000'0001, 0xff, 0x0a, 0x09},
        {0b0101'0101, 0b0101'0101, 0xff, 0x01, 0x00},
    }};

    for (auto const& [init_acc, value, address, init_x, exp_mem] : test_cases)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_acc;
        cpu.reg.x = init_x;
        cpu.mem[exp_mem] = value;

        std::array<std::uint8_t, 2> program{
            0x35,
            static_cast<std::uint8_t>(address),
        };
        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, init_acc & value);
        ASSERT_EQ(cpu.reg.x, init_x);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0x00);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
