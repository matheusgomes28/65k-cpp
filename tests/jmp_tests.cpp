import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>

// NOLINTNEXTLINE
TEST(JMPTests, AbsoluteTests)
{
    std::array<std::uint8_t, 0xffff> program{};

    emulator::Cpu cpu;

    // 0x0000 -> 0x5554 (3 cycles)
    program[0x0000] = 0x4c;
    program[0x0001] = 0x54;
    program[0x0002] = 0x55;

    // 0x5554 -> 0xfffe (3 cycles)
    program[0x5554] = 0x4c;
    program[0x5555] = 0xfe;
    program[0x5556] = 0xff;

    // noop to end the program
    // 0xfffe noop (2 cycles)
    program[0xfffe] = 0xea;


    EXPECT_EQ(emulator::execute(cpu, program), 8);
    EXPECT_EQ(cpu.reg.pc, 0xffff);
}

// NOLINTNEXTLINE
TEST(JMPTests, IndirectTests)
{
    std::array<std::uint8_t, 0xffff> program{};

    emulator::Cpu cpu;

    // 0x0000 -> 0x5554 (3 cycles)
    program[0x0000] = 0x6c;
    program[0x0001] = 0x54;
    program[0x0002] = 0x55;

    // 0x5554 -> 0xfffe (3 cycles)
    program[0x5554] = 0xfe;
    program[0x5555] = 0xff;

    // noop to end the program
    // 0xfffe noop (2 cycles)
    program[0xfffe] = 0xea;


    EXPECT_EQ(emulator::execute(cpu, program), 8);
    EXPECT_EQ(cpu.reg.pc, 0xffff);
}
