# ebnfparser.no_actions/grammar/CMakeLists.txt

project(ebnfparser_grammar)

find_package(FLEX)
find_package(BISON)

# Determine if using MSVC
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(IS_MSVC 1)
else()
    set(IS_MSVC 0)
endif()

# generated source filename should match .l filename
# cannot use generator expression in flex_target
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  flex_target(flex_files ebnf_lexer.flex.l ${CMAKE_CURRENT_BINARY_DIR}/ebnf_lexer.flex.cpp COMPILE_FLAGS -f)
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
  flex_target(flex_files ebnf_lexer.flex.l ${CMAKE_CURRENT_BINARY_DIR}/ebnf_lexer.flex.cpp COMPILE_FLAGS "-f --nounistd")
endif()
 
# generated source filename should match .y filename
bison_target(bison_files ebnf_parser.bison.y ${CMAKE_CURRENT_BINARY_DIR}/ebnf_parser.bison.cpp DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/ebnf_parser.bison.h COMPILE_FLAGS "-Wall -Wdangling-alias -Werror --report lookaheads,cex,solved --report-file ebnf_grammar_report.txt")

add_flex_bison_dependency(flex_files bison_files)

# export these variables to build standalone parser and lexer in separate directories
set(EBNF_PARSER_BISON_CPP_FILE ${BISON_bison_files_OUTPUT_SOURCE} CACHE STRING "" FORCE)

add_library(flexbisonlib.ebnfparser STATIC ${FLEX_flex_files_OUTPUTS} ${BISON_bison_files_OUTPUT_SOURCE})

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


target_include_directories(flexbisonlib.ebnfparser PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(flexbisonlib.ebnfparser PUBLIC $<$<CXX_COMPILER_ID:MSVC>: ${CMAKE_SOURCE_DIR}/thirdparty/flex>)

add_custom_command(TARGET flexbisonlib.ebnfparser POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/ebnf_grammar_report.txt ${CMAKE_CURRENT_SOURCE_DIR})
