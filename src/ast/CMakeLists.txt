# ebnf/ast/CMakeLists.txt

project(ebnf_ast)

add_library(ebnfastlib INTERFACE)
target_sources(ebnfastlib INTERFACE ebnf_ast.h)

target_include_directories(ebnfastlib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# tests

add_executable(ebnf_ast.gtest ebnf_ast.gtest.cpp)

target_link_libraries(ebnf_ast.gtest ebnfastlib gmock_main)

include(GoogleTest)
gtest_discover_tests(ebnf_ast.gtest DISCOVERY_MODE PRE_TEST EXTRA_ARGS --gtest_color=yes)

