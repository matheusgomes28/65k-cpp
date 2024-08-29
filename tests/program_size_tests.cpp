/*
This test file will make sure that all branches
checking for program sizes are covered.

Essentially, we are testing that, where the program
is malformed by not having enough argument bytes, we
fail.
*/

import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstddef>


// NOLINTNEXTLINE
TEST(ProgramSizeTests, LdImmediateMalformed)
{
    std::array<std::uint8_t, 1> constexpr program{0xa9};
    emulator::Cpu cpu{};

    ASSERT_FALSE(emulator::execute(cpu, program));
}

// 0xa5, LdZeropageMalformed
TEST(ProgramSizeTests, LdZeropageMalformed0)
{
    std::array<std::uint8_t, 1> constexpr program{0xa5};
    emulator::Cpu cpu{};

    ASSERT_FALSE(emulator::execute(cpu, program));
}

// NOLINTNEXTLINE
TEST(ProgramSizeTests, LdZeropageMalformed1)
{
    std::array<std::uint8_t, 1> constexpr program{0xa6};
    emulator::Cpu cpu{};

    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, LdZeropageMalformed2)
{
    std::array<std::uint8_t, 1> constexpr program{0xa4};
    emulator::Cpu cpu{};

    ASSERT_FALSE(emulator::execute(cpu, program));
}
// 0xb5, LdZeropagePlusRegMalformed
TEST(ProgramSizeTests, LdZeropagePlusRegMalformed0)
{
    std::array<std::uint8_t, 1> constexpr program{0xb6};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, LdZeropagePlusRegMalformed1)
{
    std::array<std::uint8_t, 1> constexpr program{0xb4};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, LdAbsoluteMalformed0)
{
    std::array<std::uint8_t, 1> constexpr program{0xad};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, LdAbsoluteMalformed1)
{
    std::array<std::uint8_t, 1> constexpr program{0xae};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, LdAbsoluteMalformed2)
{
    std::array<std::uint8_t, 1> constexpr program{0xac};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, LdAbsolutePlusRegMalformed0)
{
    std::array<std::uint8_t, 1> constexpr program{0xbd};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, LdAbsolutePlusRegMalformed1)
{
    std::array<std::uint8_t, 1> constexpr program{0xb9};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, LdAbsolutePlusRegMalformed2)
{
    std::array<std::uint8_t, 1> constexpr program{0xbe};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, LdAbsolutePlusRegMalformed3)
{
    std::array<std::uint8_t, 1> constexpr program{0xbc};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, LdindexIndirectMalformed)
{
    std::array<std::uint8_t, 1> constexpr program{0xa1};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, LdIndirectIndexMalformed)
{
    std::array<std::uint8_t, 1> constexpr program{0xb1};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, IncZeropageMalformed0)
{
    std::array<std::uint8_t, 1> constexpr program{0xe6};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, IncZeropageMalformed1)
{
    std::array<std::uint8_t, 1> constexpr program{0xf6};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, IncAbsoluteMalformed)
{
    std::array<std::uint8_t, 1> constexpr program{0x33};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, IncAbsolutePlusXmalformed)
{
    std::array<std::uint8_t, 1> constexpr program{0xf3};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, DecZeropageMalformed)
{
    std::array<std::uint8_t, 1> constexpr program{0xc6};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, StIndirectMalformed)
{
    std::array<std::uint8_t, 1> constexpr program{0x91};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, StZeroPageMalformed0)
{
    std::array<std::uint8_t, 1> constexpr program{0x84};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, StZeroPageMalformed1)
{
    std::array<std::uint8_t, 1> constexpr program{0x85};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, StZeroPageMalformed2)
{
    std::array<std::uint8_t, 1> constexpr program{0x86};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, StAbsoluteMalformed0)
{
    std::array<std::uint8_t, 1> constexpr program{0x8c};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, StAbsoluteMalformed1)
{
    std::array<std::uint8_t, 1> constexpr program{0x8d};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}
TEST(ProgramSizeTests, StAbsoluteMalformed22)
{
    std::array<std::uint8_t, 1> constexpr program{0x8e};
    emulator::Cpu cpu{};
    ASSERT_FALSE(emulator::execute(cpu, program));
}


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
