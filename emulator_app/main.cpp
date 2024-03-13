import emulator;

#include <chrono>
#include <cstdint>
#include <iostream>
#include <format>
#include <fstream>
#include <future>
#include <thread>
#include <vector>

#include <raylib.h>


namespace {

    static constexpr std::array<Color, 16> colour_table {{
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
        {255, 0, 0, 255},
    }};
}



// TODO mutex to protect memory
bool draw(emulator::Cpu& cpu) {
    InitWindow(512, 512, "6502 Graphics");

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            // 0200 - 05FF :
            for (int i = 0x0200; i < 0x0600; ++i ) {
                auto const colour_id = cpu.mem[i] % 16;
                auto const& colour = colour_table[colour_id];

                // Draw this colour rectangle 
                auto const rel_pos = i - 0x0200;
                auto const row = rel_pos / 32;
                auto const col = rel_pos % 32;
                DrawRectangle(col* 16, row * 16, 16, 16, colour);
            }
        EndDrawing();

        // Make sure we only draw at most 30 times per second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/30));
    }

    return true;
}

int main(int argc, char** argv) {
    if (argc < 2)
    {
        std::cout << "need to pass filename for the bin";
        return -1;
    }
    
    emulator::Cpu easy65k;
    auto a1 = std::async(std::launch::async, [](emulator::Cpu& cpu){ return draw(cpu); }, std::ref(easy65k));

    auto const filename = argv[1];
    auto file = std::ifstream{filename};

    std::vector<char> program_contents{(std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>()};
    
    emulator::execute(easy65k, {reinterpret_cast<std::uint8_t*>(program_contents.data()), program_contents.size()});

    a1.wait();
}
