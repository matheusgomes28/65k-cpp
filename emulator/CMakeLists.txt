
find_package(fmt REQUIRED)

add_library(emulator)
target_sources(emulator
  PUBLIC
    FILE_SET CXX_MODULES FILES
      emulator.cpp
)

if (CLOCK_SPEED_MHZ)
  message(STATUS "The clock speed was set to ${CLOCK_SPEED_MHZ}")
  target_compile_definitions(emulator PRIVATE CLOCK_SPEED_MHZ=${CLOCK_SPEED_MHZ})
endif()

target_link_libraries(emulator PRIVATE fmt::fmt)
