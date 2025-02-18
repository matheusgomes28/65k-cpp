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
if(BUILD_PROFILER)
  message(STATUS "Building profiler")
  target_link_libraries(emulator PRIVATE profiler)
  target_compile_definitions(emulator PRIVATE BUILD_PROFILER=)
else()
  message(STATUS "Profiler not build")
endif()

target_link_libraries(emulator PRIVATE fmt::fmt)
add_library(emulator::emulator ALIAS emulator)
