/*
The tests in this file will test the load zeropage
instructions in 6502.

The load zp instructions are
    - 0xa5: load into the accumulator from zp
    - 0xa6: load into the x register from zp
    - 0xa4: load into the y register from zp

Each instruction should take exactly 3 cycles,
and the N, Z flags may be set depending on the
value in the zp address.
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>

// NOLINTNEXTLINE
TEST(LDTests, LDAZeropageWithNonZero)
{
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xa5,
        0x22,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x5a);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAZeropageWithZero)
{
    emulator::Cpu cpu;

    // We load `0x00` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xa5,
        0x22,
    };
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
TEST(LDTests, LDAZeropageWithNegative)
{
    emulator::Cpu cpu;

    // We load `0xff` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0xff;

    constexpr std::array<std::uint8_t, 2> program{
        0xa5,
        0x22,
    };

    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0xff);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXZeropageWithNonZero)
{
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xa6,
        0x22,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x5a);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXZeropageWithZero)
{
    emulator::Cpu cpu;

    // We load `0x00` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xa6,
        0x22,
    };
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
TEST(LDTests, LDXZeropageWithNegative)
{
    emulator::Cpu cpu;

    // We load `0xff` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0xff;

    constexpr std::array<std::uint8_t, 2> program{
        0xa6,
        0x22,
    };

    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0xff);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYZeropageWithNonZero)
{
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xa4,
        0x22,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x5a);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYZeropageWithZero)
{
    emulator::Cpu cpu;

    // We load `0x00` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xa4,
        0x22,
    };
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
TEST(LDTests, LDYZeropageWithNegative)
{
    emulator::Cpu cpu;

    // We load `0xff` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0xff;

    constexpr std::array<std::uint8_t, 2> program{
        0xa4,
        0x22,
    };

    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0xff);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}
