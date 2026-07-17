# thirdparty/getopt/CMakeLists.txt

project(getopt_thirdparty)

add_library(getoptlib STATIC getopt.c getopt_long.c)
set_source_files_properties(getopt.c getopt_long.c PROPERTIES LANGUAGE C)

target_include_directories(getoptlib PUBLIC .)

# tests
add_executable(getopt.gtest getopt.gtest.cpp)

target_compile_options(getopt.gtest PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>: -std:c++latest -W4 -WX>
)
target_compile_options(getopt.gtest PRIVATE
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:MSVC>>: -Od -Zi>
)

target_link_libraries(getopt.gtest getoptlib gmock_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(getopt.gtest DISCOVERY_MODE PRE_TEST EXTRA_ARGS --gtest_color=yes)

