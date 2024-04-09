
import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <utility>

// NOLINTNEXTLINE
TEST(IncrementTests, INXNoFlags)
{
    // TestData = (program, init_x_register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 4> programs{{
        {{0xe8}, 0x01},
        {{0xe8}, 0x02},
        {{0xe8}, 0x03},
        {{0xe8}, 0x7E},
    }};

    for (auto const& [program, init_x] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.x = init_x;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, init_x + 1);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0x00);
        ASSERT_EQ(cpu.reg.pc, 0x01);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(IncrementTests, INXZeroFlag)
{
    constexpr std::array<std::uint8_t, 1> program = {0xe8};

    emulator::Cpu cpu;
    cpu.reg.x = 0xff;
    emulator::execute(cpu, {program.data(), program.size()});

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x01);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(IncrementTests, INXNegativeFlag)
{
    // TestData = (program, init_x_register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 8> programs{{
        {{0xe8}, 0xfe},
        {{0xe8}, 0xef},
        {{0xe8}, 0xdf},
        {{0xe8}, 0xcf},
        {{0xe8}, 0xbf},
        {{0xe8}, 0xaf},
        {{0xe8}, 0x9f},
        {{0xe8}, 0x7f},
    }};

    for (auto const& [program, init_x] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.x = init_x;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, init_x + 1);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0x00);
        ASSERT_EQ(cpu.reg.pc, 0x01);

        ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
    }
}

// TODO : Create the tests for X, Y, and Memory increment functions

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
