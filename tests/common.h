#ifndef TESTS_COMMON_H
#define TESTS_COMMON_H

#include <bitset>

/// @brief Makes the `Flags` object based on the
///        bitset given. The bitset should be in the
///        format `0bNVB1DIZC`
/// @param flags an 8-bit bitset representing the flag
/// @return the equivalent `Flags` object
// constexpr emulator::Flags make_flags(std::bitset<8> flags)
inline emulator::Flags make_flags(std::bitset<8> flags)
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

#endif // TESTS_COMMON_H