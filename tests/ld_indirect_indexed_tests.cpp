/*
Tests for the load into register, indirect indexed
instructions. These instructions are:

- 0xb1: load into accumulator from (zp) + reg.x

All the instructions above will consume 2 bytes:
the opcode and the zp address.

All these instructions should take five cycles,
or six cycles if (zp) + reg.x crosses the page boundary.
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>

// NOLINTNEXTLINE
TEST(LDTests, LDAIndirectIndexYNonZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    cpu.mem[0x58]   = 0xff;
    cpu.mem[0x59]   = 0x01;
    cpu.mem[0x0200] = 0x5a;
    cpu.reg.y       = 0x01;

    constexpr std::array<std::uint8_t, 2> program{
        0xb1,
        0x58,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x5a);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x01);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAIndirectIndexYWithZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    cpu.mem[0x58]   = 0xff;
    cpu.mem[0x59]   = 0x01;
    cpu.mem[0x0200] = 0x00;
    cpu.reg.y       = 0x01;
    cpu.reg.a       = 0xff;

    constexpr std::array<std::uint8_t, 2> program{
        0xb1,
        0x58,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x01);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAIndirectIndexYWithNegative)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    cpu.mem[0x58]   = 0xff;
    cpu.mem[0x59]   = 0x01;
    cpu.mem[0x0200] = 0xff;
    cpu.reg.y       = 0x01;

    constexpr std::array<std::uint8_t, 2> program{
        0xb1,
        0x58,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0xff);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x01);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
