# ebnfparser/flexbison.gen/CMakeLists.txt

project(ebnfparser_flexbison_gen)

add_library(flexbisonlib.ebnfparser STATIC ebnf_lexer.flex.cpp ebnf_parser.bison.cpp)

target_include_directories(flexbisonlib.ebnfparser PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src
  $<$<CXX_COMPILER_ID:MSVC>: ${CMAKE_SOURCE_DIR}/thirdparty/flex>
  $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Emscripten>: ${CMAKE_SOURCE_DIR}/thirdparty/flex>
)

target_link_libraries(flexbisonlib.ebnfparser ebnfastlib)

enable_testing()
