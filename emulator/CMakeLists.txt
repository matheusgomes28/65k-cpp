
find_package(fmt REQUIRED)

add_library(emulator)
target_sources(emulator
  PUBLIC
    FILE_SET CXX_MODULES FILES
      emulator.cpp
)

target_link_libraries(emulator PRIVATE fmt::fmt)
