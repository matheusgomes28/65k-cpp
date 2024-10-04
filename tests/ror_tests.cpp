import emulator;

#include "common.h"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <utility>

// NOLINTNEXTLINE
TEST(RORTests, EightBitShift)
{
    // Complex test to make sure we can do
    // a complete 8-bit shift with ROR:

    // TAX			; save byte
    // ROR	A		; b0 into Cb
    // TXA			; restore byte
    // ROR	A		; eight bit rotate

    constexpr std::array<std::uint8_t, 4> program{0xaa, 0x6a, 0x8a, 0x6a};

    emulator::Cpu cpu;
    cpu.reg.a = 0b0101'0101;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0b1010'1010);
    ASSERT_EQ(cpu.reg.x, 0b0101'0101);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x04);

    ASSERT_EQ(cpu.flags, make_flags(0b1000'0001));
}

// NOLINTNEXTLINE
TEST(RORTests, AccCarrySet)
{
    constexpr std::array<std::uint8_t, 1> program{
        {0x6a},
    };

    emulator::Cpu cpu;
    cpu.flags.c = true;
    cpu.reg.a   = 0b1000'0000;

    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0b1100'0000);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x01);

    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(RORTests, AccCarryUnset)
{
    constexpr std::array<std::uint8_t, 1> program{
        {0x6a},
    };

    emulator::Cpu cpu;
    cpu.flags.c = false;
    cpu.reg.a   = 0b1000'0000;

    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0b0100'0000);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x01);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(RORTests, AccCarryZero)
{
    constexpr std::array<std::uint8_t, 1> program{
        {0x6a},
    };

    // inputs that will rotate to the right to set
    // the zero flag.
    // test_data = {init_acc, expected_carry}
    constexpr std::array<std::pair<std::uint8_t, bool>, 2> test_data{{
        {0b0000'0001, true},
        {0b0000'0000, false},
    }};

    for (auto const& [init_acc, expected_carry] : test_data)
    {
        emulator::Cpu cpu;
        cpu.flags.c = false;
        cpu.reg.a   = init_acc;

        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.reg.a, 0b0000'0000);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xff);
        ASSERT_EQ(cpu.reg.pc, 0x01);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0010 | static_cast<std::uint8_t>(expected_carry)));
    }
}

// NOLINTNEXTLINE
TEST(RORTests, ZpCarrySet)
{
    constexpr std::array<std::uint8_t, 2> program{
        {0x66, 0xfe},
    };

    emulator::Cpu cpu;
    cpu.flags.c   = true;
    cpu.mem[0xfe] = 0b1000'0000;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0xfe], 0b1100'0000);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(RORTests, ZpCarryUnset)
{
    constexpr std::array<std::uint8_t, 2> program{
        {0x66, 0xfe},
    };

    emulator::Cpu cpu;
    cpu.flags.c   = false;
    cpu.mem[0xfe] = 0b1000'0000;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0xfe], 0b0100'0000);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(RORTests, ZpCarryZero)
{
    constexpr std::array<std::uint8_t, 2> program{
        {0x66, 0xfe},
    };

    // inputs that will rotate to the right to set
    // the zero flag.
    // test_data = {init_mem, expected_carry}
    constexpr std::array<std::pair<std::uint8_t, bool>, 2> test_data{{
        {0b0000'0001, true},
        {0b0000'0000, false},
    }};

    for (auto const& [init_mem, expected_carry] : test_data)
    {
        emulator::Cpu cpu;
        cpu.flags.c   = false;
        cpu.mem[0xfe] = init_mem;
        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.mem[0xfe], 0b0000'0000);

        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xff);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0010 | static_cast<std::uint8_t>(expected_carry)));
    }
}

// NOLINTNEXTLINE
TEST(RORTests, ZpIndexedCarrySet)
{
    constexpr std::array<std::uint8_t, 2> program{
        {0x76, 0xfe},
    };

    emulator::Cpu cpu;
    cpu.flags.c   = true;
    cpu.reg.x     = 0x02;
    cpu.mem[0x00] = 0b1000'0000;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0x00], 0b1100'0000);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x02);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(RORTests, ZpIndexedCarryUnset)
{
    constexpr std::array<std::uint8_t, 2> program{
        {0x76, 0xfe},
    };

    emulator::Cpu cpu;
    cpu.flags.c   = false;
    cpu.reg.x     = 0x02;
    cpu.mem[0x00] = 0b1000'0000;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0x00], 0b0100'0000);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x02);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(RORTests, ZpIndexedCarryZero)
{
    constexpr std::array<std::uint8_t, 2> program{
        {0x76, 0xfe},
    };

    // inputs that will rotate to the right to set
    // the zero flag.
    // test_data = {init_mem, expected_carry}
    constexpr std::array<std::pair<std::uint8_t, bool>, 2> test_data{{
        {0b0000'0001, true},
        {0b0000'0000, false},
    }};

    for (auto const& [init_mem, expected_carry] : test_data)
    {
        emulator::Cpu cpu;
        cpu.flags.c   = false;
        cpu.reg.x     = 0x02;
        cpu.mem[0x00] = init_mem;
        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.mem[0x00], 0b0000'0000);

        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x02);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xff);
        ASSERT_EQ(cpu.reg.pc, 0x02);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0010 | static_cast<std::uint8_t>(expected_carry)));
    }
}

