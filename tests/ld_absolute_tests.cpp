/*
These tests will check the behaviour for the load into
register absolute address memory instructions. These
instructions are:

- 0xad: load into the accumulator from 16-bit address
- 0xae: load into the x register from 16-bit address
- 0xac: load into the y register from 16-bit address

All the instructions above will consume 3 bytes: the
command and two 8-bit values for the adddress.

They should set the N and Z flags depending on the value
set in the chosen register.

All instructions tested here should take exactly four
cycles.
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>


// NOLINTNEXTLINE
TEST(LDTests, LDAAbsoluteNonZero)
{
    emulator::Cpu cpu;

    cpu.mem[0x0200] = 0x5a;

    constexpr std::array<std::uint8_t, 3> program{
        0xad,
        0x00,
        0x02,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x5a);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXAbsoluteNonZero)
{
    emulator::Cpu cpu;

    cpu.mem[0x0200] = 0x5a;

    constexpr std::array<std::uint8_t, 3> program{
        0xae,
        0x00,
        0x02,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x5a);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYAbsoluteNonZero)
{
    emulator::Cpu cpu;

    cpu.mem[0x0200] = 0x5a;

    constexpr std::array<std::uint8_t, 3> program{
        0xac,
        0x00,
        0x02,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x5a);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAAbsoluteWithZero)
{
    emulator::Cpu cpu;

    cpu.mem[0x0200] = 0x00;

    constexpr std::array<std::uint8_t, 3> program{
        0xac,
        0x00,
        0x02,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXAbsoluteWithZero)
{
    emulator::Cpu cpu;

    cpu.mem[0x0200] = 0x00;

    constexpr std::array<std::uint8_t, 3> program{
        0xad,
        0x00,
        0x02,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYAbsoluteWithZero)
{
    emulator::Cpu cpu;

    cpu.mem[0x0200] = 0x00;

    constexpr std::array<std::uint8_t, 3> program{
        0xad,
        0x00,
        0x02,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAAbsoluteWithNegative)
{
    emulator::Cpu cpu;

    cpu.mem[0x0200] = 0xFF;

    constexpr std::array<std::uint8_t, 3> program{
        0xad,
        0x00,
        0x02,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0xFF);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXAbsoluteWithNegative)
{
    emulator::Cpu cpu;

    cpu.mem[0x0200] = 0xFF;

    constexpr std::array<std::uint8_t, 3> program{
        0xae,
        0x00,
        0x02,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0xFF);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYAbsoluteWithNegative)
{
    emulator::Cpu cpu;

    cpu.mem[0x0200] = 0xFF;

    constexpr std::array<std::uint8_t, 3> program{
        0xac,
        0x00,
        0x02,
    };
    ASSERT_TRUE(emulator::execute(cpu, program));

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
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
