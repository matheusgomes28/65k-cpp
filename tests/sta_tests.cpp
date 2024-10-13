import emulator;

#include "common.h"

#include <gtest/gtest.h>

// NOLINTNEXTLINE
TEST(STATests, Zeropage)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0b1010'1010;

    constexpr std::array<std::uint8_t, 2> program{
        0x85,
        0xfe,
    };
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0xfe], 0b1010'1010);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0b1010'1010);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

TEST(STATests, ZeropageIndexed)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0b1010'1010;
    cpu.reg.x = 0x02;

    constexpr std::array<std::uint8_t, 2> program{
        0x95,
        0xfe,
    };
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0x00], 0b1010'1010);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0b1010'1010);
    ASSERT_EQ(cpu.reg.x, 0x02);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

TEST(STATests, Absolute)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0b1010'1010;

    constexpr std::array<std::uint8_t, 3> program{0x8d, 0xfe, 0xff};
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0xfffe], 0b1010'1010);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0b1010'1010);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

TEST(STATests, AbsoluteIndexedX)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0b1010'1010;
    cpu.reg.x = 0x02;

    constexpr std::array<std::uint8_t, 3> program{0x9d, 0xfe, 0xff};
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0x00], 0b1010'1010);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0b1010'1010);
    ASSERT_EQ(cpu.reg.x, 0x02);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

TEST(STATests, IndexIndirect)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0b1010'1010;
    cpu.reg.x = 0x01;

    cpu.mem[0xff] = 0xfe;
    cpu.mem[0x00] = 0xff;

    constexpr std::array<std::uint8_t, 3> program{
        0x81,
        0xfe,
    };
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0xfffe], 0b1010'1010);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0b1010'1010);
    ASSERT_EQ(cpu.reg.x, 0x01);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

TEST(STATests, IndirectIndex)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0b1010'1010;
    cpu.reg.y = 0x01;

    cpu.mem[0xfe] = 0xfe;
    cpu.mem[0xff] = 0xff;

    constexpr std::array<std::uint8_t, 3> program{
        0x91,
        0xfe,
    };
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0xffff], 0b1010'1010);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0b1010'1010);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x01);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}
