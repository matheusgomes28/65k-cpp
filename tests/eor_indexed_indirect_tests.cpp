/*
This tests will check that the behaviour of the
"EOR" indexed indirect, 0x41 opcode.

The instruction takes two bytes: one for the opcode
itself, and the immediate value of the zp-address
containing the 16-bit address to be indirected

This instrucition takes 5 bytes if the page boundary
of the target address is the same after adding reg.y,
otherwise it takes 6 cycles.
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <utility>

// NOLINTNEXTLINE
TEST(EORIndexedIndirectTests, ZeroAddressToLastAddress)
{
    // the zero page + x will render to the zero address,
    // which will store the two last accessible bytes

    emulator::Cpu cpu;
    cpu.reg.a       = 0b0101'0101;
    cpu.reg.x       = 0x00;
    cpu.mem[0xffff] = 0b0010'1010;
    cpu.mem[0x00]   = 0xff;
    cpu.mem[0x01]   = 0xff;

    std::array<std::uint8_t, 2> const program{0x41, 0x00};

    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0b0111'1111);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(EORIndexedIndirectTests, ZeropageWrapsAround)
{
    // the zero page + x address yields to the last
    // usable byte in the zeropage address. Meaning
    // that we wrap to the 0 byte as the high byte
    // on the indirect address

    emulator::Cpu cpu;
    cpu.reg.a       = 0b0101'0101;
    cpu.reg.x       = 0x01;
    cpu.mem[0xffff] = 0b0010'1010;
    cpu.mem[0xff]   = 0xff;
    cpu.mem[0x00]   = 0xff;

    std::array<std::uint8_t, 2> const program{0x41, 0xfe};

    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0b0111'1111);
    ASSERT_EQ(cpu.reg.x, 0x01);
    ASSERT_EQ(cpu.reg.y, 0x00);
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
