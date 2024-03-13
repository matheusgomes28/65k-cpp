import emulator;

#include <gtest/gtest.h>

// NOLINTNEXTLINE
TEST(EmulatorTests, EmulatesLDAImmediateWithNonZero)
{
    std::array<std::uint8_t, 2> program {
        0xa9, 0x0a,
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
    ASSERT_EQ(cpu.flags, 0b0000'0000);
}

// NOLINTNEXTLINE
TEST(EmulatorTests, EmulatesLDAImmediateWithZero)
{
    std::array<std::uint8_t, 2> program {
        0xa9, 0x00,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0x00);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    ASSERT_EQ(cpu.flags, 0b0000'0010);
}

// NOLINTNEXTLINE
TEST(EmulatorTests, EmulatesLDAImmediateWithNegative)
{
    std::array<std::uint8_t, 2> program {
        0xa9, 0xFF,
    };
    emulator::Cpu cpu;
    emulator::execute(cpu, program);

    ASSERT_EQ(cpu.reg.a, 0xFF);
    ASSERT_EQ(cpu.reg.x, 0x00);
    ASSERT_EQ(cpu.reg.y, 0x00);
    ASSERT_EQ(cpu.reg.sp, 0x00);
    ASSERT_EQ(cpu.reg.pc, 0x02);

    // Flags expect
    const auto flag_val0 = cpu.flags[0];
    const auto flag_val1 = cpu.flags[1];
    const auto flag_val2 = cpu.flags[2];
    const auto flag_val3 = cpu.flags[3];
    const auto flag_val4 = cpu.flags[4];
    const auto flag_val5 = cpu.flags[5];
    const auto flag_val6 = cpu.flags[6];
    const auto flag_val7 = cpu.flags[7];
    ASSERT_EQ(cpu.flags, 0b1000'0000);
}


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}