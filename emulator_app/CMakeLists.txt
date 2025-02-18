
add_executable(emulator_app main.cpp)
target_link_libraries(emulator_app
 PRIVATE
  emulator::emulator
  emulator::ui
  fmt::fmt
  raylib::imgui)
