/*
Tests for the load into register, index indirect
instructions. These instructions are:

- 0xa1: load into accumulator from zp + reg.x

All the instructions above will consume 2 bytes:
the opcode and the zp address.

All these instructions should take six cycles
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>

// NOLINTNEXTLINE
TEST(LDTests, LDAIndexIndirectXNonZero)
{
    emulator::Cpu cpu;

    cpu.mem[0x13]   = 0x00;
    cpu.mem[0x14]   = 0x02;
    cpu.mem[0x0200] = 0x5a;
    cpu.reg.x       = 0x13;

    constexpr std::array<std::uint8_t, 2> program{
        0xa1,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x5a);
    ASSERT_EQ(cpu.reg.x, 0x13);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAIndexIndirectXWithZero)
{
    emulator::Cpu cpu;

    cpu.mem[0x13]   = 0x00;
    cpu.mem[0x14]   = 0x02;
    cpu.mem[0x0200] = 0x00;
    cpu.reg.a       = 0xff;
    cpu.reg.x       = 0x13;

    constexpr std::array<std::uint8_t, 2> program{
        0xa1,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x13);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAIndexIndirectXNegative)
{
    emulator::Cpu cpu;

    cpu.mem[0x13]   = 0x00;
    cpu.mem[0x14]   = 0x02;
    cpu.mem[0x0200] = 0xff;
    cpu.reg.x       = 0x13;

    constexpr std::array<std::uint8_t, 2> program{
        0xa1,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0xff);
    ASSERT_EQ(cpu.reg.x, 0x13);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// This test makes sure we are wrapping the resulting
// address around the zeropage addresses, as expected
// in 6502
// NOLINTNEXTLINE
TEST(LDTests, LDAIndexIndirectXNonZeroPosWrap)
{
    emulator::Cpu cpu;

    cpu.mem[0x0000] = 0x00;
    cpu.mem[0x0001] = 0x02;
    cpu.mem[0x0200] = 0x5a;
    cpu.reg.x       = 0x12;

    constexpr std::array<std::uint8_t, 2> program{
        0xa1,
        0xee,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x5a);
    ASSERT_EQ(cpu.reg.x, 0x12);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}
