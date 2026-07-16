# ebnfparser/parser/CMakeLists.txt

project(ebnfparser_bison)

add_executable(ebnfparser ebnf_parser.cpp)

target_compile_definitions(ebnfparser PRIVATE $<$<PLATFORM_ID:CYGWIN>: GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L _GNU_SOURCE>)

target_compile_options(ebnfparser PRIVATE
  $<$<CXX_COMPILER_ID:GNU>: -Wall -Werror -Wextra -std=c++26 -pthread -fconcepts-diagnostics-depth=10>
  $<$<CXX_COMPILER_ID:MSVC>: -std:c++latest -W4 -WX>
)
target_compile_options(ebnfparser PRIVATE
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:GNU>>: -O0 -ggdb3>
  $<$<AND:$<CONFIG:release>,$<CXX_COMPILER_ID:GNU>>: -Og>
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:MSVC>>: -Od -Zi>
)

target_link_libraries(ebnfparser flexbisonlib.ebnfparser)
target_link_libraries(ebnfparser $<$<CXX_COMPILER_ID:MSVC>:getoptlib>)

# tests

add_executable(ebnf_parser.gtest ebnf_parser.gtest.cpp)

target_compile_definitions(ebnf_parser.gtest PRIVATE $<$<PLATFORM_ID:CYGWIN>: GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L _GNU_SOURCE>)

target_compile_options(ebnf_parser.gtest PRIVATE
  $<$<CXX_COMPILER_ID:GNU>: -Wall -Werror -Wextra -std=c++26 -pthread -fconcepts-diagnostics-depth=10>
  $<$<CXX_COMPILER_ID:MSVC>: -std:c++latest -W4 -WX>
)
target_compile_options(ebnf_parser.gtest PRIVATE
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:GNU>>: -O0 -ggdb3>
  $<$<AND:$<CONFIG:release>,$<CXX_COMPILER_ID:GNU>>: -Og>
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:MSVC>>: -Od -Zi>
)

target_link_libraries(ebnf_parser.gtest flexbisonlib.ebnfparser gmock_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(ebnf_parser.gtest DISCOVERY_MODE PRE_TEST EXTRA_ARGS --gtest_color=yes)

