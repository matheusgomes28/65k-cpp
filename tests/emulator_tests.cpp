import emulator;

#include "common.h"

#include <gtest/gtest.h>


// NOLINTNEXTLINE
TEST(EmulatorTests, EmulateInxNoFlag)
{
    std::array<std::vector<std::uint8_t>, 3> programs{{
        {0xa2, 0x01, 0xe8},
        {0xa2, 0x02, 0xe8},
        {0xa2, 0x03, 0xe8},
    }};

    for (auto const& program : programs)
    {
        emulator::Cpu cpu;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.x, program[1] + 1);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x03);
    }
}

// NOLINTNEXTLINE
TEST(EmulatorTests, EmulateInyNoFlag)
{
    std::array<std::vector<std::uint8_t>, 3> programs{{
        {0xa0, 0x01, 0xc8},
        {0xa0, 0x02, 0xc8},
        {0xa0, 0x03, 0xc8},
    }};

    for (auto const& program : programs)
    {
        emulator::Cpu cpu;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.y, program[1] + 1);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x03);
    }
}

// TODO : Test INX and INY when we have flags being set

// NOLINTNEXTLINE
TEST(EmulatorTests, EmulateDexNoFlag)
{
    std::array<std::vector<std::uint8_t>, 3> programs{{
        {0xa2, 0x01, 0xca},
        {0xa2, 0x02, 0xca},
        {0xa2, 0x03, 0xca},
    }};

    for (auto const& program : programs)
    {
        emulator::Cpu cpu;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.x, program[1] - 1);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x03);
    }
}

// NOLINTNEXTLINE
TEST(EmulatorTests, EmulateDeyNoFlag)
{
    std::array<std::vector<std::uint8_t>, 3> programs{{
        {0xa0, 0x01, 0x88},
        {0xa0, 0x02, 0x88},
        {0xa0, 0x03, 0x88},
    }};

    for (auto const& program : programs)
    {
        emulator::Cpu cpu;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.y, program[1] - 1);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x03);
    }
}

// TODO test dey when we have flags being set

// NOLINTNEXTLINE
TEST(EmulatorTests, EmulateCpxXGreaterThanValue)
{
    std::array<std::vector<std::uint8_t>, 3> programs{{
        {0xa2, 0x01, 0xe0, 0x00},
        {0xa2, 0x02, 0xe0, 0x01},
        {0xa2, 0x03, 0xe0, 0x02},
    }};

    for (auto const& program : programs)
    {
        emulator::Cpu cpu;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.x, program[1]);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x04);

        // Flags
        ASSERT_EQ(cpu.flags.n, 0);
        ASSERT_EQ(cpu.flags.z, 0);
        ASSERT_EQ(cpu.flags.c, 1);
    }
}


// NOLINTNEXTLINE
TEST(EmulatorTests, EmulateCpxSameValues)
{
    std::array<std::vector<std::uint8_t>, 3> programs{{
        {0xa2, 0x01, 0xe0, 0x01},
        {0xa2, 0x02, 0xe0, 0x02},
        {0xa2, 0x03, 0xe0, 0x03},
    }};

    for (auto const& program : programs)
    {
        emulator::Cpu cpu;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.x, program[1]);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x04);

        // Flags
        ASSERT_EQ(cpu.flags.n, 0);
        ASSERT_EQ(cpu.flags.z, 1);
        ASSERT_EQ(cpu.flags.c, 1);
    }
}

// NOLINTNEXTLINE
TEST(EmulatorTests, EmulateCpxSetCarry)
{
    std::array<std::vector<std::uint8_t>, 3> programs{{
        {0xa2, 0x01, 0xe0, 0x02},
        {0xa2, 0x02, 0xe0, 0x03},
        {0xa2, 0x03, 0xe0, 0x04},
    }};

    for (auto const& program : programs)
    {
        emulator::Cpu cpu;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.x, program[1]);
        ASSERT_EQ(cpu.reg.sp, 0xFF);
        ASSERT_EQ(cpu.reg.pc, 0x04);

        // Flags
        ASSERT_EQ(cpu.flags.n, 1);
        ASSERT_EQ(cpu.flags.z, 0);
        ASSERT_EQ(cpu.flags.c, 0);
    }
}
