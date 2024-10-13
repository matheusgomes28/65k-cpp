import emulator;

#include "common.h"

#include <gtest/gtest.h>

// NOLINTNEXTLINE
TEST(STYTests, Zeropage)
{
    emulator::Cpu cpu;
    cpu.reg.y = 0b1010'1010;

    constexpr std::array<std::uint8_t, 2> program{
        0x84,
        0xfe,
    };
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0xfe], 0b1010'1010);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0b1010'1010);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

TEST(STYTests, ZeropageIndexed)
{
    emulator::Cpu cpu;
    cpu.reg.x = 0x02;
    cpu.reg.y = 0b1010'1010;

    constexpr std::array<std::uint8_t, 2> program{
        0x94,
        0xfe,
    };
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0x00], 0b1010'1010);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x02);
    ASSERT_EQ(cpu.reg.y, 0b1010'1010);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

TEST(STYTests, Absolute)
{
    emulator::Cpu cpu;
    cpu.reg.y = 0b1010'1010;

    constexpr std::array<std::uint8_t, 3> program{0x8c, 0xfe, 0xff};
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0xfffe], 0b1010'1010);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0b1010'1010);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}
