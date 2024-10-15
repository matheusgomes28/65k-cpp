import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>

TEST(CMPTests, ImmediateEqualValues)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0x40;
    std::array<std::uint8_t, 2> constexpr program{0xc9, 0x40};

    ASSERT_EQ(emulator::execute(cpu, program), 2);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_TRUE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CMPTests, ImmediateAccumulatorGreater)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0x50;
    std::array<std::uint8_t, 2> constexpr program{0xc9, 0x40};

    ASSERT_EQ(emulator::execute(cpu, program), 2);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_FALSE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CMPTests, ImmediateAccumulatorLess)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0x30;
    std::array<std::uint8_t, 2> constexpr program{0xc9, 0x40};

    ASSERT_EQ(emulator::execute(cpu, program), 2);

    EXPECT_TRUE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_FALSE(cpu.flags.z);
    EXPECT_FALSE(cpu.flags.c);
}

TEST(CMPTests, ImmediateZeroAccumulator)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0x00;
    std::array<std::uint8_t, 2> constexpr program{0xc9, 0x40};

    ASSERT_EQ(emulator::execute(cpu, program), 2);

    EXPECT_TRUE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_FALSE(cpu.flags.z);
    EXPECT_FALSE(cpu.flags.c);
}

TEST(CMPTests, ImmediateMaxAccumulator)
{
    emulator::Cpu cpu;
    cpu.reg.a = 0xff;
    std::array<std::uint8_t, 2> constexpr program{0xc9, 0xfe};

    ASSERT_EQ(emulator::execute(cpu, program), 2);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_FALSE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CMPTests, ZeropageEqualValues)
{
    emulator::Cpu cpu;
    cpu.reg.a     = 0x40;
    cpu.mem[0xff] = 0x40;
    std::array<std::uint8_t, 2> constexpr program{0xc5, 0xff};

    ASSERT_EQ(emulator::execute(cpu, program), 3);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_TRUE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CMPTests, ZeropageIndexedEqualValues)
{
    emulator::Cpu cpu;
    cpu.reg.a     = 0x40;
    cpu.reg.x     = 0x02;
    cpu.mem[0x00] = 0x40;
    std::array<std::uint8_t, 2> constexpr program{0xd5, 0xfe};

    ASSERT_EQ(emulator::execute(cpu, program), 4);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_TRUE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CMPTests, AbsoluteEqualValues)
{
    emulator::Cpu cpu;
    cpu.reg.a       = 0x40;
    cpu.mem[0xfffe] = 0x40;
    std::array<std::uint8_t, 3> constexpr program{0xcd, 0xfe, 0xff};

    ASSERT_EQ(emulator::execute(cpu, program), 4);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_TRUE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CMPTests, AbsoluteIndexedXEqualValues)
{
    emulator::Cpu cpu;
    cpu.reg.a       = 0x40;
    cpu.reg.x       = 0x01;
    cpu.mem[0xffff] = 0x40;
    std::array<std::uint8_t, 3> constexpr program{0xdd, 0xfe, 0xff};

    ASSERT_EQ(emulator::execute(cpu, program), 4);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_TRUE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CMPTests, AbsoluteIndexedYEqualValues)
{
    emulator::Cpu cpu;
    cpu.reg.a       = 0x40;
    cpu.reg.y       = 0x01;
    cpu.mem[0xffff] = 0x40;
    std::array<std::uint8_t, 3> constexpr program{0xd9, 0xfe, 0xff};

    ASSERT_EQ(emulator::execute(cpu, program), 4);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_TRUE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CMPTests, IndexedIndirectEqualValues)
{
    emulator::Cpu cpu;
    cpu.reg.a       = 0x40;
    cpu.reg.x       = 0x01;
    cpu.mem[0xff]   = 0xfe;
    cpu.mem[0x00]   = 0xff;
    cpu.mem[0xfffe] = 0x40;
    std::array<std::uint8_t, 2> constexpr program{0xc1, 0xfe};

    ASSERT_EQ(emulator::execute(cpu, program), 6);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_TRUE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}

TEST(CMPTests, IndirectIndexedEqualValues)
{
    emulator::Cpu cpu;
    cpu.reg.a       = 0x40;
    cpu.reg.y       = 0x01;
    cpu.mem[0xff]   = 0xfe;
    cpu.mem[0x00]   = 0xff;
    cpu.mem[0xffff] = 0x40;
    std::array<std::uint8_t, 2> constexpr program{0xd1, 0xff};

    ASSERT_EQ(emulator::execute(cpu, program), 5);

    EXPECT_FALSE(cpu.flags.n);
    EXPECT_FALSE(cpu.flags.v);
    EXPECT_FALSE(cpu.flags.b);
    EXPECT_FALSE(cpu.flags.d);
    EXPECT_FALSE(cpu.flags.i);
    EXPECT_TRUE(cpu.flags.z);
    EXPECT_TRUE(cpu.flags.c);
}
