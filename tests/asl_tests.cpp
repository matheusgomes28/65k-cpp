import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <utility>

// NOLINTNEXTLINE
TEST(ASLTests, AccNoFlagsSet)
{
    constexpr std::array<std::uint8_t, 1> program{0x0a};

    // test_data = vector of {init_a, expected_a}
    std::array<std::tuple<std::uint8_t, std::uint8_t>, 6> test_data{{
        {0b0010'0000, 0b0100'0000},
        {0b0001'0000, 0b0010'0000},
        {0b0000'1000, 0b0001'0000},
        {0b0000'0100, 0b0000'1000},
        {0b0000'0010, 0b0000'0100},
        {0b0000'0001, 0b0000'0010},
    }};

    for (auto const& [init_a, exp_a] : test_data)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_a;
        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.reg.a, exp_a);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xff);
        ASSERT_EQ(cpu.reg.pc, 0x01);
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(ASLTests, NegativeFlagSet)
{
    constexpr std::array<std::uint8_t, 1> program{0x0a};

    emulator::Cpu cpu;
    cpu.reg.a = 0b0100'0000;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0b1000'0000);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x01);
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(ASLTests, CarryFlagSet)
{
    constexpr std::array<std::uint8_t, 1> program{0x0a};

    emulator::Cpu cpu;
    cpu.reg.a = 0b1010'0000;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0b0100'0000);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x01);
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0001));
}

// NOLINTNEXTLINE
TEST(ASLTests, ZeroFlagSet)
{
    constexpr std::array<std::uint8_t, 1> program{0x0a};

    // test_data = vector of {init_a, expected_a}
    std::array<std::uint8_t, 2> test_data{
        0b1000'0000,
        0b0000'0000,
    };

    for (auto const& init_a : test_data)
    {
        bool const exp_carry = init_a & 0b1000'0000;
        emulator::Cpu cpu;
        cpu.reg.a = init_a;
        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xff);
        ASSERT_EQ(cpu.reg.pc, 0x01);
        ASSERT_EQ(cpu.flags, make_flags(0b0000'0010 | exp_carry));
    }
}

// NOLINTNEXTLINE
TEST(ASLTests, ShiftsEightTimes)
{
    constexpr std::array<std::uint8_t, 8> program{0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a};

    emulator::Cpu cpu;
    cpu.reg.a = 0b0000'0001;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0b0000'0000);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x08);
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0011));
}
