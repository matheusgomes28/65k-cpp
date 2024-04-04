import emulator;

#include <bitset>
#include <gtest/gtest.h>

constexpr emulator::Flags make_flags(std::bitset<8> flags)
{
    // CZID B1VN
    return emulator::Flags{
        .n = flags[7],
        .v = flags[6],
        .b = flags[4],
        .d = flags[3],
        .i = flags[2],
        .z = flags[1],
        .c = flags[0],
    };
}

// NOLINTNEXTLINE
TEST(EmulatorTests, EmulatesTXANoFlags)
{
    std::array<std::vector<std::uint8_t>, 5> programs{{
        {0xa2, 0x01, 0x8a},
        {0xa2, 0x02, 0x8a},
        {0xa2, 0x03, 0x8a},
        {0xa2, 0xa8, 0x8a},
        {0xa2, 0xFF, 0x8a},
    }};

    for (auto const& program : programs)
    {
        emulator::Cpu cpu;
        emulator::execute(cpu, {program.data(), program.size()});

        ASSERT_EQ(cpu.reg.a, program[1]);
        ASSERT_EQ(cpu.reg.x, program[1]);
        ASSERT_EQ(cpu.reg.sp, 0x00);
        ASSERT_EQ(cpu.reg.pc, 0x03);
    }
}

// NOLINTNEXTLINE
TEST(EmulatorTests, EmulatesTXANeroFlag)
{
    std::array<std::uint8_t, 3> program{
        {0xa2, 0x00, 0x8a},
    };

    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, program[1]);
    ASSERT_EQ(cpu.reg.x, program[1]);
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);
}

// NOLINTNEXTLINE
TEST(EmulatorTests, EmulatesTXANegativeFlag)
{
    std::array<std::uint8_t, 3> program{
        // Twos complement F1 is -ve
        {0xa2, 0xF1, 0x8a},
    };

    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, program[1]);
    ASSERT_EQ(cpu.reg.x, program[1]);
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x03);
}
// TODO: test flags ASSERT_EQ(cpu.flags, 0b1000'0000) when loading a negative val

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
        ASSERT_EQ(cpu.reg.sp, 0x00);
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
        ASSERT_EQ(cpu.reg.sp, 0x00);
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
        ASSERT_EQ(cpu.reg.sp, 0x00);
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
        ASSERT_EQ(cpu.reg.sp, 0x00);
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
        ASSERT_EQ(cpu.reg.sp, 0x00);
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
        ASSERT_EQ(cpu.reg.sp, 0x00);
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
        ASSERT_EQ(cpu.reg.sp, 0x00);
        ASSERT_EQ(cpu.reg.pc, 0x04);

        // Flags
        ASSERT_EQ(cpu.flags.n, 1);
        ASSERT_EQ(cpu.flags.z, 0);
        ASSERT_EQ(cpu.flags.c, 0);
    }
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
