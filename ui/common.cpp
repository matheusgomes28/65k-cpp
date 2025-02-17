module;

#include <fmt/format.h>
#include <imgui.h>

#include <cstdint>

export module common_ui;

export namespace emulator::ui
{

    enum class TextAlign : std::uint8_t
    {
        Left,
        Right,
        Center,
    };

    void align_text(std::string const& text, TextAlign align)
    {
        ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        float cellWidth = ImGui::GetColumnWidth();

        float padding = 0;
        switch (align)
        {
        case TextAlign::Left:
            padding = 0;
            break;
        case TextAlign::Right:
            padding = cellWidth - textSize.x;
            break;
        case TextAlign::Center:
            padding = (cellWidth - textSize.x) * 0.5f;
            break;
        }

        // Ensure text stays centered
        if (padding > 0)
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
        }

        ImGui::TextUnformatted(text.c_str());
    }
} // namespace emulator::ui