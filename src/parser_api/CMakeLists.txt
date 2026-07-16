# parser_api/CMakeLists.txt

project(ebnfparser_api)

add_executable(ebnfparse ebnf_parse.cpp)

target_compile_definitions(ebnfparse PRIVATE $<$<PLATFORM_ID:CYGWIN>: _POSIX_C_SOURCE=200809L _GNU_SOURCE>)

target_compile_options(ebnfparse PRIVATE
  $<$<CXX_COMPILER_ID:GNU>: -Wall -Werror -Wextra -std=c++26 -pthread -fconcepts-diagnostics-depth=10>
  $<$<CXX_COMPILER_ID:MSVC>: -std:c++latest -W4 -WX>
)
target_compile_options(ebnfparse PRIVATE
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:GNU>>: -O0 -ggdb3>
  $<$<AND:$<CONFIG:release>,$<CXX_COMPILER_ID:GNU>>: -Og>
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:MSVC>>: -Od -Zi>
)

target_link_libraries(ebnfparse flexbisonlib.ebnfparser)
target_link_libraries(ebnfparse $<$<CXX_COMPILER_ID:MSVC>:getoptlib>)

# tests

add_executable(ebnf_parse.gtest ebnf_parse.gtest.cpp)

target_compile_definitions(ebnf_parse.gtest PRIVATE $<$<PLATFORM_ID:CYGWIN>: GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L _GNU_SOURCE>)

target_compile_options(ebnf_parse.gtest PRIVATE
  $<$<CXX_COMPILER_ID:GNU>: -Wall -Werror -Wextra -std=c++26 -pthread -fconcepts-diagnostics-depth=10>
  $<$<CXX_COMPILER_ID:MSVC>: -std:c++latest -W4 -WX>
)
target_compile_options(ebnf_parse.gtest PRIVATE
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:GNU>>: -O0 -ggdb3>
  $<$<AND:$<CONFIG:release>,$<CXX_COMPILER_ID:GNU>>: -Og>
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:MSVC>>: -Od -Zi>
)

target_link_libraries(ebnf_parse.gtest flexbisonlib.ebnfparser gmock_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(ebnf_parse.gtest DISCOVERY_MODE PRE_TEST EXTRA_ARGS --gtest_color=yes)

