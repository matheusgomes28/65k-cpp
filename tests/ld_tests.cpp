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
TEST(LDTests, EmulatesLDAImmediateWithNonZero)
{
    std::array<std::uint8_t, 2> program{
        0xa9,
        0x0a,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x0a);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, EmulatesLDAImmediateWithZero)
{
    std::array<std::uint8_t, 2> program{
        0xa9,
        0x00,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, EmulatesLDAImmediateWithNegative)
{
    std::array<std::uint8_t, 2> program{
        0xa9,
        0xFF,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0xFF);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// X Tests
// NOLINTNEXTLINE
TEST(LDTests, EmulatesLDXImmediateWithNonZero)
{
    std::array<std::uint8_t, 2> program{
        0xa2,
        0x0a,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x0a);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, EmulatesLDXImmediateWithZero)
{
    std::array<std::uint8_t, 2> program{
        0xa2,
        0x00,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, EmulatesLDXImmediateWithNegative)
{
    std::array<std::uint8_t, 2> program{
        0xa2,
        0xFF,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0xFF);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// Y Tests
// NOLINTNEXTLINE
TEST(LDTests, EmulatesLDYImmediateWithNonZero)
{
    std::array<std::uint8_t, 2> program{
        0xa0,
        0x0a,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x0a);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, EmulatesLDYImmediateWithZero)
{
    std::array<std::uint8_t, 2> program{
        0xa0,
        0x00,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, EmulatesLDYImmediateWithNegative)
{
    std::array<std::uint8_t, 2> program{
        0xa0,
        0xFF,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0xFF);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}