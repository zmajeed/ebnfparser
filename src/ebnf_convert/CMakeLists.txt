# ebnf_convert/CMakeLists.txt

project(ebnf_convert)

# ebnfconvert program
add_executable(ebnfconvert ebnf_convert.cpp)

target_link_libraries(ebnfconvert ebnfastlib flexbisonlib.ebnfparser)
target_link_libraries(ebnfconvert
  $<$<CXX_COMPILER_ID:MSVC>:getoptlib>
)

# tests
add_executable(ebnf_convert.gtest ebnf_convert.gtest.cpp)

target_link_libraries(ebnf_convert.gtest flexbisonlib.ebnfparser gmock_main)

include(GoogleTest)
gtest_discover_tests(ebnf_convert.gtest DISCOVERY_MODE PRE_TEST EXTRA_ARGS --gtest_color=yes)

