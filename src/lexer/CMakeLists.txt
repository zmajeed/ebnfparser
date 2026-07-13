# ebnfparser/lexer/CMakeLists.txt

project(ebnf_lexer)

add_executable(ebnf_lexer.gtest ebnf_lexer.gtest.cpp)

target_compile_definitions(ebnf_lexer.gtest PRIVATE $<$<PLATFORM_ID:CYGWIN>: GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L _GNU_SOURCE>)

target_compile_options(ebnf_lexer.gtest PRIVATE
  $<$<CXX_COMPILER_ID:GNU>: -Wall -Werror -Wextra -std=c++26 -pthread -fconcepts-diagnostics-depth=10>
  $<$<CXX_COMPILER_ID:MSVC>: -std:c++latest -W4 -WX>
)
target_compile_options(ebnf_lexer.gtest PRIVATE
    $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:GNU>>: -O0 -ggdb3>
    $<$<AND:$<CONFIG:release>,$<CXX_COMPILER_ID:GNU>>: -Og>
    $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:MSVC>>: -Od -Zi>
)

target_include_directories(ebnf_lexer.gtest PRIVATE $<$<CXX_COMPILER_ID:MSVC>: ${CMAKE_CURRENT_SOURCE_DIR}/../winflexheaders>)

target_link_libraries(ebnf_lexer.gtest flexbisonlib.ebnfparser gmock_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(ebnf_lexer.gtest DISCOVERY_MODE PRE_TEST EXTRA_ARGS --gtest_color=yes)

