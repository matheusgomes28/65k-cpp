import emulator;

#include <cstdint>
#include <iostream>
#include <format>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
    if (argc < 2)
    {
        std::cout << "need to pass filename for the bin";
        return -1;
    }
    auto const filename = argv[1];
    auto file = std::ifstream{filename};

    std::vector<char> program_contents{(std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>()};
    
    emulator::Cpu easy65k;
    emulator::execute(easy65k, {reinterpret_cast<std::uint8_t*>(program_contents.data()), program_contents.size()});
}
