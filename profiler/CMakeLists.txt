add_library(profiler)

target_sources(profiler PUBLIC
 FILE_SET CXX_MODULES FILES profiler.cpp)
