import emulator;

#include <bitset>
#include <gtest/gtest.h>

// TODO : This should be in a common header
// for all the tests to access

/// @brief Makes the `Flags` object based on the
///        bitset given. The bitset should be in the
///        format `0bNVB1DIZC`
/// @param flags an 8-bit bitset representing the flag
/// @return the equivalent `Flags` object
constexpr emulator::Flags make_flags(std::bitset<8> flags)
{
    // CZID B1VN
    return emulator::Flags{
        .n = flags[7],
        .v = flags[6],
        .b = flags[4],
        .d = flags[3],
        .i = flags[2],
        .z = flags[1],
        .c = flags[0],
    };
}


/***************************************************
************* LD* Immediate Tests ******************
***************************************************/

// NOLINTNEXTLINE
TEST(LDTests, LdAImmediateWithNonZero)
{
    std::array<std::uint8_t, 2> program{
        0xa9,
        0x0a,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x0a);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LdAImmediateWithZero)
{
    std::array<std::uint8_t, 2> program{
        0xa9,
        0x00,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LdAImmediateWithNegative)
{
    std::array<std::uint8_t, 2> program{
        0xa9,
        0xFF,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0xFF);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// X Tests
// NOLINTNEXTLINE
TEST(LDTests, LdXImmediateWithNonZero)
{
    std::array<std::uint8_t, 2> program{
        0xa2,
        0x0a,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x0a);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LdXImmediateWithZero)
{
    std::array<std::uint8_t, 2> program{
        0xa2,
        0x00,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LdXImmediateWithNegative)
{
    std::array<std::uint8_t, 2> program{
        0xa2,
        0xFF,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0xFF);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// Y Tests
// NOLINTNEXTLINE
TEST(LDTests, LdYImmediateWithNonZero)
{
    std::array<std::uint8_t, 2> program{
        0xa0,
        0x0a,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x0a);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LdYImmediateWithZero)
{
    std::array<std::uint8_t, 2> program{
        0xa0,
        0x00,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LdYImmediateWithNegative)
{
    std::array<std::uint8_t, 2> program{
        0xa0,
        0xFF,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0xFF);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

/***************************************************
************** LD* Zeropage Tests ******************
***************************************************/

// NOLINTNEXTLINE
TEST(LDTests, LDAZeropageWithNonZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xa5,
        0x22,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x5a);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAZeropageWithZero)
{
    emulator::Cpu cpu;

    // We load `0x00` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xa5,
        0x22,
    };
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAZeropageWithNegative)
{
    emulator::Cpu cpu;

    // We load `0x00` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0xFF;

    constexpr std::array<std::uint8_t, 2> program{
        0xa5,
        0x22,
    };

    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0xFF);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXZeropageWithNonZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xa6,
        0x22,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x5a);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXZeropageWithZero)
{
    emulator::Cpu cpu;

    // We load `0x00` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xa6,
        0x22,
    };
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXZeropageWithNegative)
{
    emulator::Cpu cpu;

    // We load `0x00` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0xFF;

    constexpr std::array<std::uint8_t, 2> program{
        0xa6,
        0x22,
    };

    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0xFF);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYZeropageWithNonZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xa4,
        0x22,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x5a);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYZeropageWithZero)
{
    emulator::Cpu cpu;

    // We load `0x00` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xa4,
        0x22,
    };
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - Zero flag should be true
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYZeropageWithNegative)
{
    emulator::Cpu cpu;

    // We load `0x00` into the `0x22` mem address
    // and expect that to be loaded into the register
    cpu.mem[0x22] = 0xFF;

    constexpr std::array<std::uint8_t, 2> program{
        0xa4,
        0x22,
    };

    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0xFF);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect - n (negative) should be set
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

/***************************************************
*********** LD* Zeropage + Reg Tests ***************
***************************************************/

// NOLINTNEXTLINE
TEST(LDTests, LDAZeropagePlusXWithNonZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x30` mem address
    // and expect that loading zeropage from `0x20`
    // plus an x register value of `0x10` will result
    // into `0x5a` being loaded
    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xb5,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x5a);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAZeropagePlusXWithZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x30` mem address
    // and expect that loading zeropage from `0x20`
    // plus an x register value of `0x10` will result
    // into `0x5a` being loaded
    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xb5,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDAZeropagePlusXWithNegative)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0xFf` into the `0x30` mem address
    // and expect that loading zeropage from `0x20`
    // plus an x register value of `0x10` will result
    // into `0xFF` being loaded
    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0xFF;

    constexpr std::array<std::uint8_t, 2> program{
        0xb5,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0xFF);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXZeropagePlusYWithNonZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x30` mem address
    // and expect that loading zeropage from `0x20`
    // plus an y register value of `0x10` will result
    // into `0x5a` being loaded
    cpu.reg.y     = 0x10;
    cpu.mem[0x30] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xb6,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x5a);
    ASSERT_EQ(cpu.reg.y, 0x10);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXZeropagePlusYWithZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x30` mem address
    // and expect that loading zeropage from `0x20`
    // plus an y register value of `0x10` will result
    // into `0x5a` being loaded
    cpu.reg.y     = 0x10;
    cpu.mem[0x30] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xb6,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x10);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDXZeropagePlusYWithNegative)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0xFf` into the `0x30` mem address
    // and expect that loading zeropage from `0x20`
    // plus an y register value of `0x10` will result
    // into `0xFF` being loaded
    cpu.reg.y     = 0x10;
    cpu.mem[0x30] = 0xFF;

    constexpr std::array<std::uint8_t, 2> program{
        0xb6,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0xFF);
    ASSERT_EQ(cpu.reg.y, 0x10);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYZeropagePlusXWithNonZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x30` mem address
    // and expect that loading zeropage from `0x20`
    // plus an x register value of `0x10` will result
    // into `0x5a` being loaded
    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0x5a;

    constexpr std::array<std::uint8_t, 2> program{
        0xb4,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0x5a);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0000));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYZeropagePlusXWithZero)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0x5a` into the `0x30` mem address
    // and expect that loading zeropage from `0x20`
    // plus an x register value of `0x10` will result
    // into `0x5a` being loaded
    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0x00;

    constexpr std::array<std::uint8_t, 2> program{
        0xb4,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b0000'0010));
}

// NOLINTNEXTLINE
TEST(LDTests, LDYZeropagePlusXWithNegative)
{
    // TODO : Load the value in a zeropage mem
    emulator::Cpu cpu;

    // We load `0xFf` into the `0x30` mem address
    // and expect that loading zeropage from `0x20`
    // plus an x register value of `0x10` will result
    // into `0xFF` being loaded
    cpu.reg.x     = 0x10;
    cpu.mem[0x30] = 0xFF;

    constexpr std::array<std::uint8_t, 2> program{
        0xb4,
        0x20,
    };
    emulator::execute(cpu, program);

    // Registry expect
    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x10);
    ASSERT_EQ(cpu.reg.y, 0xFF);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, make_flags(0b1000'0000));
}