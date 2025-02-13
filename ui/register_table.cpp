module;

#include <fmt/format.h>
#include <imgui.h>

#include <array>
#include <string>

export module register_table_ui;

import emulator;
import common_ui;

namespace
{
    /// number of columns in the registers table
    constexpr int columns_count = 7;

    /// Header text for each of the registers cells
    constexpr std::array<std::string, columns_count> headers = {"PC1", "PC2", "AC", "X", "Y", "SR", "SP"};

    /// Array of boolens where each index represents whether the
    /// header is selected (true) or not (false)
    std::array<bool, columns_count> column_selected{};

    auto draw_register_cell(std::uint8_t val, int col_index) -> void {
        ImGui::TableSetColumnIndex(col_index);
        std::string const reg_str = fmt::format("0x{:02x}", val);
        // ImGui::Selectable(reg_str.c_str(), column_selected[col_index]);
        emulator::ui::align_text(reg_str, emulator::ui::TextAlign::Right);
    }
} // namespace

export namespace emulator::ui
{
    auto draw_register_table(emulator::Cpu& cpu)
    {
        if (ImGui::BeginTable("RegistersTable", columns_count,
                ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable))
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
            draw_register_cell(static_cast<std::uint8_t>(cpu.reg.pc & 0xff), 0);
            draw_register_cell(static_cast<std::uint8_t>((cpu.reg.pc >> 8) & 0xff), 1);
            draw_register_cell(cpu.reg.a, 2);
            draw_register_cell(cpu.reg.x, 3);
            draw_register_cell(cpu.reg.y, 4);
            draw_register_cell(cpu.sr(), 5);
            draw_register_cell(cpu.reg.sp, 6);
            ImGui::EndTable();
        }
    }
} // emulator::ui
