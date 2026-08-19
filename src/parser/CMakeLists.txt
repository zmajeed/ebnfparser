# ebnfparser/parser/CMakeLists.txt

project(ebnfparser_bison)

add_executable(ebnfparser ebnf_parser.cpp)

target_link_libraries(ebnfparser ebnfastlib flexbisonlib.ebnfparser)
target_link_libraries(ebnfparser
  $<$<CXX_COMPILER_ID:MSVC>: getoptlib>
)

# tests

add_executable(ebnf_parser.gtest ebnf_parser.gtest.cpp)

target_link_libraries(ebnf_parser.gtest ebnfastlib flexbisonlib.ebnfparser gmock_main)

include(GoogleTest)
gtest_discover_tests(ebnf_parser.gtest DISCOVERY_MODE PRE_TEST EXTRA_ARGS --gtest_color=yes)

