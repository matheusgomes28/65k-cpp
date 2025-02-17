module;

#include <array>
#include <fmt/format.h>
#include <imgui.h>
#include <string>

export module flag_table_ui;

import common_ui;
import emulator;

namespace
{
    /// number of columns in the registers table
    constexpr int columns_count = 8;

    /// Header text for each of the registers cells
    constexpr std::array<std::string, columns_count> headers = {"N", "V", "-", "B", "D", "I", "Z", "C"};

    /// Array of boolens where each index represents whether the
    /// header is selected (true) or not (false)
    std::array<bool, columns_count> column_selected{};

    auto draw_register_cell(bool val, int col_index) -> void
    {
        ImGui::TableSetColumnIndex(col_index);
        std::string const reg_str = fmt::format("{}", val ? "1" : "0");
        // ImGui::Selectable(reg_str.c_str(), column_selected[col_index]);
        emulator::ui::align_text(reg_str, emulator::ui::TextAlign::Center);
    }
} // namespace

export namespace emulator::ui
{
    auto draw_flag_table(emulator::Cpu& cpu)
    {
        if (ImGui::BeginTable("FlagsTable", columns_count, ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable))
        {
            for (auto const& header : headers)
            {
                ImGui::TableSetupColumn(header.c_str());
            }

            // This is submitting a different style and checkboxes to
            // the header cells
            ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
            for (int column = 0; column < columns_count; column++)
            {
                ImGui::TableSetColumnIndex(column);
                const char* column_name =
                    ImGui::TableGetColumnName(column); // Retrieve name passed to TableSetupColumn()
                ImGui::PushID(column);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
                ImGui::Checkbox("##checkall", &column_selected[column]);
                ImGui::PopStyleVar();
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::TableHeader(column_name);
                ImGui::PopID();
            }

            // Submit table contents
            ImGui::TableNextRow();
            draw_register_cell(cpu.flags.n, 0);
            draw_register_cell(cpu.flags.v, 1);
            draw_register_cell(cpu.flags.b, 3);
            draw_register_cell(cpu.flags.d, 4);
            draw_register_cell(cpu.flags.i, 5);
            draw_register_cell(cpu.flags.z, 6);
            draw_register_cell(cpu.flags.c, 7);
            ImGui::EndTable();
        }
    }
} // namespace emulator::ui

module :private;
