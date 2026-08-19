# parser_api/CMakeLists.txt

project(ebnfparser_api)

add_executable(ebnfparse ebnf_parse.cpp)

target_link_libraries(ebnfparse flexbisonlib.ebnfparser)
target_link_libraries(ebnfparse $<$<CXX_COMPILER_ID:MSVC>:getoptlib>)

# tests

add_executable(ebnf_parse.gtest ebnf_parse.gtest.cpp)

target_link_libraries(ebnf_parse.gtest ebnfastlib flexbisonlib.ebnfparser gmock_main)

include(GoogleTest)
gtest_discover_tests(ebnf_parse.gtest DISCOVERY_MODE PRE_TEST EXTRA_ARGS --gtest_color=yes)