// NOLINTNEXTLINE
TEST(RORTests, AbsCarrySet)
{
    constexpr std::array<std::uint8_t, 3> program{
        {0x6e, 0xfe, 0xff},
    };

    emulator::Cpu cpu;
    cpu.flags.c     = true;
    cpu.mem[0xfffe] = 0b1000'0000;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0xfffe], 0b1100'0000);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(RORTests, AbsCarryUnset)
{
    constexpr std::array<std::uint8_t, 3> program{
        {0x6e, 0xfe, 0xff},
    };

    emulator::Cpu cpu;
    cpu.flags.c     = false;
    cpu.mem[0xfffe] = 0b1000'0000;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0xfffe], 0b0100'0000);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(RORTests, AbsCarryZero)
{
    constexpr std::array<std::uint8_t, 3> program{
        {0x6e, 0xfe, 0xff},
    };

    // inputs that will rotate to the right to set
    // the zero flag.
    // test_data = {init_mem, expected_carry}
    constexpr std::array<std::pair<std::uint8_t, bool>, 2> test_data{{
        {0b0000'0001, true},
        {0b0000'0000, false},
    }};

    for (auto const& [init_mem, expected_carry] : test_data)
    {
        emulator::Cpu cpu;
        cpu.flags.c     = false;
        cpu.mem[0xfffe] = init_mem;
        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.mem[0xfffe], 0b0000'0000);

        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x00);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xff);
        ASSERT_EQ(cpu.reg.pc, 0x03);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0010 | static_cast<std::uint8_t>(expected_carry)));
    }
}

// NOLINTNEXTLINE
TEST(RORTests, AbsIndexedCarrySet)
{
    constexpr std::array<std::uint8_t, 3> program{
        {0x7e, 0xfe, 0xff},
    };

    emulator::Cpu cpu;
    cpu.flags.c     = true;
    cpu.reg.x       = 0x02;
    cpu.mem[0x0000] = 0b1000'0000;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0x0000], 0b1100'0000);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x02);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(RORTests, AbsIndexedCarryUnset)
{
    constexpr std::array<std::uint8_t, 3> program{
        {0x7e, 0xfe, 0xff},
    };

    emulator::Cpu cpu;
    cpu.flags.c     = false;
    cpu.reg.x       = 0x02;
    cpu.mem[0x0000] = 0b1000'0000;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.mem[0x00], 0b0100'0000);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x02);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0xff);
    ASSERT_EQ(cpu.reg.pc, 0x03);

    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(RORTests, AbsIndexedCarryZero)
{
    constexpr std::array<std::uint8_t, 3> program{
        {0x7e, 0xfe, 0xff},
    };

    // inputs that will rotate to the right to set
    // the zero flag.
    // test_data = {init_mem, expected_carry}
    constexpr std::array<std::pair<std::uint8_t, bool>, 2> test_data{{
        {0b0000'0001, true},
        {0b0000'0000, false},
    }};

    for (auto const& [init_mem, expected_carry] : test_data)
    {
        emulator::Cpu cpu;
        cpu.flags.c   = false;
        cpu.reg.x     = 0x02;
        cpu.mem[0x00] = init_mem;
        emulator::execute(cpu, program);

        ASSERT_EQ(cpu.mem[0x00], 0b0000'0000);

        ASSERT_EQ(cpu.reg.a, 0x00);
        ASSERT_EQ(cpu.reg.x, 0x02);
        ASSERT_EQ(cpu.reg.y, 0x00);
        ASSERT_EQ(cpu.reg.sp, 0xff);
        ASSERT_EQ(cpu.reg.pc, 0x03);

        ASSERT_EQ(cpu.flags, make_flags(0b0000'0010 | static_cast<std::uint8_t>(expected_carry)));
    }
}
