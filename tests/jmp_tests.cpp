import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>

// NOLINTNEXTLINE
TEST(JMPTests, AbsoluteTests)
{
    std::array<std::uint8_t, 0x7fff> program{};

    emulator::Cpu cpu;

    // 0x0000 -> 0x5554 (3 cycles)
    program[0x0000] = 0x4c; // will end up at 0x8000
    program[0x0001] = 0xa4; // represent 0xaaa4
    program[0x0002] = 0xaa;

    // 0x5554 -> 0xfffe (3 cycles)
    program[0x2aa4] = 0x4c;
    program[0x2aa5] = 0xfe;
    program[0x2aa6] = 0xff;

    // noop to end the program
    // 0xfffe noop (2 cycles)
    program[0x7ffe] = 0xea; // will end up at 0xfffe

    EXPECT_EQ(emulator::execute(cpu, program), 8);
    EXPECT_EQ(cpu.reg.pc, 0xffff);
}
