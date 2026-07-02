# ebnfparser.no_actions/grammar/CMakeLists.txt

project(ebnfparser_grammar)

find_package(FLEX)
find_package(BISON)

# generated source filename should match .l filename
flex_target(flex_files ebnf_lexer.flex.l ${CMAKE_CURRENT_BINARY_DIR}/ebnf_lexer.flex.cpp COMPILE_FLAGS -f)

# generated source filename should match .y filename
bison_target(bison_files ebnf_parser.bison.y ${CMAKE_CURRENT_BINARY_DIR}/ebnf_parser.bison.cpp COMPILE_FLAGS "-Wall -Wdangling-alias -Werror --report lookaheads,cex,solved --report-file ebnf_grammar_report.txt")

add_flex_bison_dependency(flex_files bison_files)

# export these variables to build standalone parser and lexer in separate directories
set(EBNF_PARSER_BISON_CPP_FILE ${BISON_bison_files_OUTPUT_SOURCE} CACHE STRING "" FORCE)

add_library(flexbisonlib.ebnfparser STATIC ${FLEX_flex_files_OUTPUTS} ${BISON_bison_files_OUTPUT_SOURCE})

target_compile_definitions(flexbisonlib.ebnfparser PRIVATE $<$<PLATFORM_ID:CYGWIN>: GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L _GNU_SOURCE>)
target_compile_options(flexbisonlib.ebnfparser PRIVATE -Wall -Werror -Wextra -std=c++26 -pthread -fconcepts-diagnostics-depth=10)
target_compile_options(flexbisonlib.ebnfparser PRIVATE $<$<CONFIG:debug>: -O0 -ggdb3> $<$<CONFIG:release>: -Og>)

target_include_directories(flexbisonlib.ebnfparser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET flexbisonlib.ebnfparser POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/ebnf_grammar_report.txt ${CMAKE_CURRENT_SOURCE_DIR})
