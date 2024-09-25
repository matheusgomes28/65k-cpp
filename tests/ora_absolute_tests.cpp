/*
This tests will check that the behaviour of the
"ORA" absolute opcode 0x0d

This instruction occupies three bytes: one for
the opcode, and two fro the address (high bits,
then low bits).

This instruction takes exactly four cycles.
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <utility>

// NOLINTNEXTLINE
TEST(ORAAbsoluteTests, GeneralTests)
{
    // test_case = {init acc, value, address}
    std::array<std::tuple<std::uint8_t, std::uint8_t, std::uint16_t>, 4> const test_cases{{
        {0b0101'0101, 0b0010'1010, 0x0000},
        {0b0101'0101, 0b0101'0101, 0x00ff},
        {0b0111'1111, 0b0000'0000, 0xff00},
        {0b0101'0101, 0b0101'0101, 0xffff},
    }};

    for (auto const& [init_acc, value, address] : test_cases)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_acc;
        cpu.mem[address] = value;

        auto const hsb = static_cast<std::uint8_t>((address >> 8) & 0b1111'11111);
        auto const lsb = static_cast<std::uint8_t>(address & 0b1111'1111);
        std::array<std::uint8_t, 3> const program{0x0d, lsb, hsb};

        emulator::execute(cpu, program);

        // Registry expect
        ASSERT_EQ(cpu.reg.a, init_acc | value);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x03);

        // Flags expect
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
