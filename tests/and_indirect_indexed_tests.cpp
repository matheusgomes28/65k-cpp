/*
This tests will check that the behaviour of the
"AND" indirect indexed, 0x31 opcode.

The instruction takes two bytes: one for the opcode
itself, and the immediate value of the zp-address
containing the 16-bit address to be indirected

This instrucition takes exactly 6 bytes.
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <utility>

// NOLINTNEXTLINE
TEST(ANDIndirectIndexedTests, ZeroAddressToLastAddress)
{
    // the zero page will point to 0x00, which stores the
    // abs address 0xfffe, then reg.y=0x01 is added to it
    // resulting in the target address 0xffff holding the
    // complement of the or operation

    emulator::Cpu cpu;
    cpu.reg.a       = 0b0111'1111;
    cpu.reg.y       = 0x01;
    cpu.mem[0xffff] = 0b0111'1111;
    cpu.mem[0x00]   = 0xfe;
    cpu.mem[0x01]   = 0xff;

    std::array<std::uint8_t, 2> const program{0x31, 0x00};

    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0b0111'1111);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x01);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(ANDIndirectIndexedTests, ZeropageWrapsAround)
{
    // here we check that the zeropage address
    // is wrapped around to get the address held
    // at 0xff and 0x00, holding the address 0xfffe
    // which is added to reg.y=0x01, resulting
    // in target 0xffff holding the complement of
    // the or operation

    emulator::Cpu cpu;
    cpu.reg.a       = 0b0111'1111;
    cpu.reg.y       = 0x01;
    cpu.mem[0xffff] = 0b0111'1111;
    cpu.mem[0xff]   = 0xfe;
    cpu.mem[0x00]   = 0xff;

    std::array<std::uint8_t, 2> const program{0x31, 0xff};

    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0b0111'1111);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x01);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
