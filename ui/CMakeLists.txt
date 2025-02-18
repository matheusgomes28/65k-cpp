add_library(emulator_ui)
target_sources(emulator_ui
  PUBLIC
    FILE_SET CXX_MODULES FILES
      common.cpp
      flags_table.cpp
      register_table.cpp
)

target_link_libraries(emulator_ui
 PUBLIC
  emulator
  imgui::imgui
  fmt::fmt)

add_library(emulator::ui ALIAS emulator_ui)
