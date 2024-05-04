cmake_minimum_required(VERSION 3.28)
project(Kemu65 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN On)

option(CLOCK_SPEED_MHZ "The clock speed for the processor")

add_subdirectory(emulator)
add_subdirectory(emulator_app)

enable_testing()
add_subdirectory(tests)
