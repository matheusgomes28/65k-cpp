include(CTest)

find_package(GTest REQUIRED COMPONENTS gtest_main)

add_executable(emulator_tests emulator_tests.cpp)
target_link_libraries(emulator_tests
 PRIVATE
  emulator
  gtest::gtest)

add_test(NAME emulator_tests COMMAND emulator_tests)
