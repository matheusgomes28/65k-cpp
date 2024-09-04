import emulator;

#include <chrono>
#include <cstdint>
#include <format>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include <raylib.h>

namespace
{

    static constexpr std::array<Color, 16> colour_table{{
        {0, 0, 0, 255}, // Black
        {255, 255, 255, 255}, // White
        {136, 0, 0, 255}, // Dark Red
        {170, 255, 238, 255}, // Light Cyan
        {204, 68, 204, 255}, // Violet
        {0, 204, 85, 255}, // Light Green
        {0, 0, 170, 255}, // Blue
        {255, 241, 224, 255}, {221, 136, 85, 255}, // Light Brown
        {102, 68, 0, 255}, // Dark Brown
        {255, 119, 119, 255}, // Light Red
        {51, 51, 51, 255}, // Dark Grey
        {119, 119, 119, 255}, // Light Grey
        {170, 255, 102, 255}, // Light Lime
        {0, 136, 255, 255}, // Light Blue
        {187, 187, 187, 255} // Grey
    }};
}


// TODO mutex to protect memory
bool draw(emulator::Cpu& cpu)
{
    InitWindow(512, 512, "6502 Graphics");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        // 0200 - 05FF :
        for (int i = 0x0200; i < 0x0600; ++i)
        {
            auto const colour_id = cpu.mem[i] % 16;
            auto const& colour   = colour_table[colour_id];

            // Draw this colour rectangle
            auto const rel_pos = i - 0x0200;
            auto const row     = rel_pos / 32;
            auto const col     = rel_pos % 32;
            DrawRectangle(col * 16, row * 16, 16, 16, colour);
        }
        EndDrawing();

        // Make sure we only draw at most 30 times per second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
    }

    return true;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "need to pass filename for the bin";
        return -1;
    }

    emulator::Cpu easy65k;

    std::cout << std::format("The clock speed was set to {}\n", easy65k.clock_speed);

    auto const filename = argv[1];
    auto file           = std::ifstream{filename};

    std::vector<char> program_contents{(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};

    emulator::execute(easy65k, {reinterpret_cast<std::uint8_t*>(program_contents.data()), program_contents.size()});

    for (auto const& [function, profile] : easy65k.current_profile())
    {
        std::cout << std::format("{}: {:.6f}\n", function, profile);
    }

    draw(easy65k);
}
