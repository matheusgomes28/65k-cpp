import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <utility>

// NOLINTNEXTLINE
TEST(TXTests, TXANoFlags)
{
    // TestData = (program, init_x_register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 5> programs{{
        {{0x8a}, 0x1},
        {{0x8a}, 0x02},
        {{0x8a}, 0x03},
        {{0x8a}, 0x7F},
        {{0x8a}, 0x70},
    }};

    for (auto const& [program, init_x] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.x = init_x;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.a, init_x);
        ASSERT_EQ(cpu.reg.x, init_x);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x01);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(TXTests, TAXNoFlags)
{
    // TestData = (program, init_x_register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 5> programs{{
        {{0xaa}, 0x1},
        {{0xaa}, 0x02},
        {{0xaa}, 0x03},
        {{0xaa}, 0x7F},
        {{0xaa}, 0x70},
    }};

    for (auto const& [program, init_a] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_a;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.a, init_a);
        ASSERT_EQ(cpu.reg.x, init_a);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x01);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(TXTests, TAYNoFlags)
{
    // TestData = (program, init_a_register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 5> programs{{
        {{0xa8}, 0x1},
        {{0xa8}, 0x02},
        {{0xa8}, 0x03},
        {{0xa8}, 0x7F},
        {{0xa8}, 0x70},
    }};

    for (auto const& [program, init_a] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_a;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.a, init_a);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, init_a);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x01);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(TXTests, TSXNoFlags)
{
    // TestData = (program, init sp register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 5> programs{{
        {{0xba}, 0x1},
        {{0xba}, 0x02},
        {{0xba}, 0x03},
        {{0xba}, 0x7F},
        {{0xba}, 0x70},
    }};

    for (auto const& [program, init_sp] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.sp = init_sp;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, init_sp);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, init_sp);
        ASSERT_EQ(cpu.reg.pc, 0x01);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(TXTests, TYANoFlags)
{
    // TestData = (program, init y register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 5> programs{{
        {{0x98}, 0x1},
        {{0x98}, 0x02},
        {{0x98}, 0x03},
        {{0x98}, 0x7F},
        {{0x98}, 0x70},
    }};

    for (auto const& [program, init_y] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.y = init_y;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.a, init_y);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, init_y);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x01);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(TXTests, TXSNoFlags)
{
    // TestData = (program, init y register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 8> programs{{
        {{0x9a}, 0x00},
        {{0x9a}, 0x01},
        {{0x9a}, 0x02},
        {{0x9a}, 0x03},
        {{0x9a}, 0x7F},
        {{0x9a}, 0x70},
        {{0x9a}, 0x80},
        {{0x9a}, 0xFF},
    }};

    for (auto const& [program, init_x] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.x = init_x;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, init_x);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, init_x);
        ASSERT_EQ(cpu.reg.pc, 0x01);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
    }
}

// NOLINTNEXTLINE
TEST(TXTests, TXAZeroFlag)
{
    constexpr std::array<std::uint8_t, 1> program{
        {0x8a},
    };

    emulator::Cpu cpu;
    cpu.reg.x = 0x00;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x01);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(TXTests, TAXZeroFlag)
{
    constexpr std::array<std::uint8_t, 1> program{
        {0xaa},
    };

    emulator::Cpu cpu;
    cpu.reg.a = 0x00;
    cpu.reg.x = 0xFF;

    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x01);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(TXTests, TAYZeroFlag)
{
    constexpr std::array<std::uint8_t, 1> program{
        {0xa8},
    };

    emulator::Cpu cpu;
    cpu.reg.a = 0x00;
    cpu.reg.y = 0xFF;

    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x01);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(TXTests, TSXZeroFlag)
{
    constexpr std::array<std::uint8_t, 1> program{
        {0xba},
    };

    emulator::Cpu cpu;
    cpu.reg.sp = 0x00;
    cpu.reg.x  = 0xFF;

    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x01);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(TXTests, TYAZeroFlag)
{
    constexpr std::array<std::uint8_t, 1> program{
        {0x98},
    };

    emulator::Cpu cpu;
    cpu.reg.y = 0x00;
    cpu.reg.a = 0xFF;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x01);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(TXTests, TXANegativeFlag)
{
    // TestData = (program, init_x_register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 8> programs{{
        {{0x8a}, 0xFF},
        {{0x8a}, 0xE0},
        {{0x8a}, 0xD0},
        {{0x8a}, 0xC0},
        {{0x8a}, 0xB0},
        {{0x8a}, 0xA0},
        {{0x8a}, 0x90},
        {{0x8a}, 0x80},
    }};

    for (auto const& [program, init_x] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.x = init_x;
        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.reg.a, init_x);
        ASSERT_EQ(cpu.reg.x, init_x);
        ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x01);
    }
}

TEST(TXTests, TAXNegativeFlag)
{
    // TestData = (program, init_a_register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 8> programs{{
        {{0xaa}, 0xFF},
        {{0xaa}, 0xE0},
        {{0xaa}, 0xD0},
        {{0xaa}, 0xC0},
        {{0xaa}, 0xB0},
        {{0xaa}, 0xA0},
        {{0xaa}, 0x90},
        {{0xaa}, 0x80},
    }};

    for (auto const& [program, init_a] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_a;
        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.reg.a, init_a);
        ASSERT_EQ(cpu.reg.x, init_a);
        ASSERT_EQ(cpu.reg.y, 0);
        ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x01);
    }
}

TEST(TXTests, TAYNegativeFlag)
{
    // TestData = (program, init_a_register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 8> programs{{
        {{0xa8}, 0xFF},
        {{0xa8}, 0xE0},
        {{0xa8}, 0xD0},
        {{0xa8}, 0xC0},
        {{0xa8}, 0xB0},
        {{0xa8}, 0xA0},
        {{0xa8}, 0x90},
        {{0xa8}, 0x80},
    }};

    for (auto const& [program, init_a] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.a = init_a;
        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.reg.a, init_a);
        ASSERT_EQ(cpu.reg.x, 0);
        ASSERT_EQ(cpu.reg.y, init_a);
        ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x01);
    }
}

TEST(TXTests, TSXNegativeFlag)
{
    // TestData = (program, init sp register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 8> programs{{
        {{0xba}, 0xFF},
        {{0xba}, 0xE0},
        {{0xba}, 0xD0},
        {{0xba}, 0xC0},
        {{0xba}, 0xB0},
        {{0xba}, 0xA0},
        {{0xba}, 0x90},
        {{0xba}, 0x80},
    }};

    for (auto const& [program, init_sp] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.sp = init_sp;
        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, init_sp);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, init_sp);
        ASSERT_EQ(cpu.reg.pc, 0x01);

        ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
    }
}

TEST(TXTests, TYANegativeFlag)
{
    // TestData = (program, init y register)
    using TestData = std::pair<std::array<std::uint8_t, 1>, std::uint8_t>;

    constexpr std::array<TestData, 8> programs{{
        {{0x98}, 0xFF},
        {{0x98}, 0xE0},
        {{0x98}, 0xD0},
        {{0x98}, 0xC0},
        {{0x98}, 0xB0},
        {{0x98}, 0xA0},
        {{0x98}, 0x90},
        {{0x98}, 0x80},
    }};

    for (auto const& [program, init_y] : programs)
    {
        emulator::Cpu cpu;
        cpu.reg.y = init_y;
        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.reg.a, init_y);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, init_y);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x01);

        ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}