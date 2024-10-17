import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>

// NOLINTNEXTLINE
TEST(NOPTests, VeryBoringTests)
{
    constexpr std::array<std::uint8_t, 1> program{0xea};

    emulator::Cpu cpu;
    ASSERT_EQ(emulator::execute(cpu, program), 2);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x01);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(NOPTests, AnotherVeryBoringTests)
{
    constexpr std::array<std::uint8_t, 3> program{0xea, 0xea, 0xea};

    emulator::Cpu cpu;
    ASSERT_EQ(emulator::execute(cpu, program), 6);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}
