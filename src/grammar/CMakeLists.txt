# ebnfparser/grammar/CMakeLists.txt

project(ebnfparser_grammar)

find_package(FLEX)
find_package(BISON)

# generated source filename should match .l filename
flex_target(flex_files ebnf_lexer.flex.l
  ${CMAKE_CURRENT_BINARY_DIR}/ebnf_lexer.flex.cpp
  COMPILE_FLAGS "-f --nounistd"
)
 
# generated source filename should match .y filename
bison_target(bison_files ebnf_parser.bison.y
  ${CMAKE_CURRENT_BINARY_DIR}/ebnf_parser.bison.cpp
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/ebnf_parser.bison.h
  COMPILE_FLAGS "-Wall -Wdangling-alias -Werror --report lookaheads,cex,solved --report-file ebnf_grammar_report.txt"
)

add_flex_bison_dependency(flex_files bison_files)

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

message("flexbisonlib: flex include dirs \"${FLEX_INCLUDE_DIRS}\"")

target_include_directories(flexbisonlib.ebnfparser PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(flexbisonlib.ebnfparser PUBLIC
  $<$<CXX_COMPILER_ID:MSVC>: ${CMAKE_SOURCE_DIR}/thirdparty/flex>
  $<$<PLATFORM_ID:Darwin>: ${FLEX_INCLUDE_DIRS}>
)

add_custom_command(TARGET flexbisonlib.ebnfparser POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/ebnf_grammar_report.txt ${CMAKE_CURRENT_SOURCE_DIR})

# copy generated files and bison grammar report to source tree
set(GEN_SRC_DIR ${CMAKE_SOURCE_DIR}/src/flexbison.gen)

add_custom_command(TARGET flexbisonlib.ebnfparser POST_BUILD
  COMMENT "Copy generated parser, lexer and bison grammar report to source tree"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/ebnf_grammar_report.txt ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/ebnf_parser.bison.h ${GEN_SRC_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/ebnf_parser.bison.cpp ${GEN_SRC_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/locations.bison.h ${GEN_SRC_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/ebnf_lexer.flex.cpp ${GEN_SRC_DIR}
)

add_custom_command(TARGET flexbisonlib.ebnfparser POST_BUILD
  COMMENT "Strip absolute local paths from generated bison, flex files"
  COMMAND sed -Ei 's!${CMAKE_CURRENT_BINARY_DIR}!${GEN_SRC_DIR}!' ${GEN_SRC_DIR}/ebnf_parser.bison.h
  COMMAND sed -Ei 's!${CMAKE_SOURCE_DIR}!.!' ${GEN_SRC_DIR}/ebnf_parser.bison.h
  COMMAND sed -Ei 's!${CMAKE_CURRENT_BINARY_DIR}!${GEN_SRC_DIR}!' ${GEN_SRC_DIR}/ebnf_parser.bison.cpp
  COMMAND sed -Ei 's!${CMAKE_SOURCE_DIR}!.!' ${GEN_SRC_DIR}/ebnf_parser.bison.cpp
  COMMAND sed -Ei 's!${CMAKE_CURRENT_BINARY_DIR}!${GEN_SRC_DIR}!' ${GEN_SRC_DIR}/locations.bison.h
  COMMAND sed -Ei 's!${CMAKE_SOURCE_DIR}!.!' ${GEN_SRC_DIR}/locations.bison.h
  COMMAND sed -Ei 's!${CMAKE_CURRENT_BINARY_DIR}!${GEN_SRC_DIR}!' ${GEN_SRC_DIR}/ebnf_lexer.flex.cpp
  COMMAND sed -Ei 's!${CMAKE_SOURCE_DIR}!.!' ${GEN_SRC_DIR}/ebnf_lexer.flex.cpp
)

add_custom_command(TARGET flexbisonlib.ebnfparser POST_BUILD
  COMMAND sed -Ei [[s/(YY_YY).*(_EBNF_PARSER_BISON_H)/\1\2/]] ${GEN_SRC_DIR}/ebnf_parser.bison.h
  COMMAND sed -Ei [[s/(YY_YY).*(_LOCATIONS_BISON_H)/\1\2/]] ${GEN_SRC_DIR}/locations.bison.h
  VERBATIM
)

enable_testing()
