import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>

TEST(CPYTests, ImmediateEqualValues)
{
    emulator::Cpu cpu;
    cpu.reg.y = 0x40;
    std::array<std::uint8_t, 2> constexpr program{0xc0, 0x40};

    ASSERT_EQ(emulator::execute(cpu, program), 2);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_TRUE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CPYTests, ImmediateAccumulatorGreater)
{
    emulator::Cpu cpu;
    cpu.reg.y = 0x50;
    std::array<std::uint8_t, 2> constexpr program{0xc0, 0x40};

    ASSERT_EQ(emulator::execute(cpu, program), 2);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_FALSE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CPYTests, ImmediateAccumulatorLess)
{
    emulator::Cpu cpu;
    cpu.reg.y = 0x30;
    std::array<std::uint8_t, 2> constexpr program{0xc0, 0x40};

    ASSERT_EQ(emulator::execute(cpu, program), 2);

    EXPECT_TRUE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_FALSE(cpu.flags.z);
    EXPECT_FALSE(cpu.flags.c);
}

TEST(CPYTests, ImmediateZeroAccumulator)
{
    emulator::Cpu cpu;
    cpu.reg.y = 0x00;
    std::array<std::uint8_t, 2> constexpr program{0xc0, 0x40};

    ASSERT_EQ(emulator::execute(cpu, program), 2);

    EXPECT_TRUE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_FALSE(cpu.flags.z);
    EXPECT_FALSE(cpu.flags.c);
}

TEST(CPYTests, ImmediateMaxAccumulator)
{
    emulator::Cpu cpu;
    cpu.reg.y = 0xff;
    std::array<std::uint8_t, 2> constexpr program{0xc0, 0xfe};

    ASSERT_EQ(emulator::execute(cpu, program), 2);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_FALSE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CPYTests, ZeropageEqualValues)
{
    emulator::Cpu cpu;
    cpu.reg.y     = 0x40;
    cpu.mem[0xff] = 0x40;
    std::array<std::uint8_t, 2> constexpr program{0xc4, 0xff};

    ASSERT_EQ(emulator::execute(cpu, program), 3);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_TRUE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CPYTests, AbsoluteEqualValues)
{
    emulator::Cpu cpu;
    cpu.reg.y       = 0x40;
    cpu.mem[0xfffe] = 0x40;
    std::array<std::uint8_t, 3> constexpr program{0xcc, 0xfe, 0xff};

    ASSERT_EQ(emulator::execute(cpu, program), 4);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_TRUE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}
