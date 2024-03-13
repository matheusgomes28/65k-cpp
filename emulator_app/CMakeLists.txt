
find_package(fmt REQUIRED)

add_executable(emulator_app main.cpp)
target_link_libraries(emulator_app
 PRIVATE
  emulator
  fmt::fmt)
