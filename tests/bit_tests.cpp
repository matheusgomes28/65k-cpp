import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>

// NOLINTNEXTLINE
TEST(BITTests, ZeropageNegativeFlag)
{
    constexpr std::array<std::uint8_t, 2> program{0x24, 0xff};

    emulator::Cpu cpu;
    cpu.reg.a     = 0b1111'1111;
    cpu.mem[0xff] = 0b1000'0000;

    EXPECT_EQ(emulator::execute(cpu, program), 3);
    EXPECT_EQ(cpu.reg.a, 0b1111'1111);
    EXPECT_EQ(cpu.reg.x, 0x00);
    EXPECT_EQ(cpu.reg.y, 0x00);
    EXPECT_EQ(cpu.reg.sp, 0xff);
    EXPECT_EQ(cpu.reg.pc, 0x8002);
    EXPECT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(BITTests, ZeropageOverflowFlag)
{
    constexpr std::array<std::uint8_t, 2> program{0x24, 0xff};

    emulator::Cpu cpu;
    cpu.reg.a     = 0b1111'1111;
    cpu.mem[0xff] = 0b0100'0000;

    EXPECT_EQ(emulator::execute(cpu, program), 3);
    EXPECT_EQ(cpu.reg.a, 0b1111'1111);
    EXPECT_EQ(cpu.reg.x, 0x00);
    EXPECT_EQ(cpu.reg.y, 0x00);
    EXPECT_EQ(cpu.reg.sp, 0xff);
    EXPECT_EQ(cpu.reg.pc, 0x8002);
    EXPECT_EQ(cpu.flags, make_flags(0b0100'0000));
}

// NOLINTNEXTLINE
TEST(BITTests, ZeropageZeroFlag)
{
    constexpr std::array<std::uint8_t, 2> program{0x24, 0xff};

    emulator::Cpu cpu;
    cpu.mem[0xff] = 0b0011'1111;

    EXPECT_EQ(emulator::execute(cpu, program), 3);
    EXPECT_EQ(cpu.reg.a, 0x00);
    EXPECT_EQ(cpu.reg.x, 0x00);
    EXPECT_EQ(cpu.reg.y, 0x00);
    EXPECT_EQ(cpu.reg.sp, 0xff);
    EXPECT_EQ(cpu.reg.pc, 0x8002);
    EXPECT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(BITTests, AbsoluteNegativeFlag)
{
    constexpr std::array<std::uint8_t, 3> program{0x2c, 0xfe, 0xff};

    emulator::Cpu cpu;
    cpu.reg.a       = 0b1111'1111;
    cpu.mem[0xfffe] = 0b1000'0000;

    EXPECT_EQ(emulator::execute(cpu, program), 4);
    EXPECT_EQ(cpu.reg.a, 0b1111'1111);
    EXPECT_EQ(cpu.reg.x, 0x00);
    EXPECT_EQ(cpu.reg.y, 0x00);
    EXPECT_EQ(cpu.reg.sp, 0xff);
    EXPECT_EQ(cpu.reg.pc, 0x8003);
    EXPECT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(BITTests, AbsoluteOverflowFlag)
{
    constexpr std::array<std::uint8_t, 3> program{0x2c, 0xfe, 0xff};

    emulator::Cpu cpu;
    cpu.reg.a       = 0b1111'1111;
    cpu.mem[0xfffe] = 0b0100'0000;

    EXPECT_EQ(emulator::execute(cpu, program), 4);
    EXPECT_EQ(cpu.reg.a, 0b1111'1111);
    EXPECT_EQ(cpu.reg.x, 0x00);
    EXPECT_EQ(cpu.reg.y, 0x00);
    EXPECT_EQ(cpu.reg.sp, 0xff);
    EXPECT_EQ(cpu.reg.pc, 0x8003);
    EXPECT_EQ(cpu.flags, make_flags(0b0100'0000));
}

// NOLINTNEXTLINE
TEST(BITTests, AbsoluteZeroFlag)
{
    constexpr std::array<std::uint8_t, 3> program{0x2c, 0xfe, 0xff};

    emulator::Cpu cpu;
    cpu.mem[0xfffe] = 0b0011'1111;

    EXPECT_EQ(emulator::execute(cpu, program), 4);
    EXPECT_EQ(cpu.reg.a, 0x00);
    EXPECT_EQ(cpu.reg.x, 0x00);
    EXPECT_EQ(cpu.reg.y, 0x00);
    EXPECT_EQ(cpu.reg.sp, 0xff);
    EXPECT_EQ(cpu.reg.pc, 0x8003);
    EXPECT_EQ(cpu.flags, make_flags(0b0000'0010));
}
