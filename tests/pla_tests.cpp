import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <utility>

// NOLINTNEXTLINE
TEST(PLATests, PopsNoFlagsNoOverflow)
{
    emulator::Cpu cpu;
    ASSERT_EQ(cpu.reg.a, 0x00);

    cpu.reg.sp = 0xfe;
    cpu.mem[0x01ff] = 0b0111'1111;

    constexpr std::array<std::uint8_t, 256> program{ 0x68 };
    emulator::execute(cpu, {program.data(), program.size()});

    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.a, 0b0111'1111);

    // Assert no flags are set
    ASSERT_EQ(cpu.flags.n, false);
    ASSERT_EQ(cpu.flags.v, false);
    ASSERT_EQ(cpu.flags.b, false);
    ASSERT_EQ(cpu.flags.d, false);
    ASSERT_EQ(cpu.flags.i, false);
    ASSERT_EQ(cpu.flags.z, false);
    ASSERT_EQ(cpu.flags.c, false);
}

// NOLINTNEXTLINE
TEST(PLATests, PopsNegativeFlag)
{
    emulator::Cpu cpu;
    ASSERT_EQ(cpu.reg.a, 0x00);

    cpu.reg.sp = 0xfe;
    cpu.mem[0x01ff] = 0b1111'1111;

    constexpr std::array<std::uint8_t, 256> program{ 0x68 };
    emulator::execute(cpu, {program.data(), program.size()});

    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.a, 0b1111'1111);

    // Assert no flags are set
    ASSERT_EQ(cpu.flags.n, true);
    ASSERT_EQ(cpu.flags.v, false);
    ASSERT_EQ(cpu.flags.b, false);
    ASSERT_EQ(cpu.flags.d, false);
    ASSERT_EQ(cpu.flags.i, false);
    ASSERT_EQ(cpu.flags.z, false);
    ASSERT_EQ(cpu.flags.c, false);
}

// NOLINTNEXTLINE
TEST(PLATests, PopsZeroFlag)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0xff;

    cpu.reg.sp = 0xfe;
    cpu.mem[0x01ff] = 0x00;

    constexpr std::array<std::uint8_t, 256> program{ 0x68 };
    emulator::execute(cpu, {program.data(), program.size()});

    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.a, 0x00);

    // Assert no flags are set
    ASSERT_EQ(cpu.flags.n, false);
    ASSERT_EQ(cpu.flags.v, false);
    ASSERT_EQ(cpu.flags.b, false);
    ASSERT_EQ(cpu.flags.d, false);
    ASSERT_EQ(cpu.flags.i, false);
    ASSERT_EQ(cpu.flags.z, true);
    ASSERT_EQ(cpu.flags.c, false);
}

// NOLINTNEXTLINE
TEST(PLATests, PopsOverflow)
{
    emulator::Cpu cpu;
    ASSERT_EQ(cpu.reg.a, 0x00);

    cpu.reg.sp = 0xff;
    cpu.mem[0x0100] = 0b0111'1111;

    constexpr std::array<std::uint8_t, 256> program{ 0x68 };
    emulator::execute(cpu, {program.data(), program.size()});

    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.a, 0b0111'1111);

    // Assert no flags are set
    ASSERT_EQ(cpu.flags.n, false);
    ASSERT_EQ(cpu.flags.v, false);
    ASSERT_EQ(cpu.flags.b, false);
    ASSERT_EQ(cpu.flags.d, false);
    ASSERT_EQ(cpu.flags.i, false);
    ASSERT_EQ(cpu.flags.z, false);
    ASSERT_EQ(cpu.flags.c, false);
}