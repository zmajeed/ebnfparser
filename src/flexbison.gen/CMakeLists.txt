# ebnfparser/flexbison.gen/CMakeLists.txt

project(ebnfparser_flexbison_gen)

add_library(flexbisonlib.ebnfparser STATIC ebnf_lexer.flex.cpp ebnf_parser.bison.cpp)

target_compile_definitions(flexbisonlib.ebnfparser PRIVATE $<$<PLATFORM_ID:CYGWIN>: GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L _GNU_SOURCE>)

target_compile_options(flexbisonlib.ebnfparser PRIVATE
  $<$<CXX_COMPILER_ID:GNU>: -std=c++26 -Wall -Werror -Wextra -pthread -fconcepts-diagnostics-depth=10>
  $<$<CXX_COMPILER_ID:MSVC>: -std:c++latest -W4 -WX>
)
target_compile_options(flexbisonlib.ebnfparser PRIVATE
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:GNU>>: -O0 -ggdb3>
  $<$<AND:$<CONFIG:release>,$<CXX_COMPILER_ID:GNU>>: -Og>
  $<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:MSVC>>: -Od -Zi>
)

target_include_directories(flexbisonlib.ebnfparser PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(flexbisonlib.ebnfparser PUBLIC . $<$<CXX_COMPILER_ID:MSVC>: ${CMAKE_SOURCE_DIR}/thirdparty/flex>)

