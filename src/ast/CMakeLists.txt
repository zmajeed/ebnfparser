# ebnf/ast/CMakeLists.txt

project(ebnf_ast)

add_executable(ebnf_ast.gtest ebnf_ast.gtest.cpp)

target_compile_definitions(ebnf_ast.gtest PRIVATE $<$<PLATFORM_ID:CYGWIN>: GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L _GNU_SOURCE>)

target_compile_options(ebnf_ast.gtest PRIVATE
  $<$<CXX_COMPILER_ID:GNU>: -std=c++26 -Wall -Werror -Wextra -pthread -fconcepts-diagnostics-depth=10>
  $<$<CXX_COMPILER_ID:MSVC>: -std:c++latest -W4 -WX>
)
target_compile_options(ebnf_ast.gtest PRIVATE
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:GNU>>: -O0 -ggdb3>
  $<$<AND:$<CONFIG:release>,$<CXX_COMPILER_ID:GNU>>: -Og>
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:MSVC>>: -Od -Zi>
)

target_link_libraries(ebnf_ast.gtest gmock_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(ebnf_ast.gtest DISCOVERY_MODE PRE_TEST EXTRA_ARGS --gtest_color=yes)

