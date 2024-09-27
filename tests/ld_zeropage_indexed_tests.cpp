/*
Tests for the load into register, zeropage indexed
instructions. These instructions are:

- 0xb5: load into accumulator from zp + reg.x
- 0xb6: load into the x register from zp + reg.y
- 0xb4: load into the y register from zp + reg.x

All these instructions should take exactly 2 cycles
and set the flags N, Z according to the value loaded.
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>

// NOLINTNEXTLINE
TEST(LDTests, LDAZeropagePlusXWithNonZero)
{
    emulator::Cpu cpu;

    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xb5,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x5a);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAZeropagePlusXWithZero)
{
    emulator::Cpu cpu;

    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xb5,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAZeropagePlusXWithNegative)
{
    emulator::Cpu cpu;

    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0xFF;

    constexpr std::array<std::uint8_t, 2> program{
        0xb5,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0xFF);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXZeropagePlusYWithNonZero)
{
    emulator::Cpu cpu;

    cpu.reg.y     = 0x10;
    cpu.mem[0x30] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xb6,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x5a);
    ASSERT_EQ(cpu.reg.y, 0x10);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXZeropagePlusYWithZero)
{
    emulator::Cpu cpu;

    cpu.reg.y     = 0x10;
    cpu.mem[0x30] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xb6,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x10);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXZeropagePlusYWithNegative)
{
    emulator::Cpu cpu;

    cpu.reg.y     = 0x10;
    cpu.mem[0x30] = 0xFF;

    constexpr std::array<std::uint8_t, 2> program{
        0xb6,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0xFF);
    ASSERT_EQ(cpu.reg.y, 0x10);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYZeropagePlusXWithNonZero)
{
    emulator::Cpu cpu;

    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xb4,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0x5a);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYZeropagePlusXWithZero)
{
    emulator::Cpu cpu;

    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xb4,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYZeropagePlusXWithNegative)
{
    emulator::Cpu cpu;

    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0xFF;

    constexpr std::array<std::uint8_t, 2> program{
        0xb4,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0xFF);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}
