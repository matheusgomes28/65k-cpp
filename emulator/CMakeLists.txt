add_library(emulator)
target_sources(emulator
  PUBLIC
    FILE_SET CXX_MODULES FILES
      emulator.cpp
)

if(BUILD_PROFILER)
  message(STATUS "Building profiler")
  target_link_libraries(emulator PRIVATE profiler)
  target_compile_definitions(emulator PRIVATE BUILD_PROFILER=)
else()
  message(STATUS "Profiler not build")
endif()
