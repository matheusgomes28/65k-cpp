
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.2/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=c8cdc32c03816538ce22781ed72964dc864b2a34a310d3b7104812a5ca2d835d
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMAddPackage(
  NAME raylib
  GITHUB_REPOSITORY raysan5/raylib
  GIT_TAG 5.0
  VERSION 5.0
)

CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG v1.15.2
  VERSION 1.15.2
  OPTIONS
    "INSTALL_GTEST OFF"
    "gtest_force_shared_crt ON"
    "BUILD_GMOCK OFF"
)

CPMAddPackage(
  NAME fmt
  GIT_TAG 11.0.2
  VERSION 11.0.2
  GITHUB_REPOSITORY fmtlib/fmt
)

# Aliases
add_library(gtest::gtest ALIAS gtest)
add_library(gtest::gtest_main ALIAS gtest_main)
add_library(raylib::raylib ALIAS raylib)