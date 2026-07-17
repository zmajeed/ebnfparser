# ebnfparser/src/CMakeLists.txt

project(ebnfparser_src)

if(NOT WIN32)
  message("Generate bison, flex files from src/grammar")
  add_subdirectory(grammar)
else()
  message("Use checked-in generated bison, flex files from src/flexbison.gen")
  add_subdirectory(flexbison.gen)
endif()

add_subdirectory(parser)
add_subdirectory(lexer)

add_subdirectory(ast)

add_subdirectory(parser_api)

add_subdirectory(ebnf_convert)

enable_testing()
