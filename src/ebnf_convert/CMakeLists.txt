# ebnf_convert/CMakeLists.txt

project(ebnf_convert)

# ebnfconvert program
add_executable(ebnfconvert ebnf_convert.cpp)

target_compile_definitions(ebnfconvert PRIVATE $<$<PLATFORM_ID:CYGWIN>: GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L _GNU_SOURCE>)

target_compile_options(ebnfconvert PRIVATE
  $<$<CXX_COMPILER_ID:GNU>: -Wall -Werror -Wextra -std=c++26 -pthread -fconcepts-diagnostics-depth=10>
  $<$<CXX_COMPILER_ID:MSVC>: -std:c++latest -W4 -WX>
)
target_compile_options(ebnfconvert PRIVATE
    $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:GNU>>: -O0 -ggdb3>
    $<$<AND:$<CONFIG:release>,$<CXX_COMPILER_ID:GNU>>: -Og>
    $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:MSVC>>: -Od -Zi>
)

target_link_libraries(ebnfconvert flexbisonlib.ebnfparser)
target_link_libraries(ebnfconvert $<$<CXX_COMPILER_ID:MSVC>:getoptlib>)

# tests
add_executable(ebnf_convert.gtest ebnf_convert.gtest.cpp)

target_compile_definitions(ebnf_convert.gtest PRIVATE $<$<PLATFORM_ID:CYGWIN>: GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L _GNU_SOURCE>)

target_compile_options(ebnf_convert.gtest PRIVATE
  $<$<CXX_COMPILER_ID:GNU>: -Wall -Werror -Wextra -std=c++26 -pthread -fconcepts-diagnostics-depth=10>
  $<$<CXX_COMPILER_ID:MSVC>: -std:c++latest -W4 -WX>
)
target_compile_options(ebnf_convert.gtest PRIVATE
    $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:GNU>>: -O0 -ggdb3>
    $<$<AND:$<CONFIG:release>,$<CXX_COMPILER_ID:GNU>>: -Og>
    $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:MSVC>>: -Od -Zi>
)

target_link_libraries(ebnf_convert.gtest flexbisonlib.ebnfparser gmock_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(ebnf_convert.gtest DISCOVERY_MODE PRE_TEST EXTRA_ARGS --gtest_color=yes)

