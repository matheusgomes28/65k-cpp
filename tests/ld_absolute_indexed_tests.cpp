/*
Tests for the load into register, absolute indexed
instructions. These instructions are:

- 0xbd: load into accumulator from absolute + reg.x
- 0xb9: load into accumulator from absolute + reg.x
- 0xbe: load into the x register from absolute + reg.y
- 0xbc: load into the y register from absolute + reg.x

All the instructions above will consume 3 bytes: the
instruction byte, and two 8-bit values for the address.

All these instructions should take four cycles OR
five cycles if the page boundary is crossed.
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>
/***************************************************
*********** LD* Absolute  + Reg Tests **************
***************************************************/

// NOLINTNEXTLINE
TEST(LDTests, LDAAbsolutePlusXNonZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // Load `0x5a` into mem address `0x0200` and
    // see if the LDA absolute command will load
    // the value into register A
    cpu.mem[0x0100] = 0x5a;
    cpu.reg.x       = 0x13;

    constexpr std::array<std::uint8_t, 3> program{
        0xbd,
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x5a);
    ASSERT_EQ(cpu.reg.x, 0x13);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAAbsolutePlusYNonZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // Load `0x5a` into mem address `0x0200` and
    // see if the LDA absolute command will load
    // the value into register A
    cpu.mem[0x0100] = 0x5a;
    cpu.reg.y       = 0x13;

    constexpr std::array<std::uint8_t, 3> program{
        0xb9,
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x5a);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x13);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXAbsolutePlusYNonZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // Load `0x5a` into mem address `0x0200` and
    // see if the LDA absolute command will load
    // the value into register A
    cpu.mem[0x0100] = 0x5a;
    cpu.reg.y       = 0x13;

    constexpr std::array<std::uint8_t, 3> program{
        0xbe,
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x5a);
    ASSERT_EQ(cpu.reg.y, 0x13);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYAbsolutePlusXNonZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // Load `0x5a` into mem address `0x0200` and
    // see if the LDA absolute command will load
    // the value into register A
    cpu.mem[0x0100] = 0x5a;
    cpu.reg.x       = 0x13;

    constexpr std::array<std::uint8_t, 3> program{
        0xbc,
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x13);
    ASSERT_EQ(cpu.reg.y, 0x5a);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAAbsolutePlusXWithZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x00` into the `0x0200` mem address
    // and expect to have `0x00` in register A
    cpu.mem[0x0100] = 0x00;
    cpu.reg.x       = 0x13;
    cpu.reg.a       = 0xFF;
    // TODO : add above side effect test in other tests

    constexpr std::array<std::uint8_t, 3> program{
        0xbd,
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x13);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAAbsolutePlusYWithZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x00` into the `0x0200` mem address
    // and expect to have `0x00` in register A
    cpu.mem[0x0100] = 0x00;
    cpu.reg.y       = 0x13;
    cpu.reg.a       = 0xFF;
    // TODO : add above side effect test in other tests

    constexpr std::array<std::uint8_t, 3> program{
        0xb9,
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x13);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXAbsolutePlusYWithZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x00` into the `0x0200` mem address
    // and expect to have `0x00` in register A
    cpu.mem[0x0100] = 0x00;
    cpu.reg.y       = 0x13;
    cpu.reg.x       = 0xFF;
    // TODO : add above side effect test in other tests

    constexpr std::array<std::uint8_t, 3> program{
        0xbe, // change from x
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x13);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYAbsolutePlusXWithZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x00` into the `0x0200` mem address
    // and expect to have `0x00` in register A
    cpu.mem[0x0100] = 0x00;
    cpu.reg.x       = 0x13;
    cpu.reg.y       = 0xFF;
    // TODO : add above side effect test in other tests

    constexpr std::array<std::uint8_t, 3> program{
        0xbc,
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x13);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAAbsolutePlusXWithNegative)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    cpu.mem[0x0100] = 0xFF;
    cpu.reg.x       = 0x13;

    constexpr std::array<std::uint8_t, 3> program{
        0xbd,
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0xFF);
    ASSERT_EQ(cpu.reg.x, 0x13);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAAbsolutePlusYWithNegative)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    cpu.mem[0x0100] = 0xFF;
    cpu.reg.y       = 0x13;

    constexpr std::array<std::uint8_t, 3> program{
        0xb9,
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0xFF);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x13);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXAbsolutePlusYWithNegative)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    cpu.mem[0x0100] = 0xFF;
    cpu.reg.y       = 0x13;

    constexpr std::array<std::uint8_t, 3> program{
        0xbe,
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0xFF);
    ASSERT_EQ(cpu.reg.y, 0x13);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYAbsolutePlusXWithNegative)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    cpu.mem[0x0100] = 0xFF;
    cpu.reg.x       = 0x13;

    constexpr std::array<std::uint8_t, 3> program{
        0xbc,
        0xed,
        0x00,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x13);
    ASSERT_EQ(cpu.reg.y, 0xFF);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
