#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include <fmt/format.h>
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

import emulator;
import register_table_ui; // make these names better
import flag_table_ui;
import common_ui;

namespace
{

    constexpr std::array<Color, 16> colour_table{{
        {.r = 0, .g = 0, .b = 0, .a = 255}, // Black
        {.r = 255, .g = 255, .b = 255, .a = 255}, // White
        {.r = 136, .g = 0, .b = 0, .a = 255}, // Dark Red
        {.r = 170, .g = 255, .b = 238, .a = 255}, // Light Cyan
        {.r = 204, .g = 68, .b = 204, .a = 255}, // Violet
        {.r = 0, .g = 204, .b = 85, .a = 255}, // Light Green
        {.r = 0, .g = 0, .b = 170, .a = 255}, // Blue
        {.r = 255, .g = 241, .b = 224, .a = 255}, // Something close to white
        {.r = 221, .g = 136, .b = 85, .a = 255}, // Light Brown
        {.r = 102, .g = 68, .b = 0, .a = 255}, // Dark Brown
        {.r = 255, .g = 119, .b = 119, .a = 255}, // Light Red
        {.r = 51, .g = 51, .b = 51, .a = 255}, // Dark Grey
        {.r = 119, .g = 119, .b = 119, .a = 255}, // Light Grey
        {.r = 170, .g = 255, .b = 102, .a = 255}, // Light Lime
        {.r = 0, .g = 136, .b = 255, .a = 255}, // Light Blue
        {.r = 187, .g = 187, .b = 187, .a = 255} // Grey
    }};

    enum class ScrollDriver : std::uint8_t
    {
        Left,
        Right
    };


    void draw_memory_view(emulator::Cpu& cpu)
    {
        static constexpr float offset_view_width  = 75.0f;
        static constexpr float offset_view_height = 200.0f;
        int constexpr num_columns                 = 16;
        int constexpr num_rows                    = 16;

        // Keep the previous value of the scrollbar
        static float scroll_value = 0.0f;
        static auto scroll_driver = ScrollDriver::Left;

        /****************************************************
         Memory Offset View
         ***************************************************/
        ImGui::BeginChild("Offset View", ImVec2(offset_view_width, offset_view_height), ImGuiChildFlags_Borders,
            ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            ImGui::TextUnformatted("Offset");
            ImGui::EndMenuBar();
        }

        if (ImGui::BeginTable("MemoryOffsetTable", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            // Fill the table with data
            for (int row = 0; row < num_rows; row++)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                std::string const offset_str = fmt::format("0x{:02x}", row * 16);
                ImGui::Text("%s", offset_str.c_str());
            }

            ImGui::EndTable();
        }


        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_NoPopupHierarchy))
        {
            scroll_driver = ScrollDriver::Left;
        }


        if (scroll_driver == ScrollDriver::Left)
        {
            auto const left_scroll_value = ImGui::GetScrollY();
            if (scroll_value != left_scroll_value)
            {
                scroll_value = left_scroll_value;
            }
        }
        else
        {
            ImGui::SetScrollY(scroll_value);
        }

        // override scroll progress if not in focus
        // if (left_scroll_value != scroll_value) {
        //     ImGui::SetScrollY(scroll_value);
        // }

        ImGui::EndChild();
        ImGui::SameLine();

        /****************************************************
         Memory Contents View
         ***************************************************/
        auto const available_size = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("ScrollingRegion", ImVec2(available_size.x, offset_view_height), true,
            ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

        // Menu top bar
        if (ImGui::BeginMenuBar())
        {
            ImGui::TextUnformatted("Memory Contents");
            ImGui::EndMenuBar();
        }

        if (ImGui::BeginTable("TableWithoutHeader", num_columns, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            // Fill the table with data
            for (int row = 0; row < num_rows; row++)
            {
                ImGui::TableNextRow();
                for (int col = 0; col < num_columns; col++)
                {
                    ImGui::TableSetColumnIndex(col);
                    auto const data = fmt::format("0x{:02x}", cpu.mem[row * col]);
                    ImGui::Text("%s", data.c_str());
                }
            }

            ImGui::EndTable();
        }

        ImGui::IsItemActive();
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_NoPopupHierarchy))
        {
            scroll_driver = ScrollDriver::Right;
        }

        if (scroll_driver == ScrollDriver::Right)
        {
            auto const right_scroll_value = ImGui::GetScrollY();
            if (right_scroll_value != scroll_value)
            {
                scroll_value = right_scroll_value;
            }
        }
        else
        {
            ImGui::SetScrollY(scroll_value);
        }


        ImGui::EndChild();
    }
} // namespace


// TODO mutex to protect memory
auto draw(emulator::Cpu& cpu) -> bool
{
    // before your game loop
    InitWindow(512, 512, "6502 Graphics");
    SetTargetFPS(30);
    rlImGuiSetup(true); // sets up ImGui with ether a dark or light default theme

    bool window_open = true;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        rlImGuiBegin();

        ImGui::ShowDemoWindow();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(515, 131));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Emulator", &window_open);


        /***************************************************
         * Drawing the top tables for registers and flags   *
         ***************************************************/
        float const margin_x     = 50.0f;
        float const table_width  = (ImGui::GetContentRegionAvail().x - margin_x) * 0.5f;
        float const table_height = (ImGui::GetContentRegionAvail().y * 0.25f); // Adjust table height as needed

        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(margin_x, 0));

        { // table one
            ImGui::BeginChild("LeftTable", ImVec2(table_width, table_height), true);
            emulator::ui::draw_flag_table(cpu);
            ImGui::EndChild();
        }

        ImGui::SameLine();

        // Pop item spacing only
        ImGui::PopStyleVar(1);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        { // table two
            ImGui::BeginChild("RightTable", ImVec2(table_width, table_height), true);
            emulator::ui::draw_register_table(cpu);
            ImGui::EndChild();
        }
        ImGui::PopStyleVar(6);


        /***************************************************
         * Drawing the scrollable memory view               *
         ***************************************************/
        draw_memory_view(cpu);
        ImGui::BeginChild("LeftTable", ImVec2(table_width, table_height), true);
        ImGui::BeginGroup();
        { // left panel with the offsets
        }
        ImGui::EndGroup();
        ImGui::EndChild();

        ImGui::End();


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
        rlImGuiEnd();
        EndDrawing();

        // Make sure we only draw at most 30 times per second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
    }
    rlImGuiShutdown();
    CloseWindow();
    return true;
}

auto main(int argc, char** argv) -> int
{
    if (argc < 2)
    {
        std::cout << "need to pass filename for the bin";
        return -1;
    }

    emulator::Cpu easy65k;

    std::cout << fmt::format("The clock speed was set to {}\n", easy65k.clock_speed);

    auto* const filename = argv[1];
    auto file            = std::ifstream{filename};

    std::vector<char> program_contents{(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};

    emulator::execute(easy65k, {reinterpret_cast<std::uint8_t*>(program_contents.data()), program_contents.size()});

    for (auto const& [function, profile] : easy65k.current_profile())
    {
        std::cout << fmt::format("{}: {:.6f}\n", function, profile);
    }

    draw(easy65k);
}
