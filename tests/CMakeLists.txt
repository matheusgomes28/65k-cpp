include(CTest)

find_package(GTest REQUIRED COMPONENTS gtest_main)


# This is meant to be used on single .cpp files that
# export GTest functions. Use "create_tests" by calling
# it with the filename minus the extension.
# Note that the file must be in the "tests" directory
function(create_tests cpp_file_name)
  add_executable(${cpp_file_name} ${cpp_file_name}.cpp)
  target_link_libraries(${cpp_file_name}
  PRIVATE
    emulator
    gtest::gtest)

  add_test(NAME ${cpp_file_name} COMMAND ${cpp_file_name})
endfunction()


create_tests(emulator_tests)
create_tests(ld_immediate_tests)
create_tests(ld_zeropage_tests)
create_tests(ld_absolute_tests)
create_tests(ld_absolute_indexed_tests)
create_tests(ld_index_indirect_tests)
create_tests(ld_indirect_indexed_tests)
create_tests(branch_tests)
create_tests(tx_tests)
create_tests(increment_tests)
