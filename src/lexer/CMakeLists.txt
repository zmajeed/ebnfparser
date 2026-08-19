# ebnfparser/lexer/CMakeLists.txt

project(ebnf_lexer)

# tests

add_executable(ebnf_lexer.gtest ebnf_lexer.gtest.cpp)

target_link_libraries(ebnf_lexer.gtest flexbisonlib.ebnfparser gmock_main)

include(GoogleTest)
gtest_discover_tests(ebnf_lexer.gtest DISCOVERY_MODE PRE_TEST EXTRA_ARGS --gtest_color=yes)

