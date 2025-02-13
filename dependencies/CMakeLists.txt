
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.5/CPM.cmake
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
  NAME raylib_imgui
  GITHUB_REPOSITORY raylib-extras/rlImGui
  GIT_TAG 583d4fea67e67d431319974f0625f680d3840dfb
  VERSION 583d4fea67e67d431319974f0625f680d3840dfb
)

add_library(raylib_imgui)
add_library(raylib::imgui ALIAS raylib_imgui)
target_sources(raylib_imgui PRIVATE
  ${raylib_imgui_SOURCE_DIR}/rlImGui.cpp
  ${raylib_imgui_SOURCE_DIR}/rlImGui.h
  ${raylib_imgui_SOURCE_DIR}/rlImGuiColors.h)
target_include_directories(raylib_imgui PUBLIC ${raylib_imgui_SOURCE_DIR})
target_link_libraries(raylib_imgui PUBLIC raylib::raylib imgui::imgui)

CPMAddPackage(
  NAME imgui
  VERSION 1.91.1
  GITHUB_REPOSITORY ocornut/imgui
  DOWNLOAD_ONLY TRUE
)

# CMakeLists.txt from https://gist.githubusercontent.com/rokups/f771217b2d530d170db5cb1e08e9a8f4
file(
  DOWNLOAD
  "https://gist.githubusercontent.com/rokups/f771217b2d530d170db5cb1e08e9a8f4/raw/4c2c14374ab878ca2f45daabfed4c156468e4e27/CMakeLists.txt"
  "${imgui_SOURCE_DIR}/CMakeLists.txt"
  EXPECTED_HASH SHA256=fd62f69364ce13a4f7633a9b50ae6672c466bcc44be60c69c45c0c6e225bb086
)
# Options
set(IMGUI_EXAMPLES FALSE)
set(IMGUI_DEMO FALSE)
set(IMGUI_ENABLE_STDLIB_SUPPORT TRUE)
# FreeType (https://github.com/cpm-cmake/CPM.cmake/wiki/More-Snippets#freetype)
set(FREETYPE_FOUND TRUE)
set(FREETYPE_INCLUDE_DIRS "")
set(FREETYPE_LIBRARIES Freetype::Freetype)

file(GLOB IMGUI_SRC
        ${imgui_SOURCE_DIR}/*.c
        ${imgui_SOURCE_DIR}/*.cpp
)

add_library(imgui)
add_library(imgui::imgui ALIAS imgui)
target_sources(imgui PRIVATE ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

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