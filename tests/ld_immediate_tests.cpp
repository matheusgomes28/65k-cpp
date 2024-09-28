/*
These tests check that the load immediate instructions
do what they are supposed to do. All these instructions
will consume two bytes from the program.


They should set the N and Z flags depending on the value
set in the chosen register.

All instructions tested here should take exactly two
cycles.
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>


// NOLINTNEXTLINE
TEST(LDTests, LdAImmediateWithNonZero)
{
    // Here we simply load a non-zero value
    // into the accumulator and check that
    // the CPU is update accordingly
    std::array<std::uint8_t, 2> program{
        0xa9,
        0x0a,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x0a);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LdAImmediateWithZero)
{
    // Here we simply load a zero value
    // into the accumulator and check that
    // the CPU is update accordingly, and
    // that the zero flag is set
    std::array<std::uint8_t, 2> program{
        0xa9,
        0x00,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LdAImmediateWithNegative)
{
    // Here we simply load a negative value
    // into the accumulator and check that
    // the CPU is update accordingly, and the
    // negative flag is set
    std::array<std::uint8_t, 2> program{
        0xa9,
        0xFF,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0xFF);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LdXImmediateWithNonZero)
{
    std::array<std::uint8_t, 2> program{
        0xa2,
        0x0a,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x0a);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LdXImmediateWithZero)
{
    std::array<std::uint8_t, 2> program{
        0xa2,
        0x00,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LdXImmediateWithNegative)
{
    std::array<std::uint8_t, 2> program{
        0xa2,
        0xFF,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0xFF);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// Y Tests
// NOLINTNEXTLINE
TEST(LDTests, LdYImmediateWithNonZero)
{
    std::array<std::uint8_t, 2> program{
        0xa0,
        0x0a,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x0a);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LdYImmediateWithZero)
{
    std::array<std::uint8_t, 2> program{
        0xa0,
        0x00,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LdYImmediateWithNegative)
{
    std::array<std::uint8_t, 2> program{
        0xa0,
        0xFF,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0xFF);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}
