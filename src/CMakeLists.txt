# ebnfparser/src/CMakeLists.txt

project(ebnfparser_src)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG main
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(googletest)

find_package(FLEX)
find_package(BISON)

if(NOT WIN32 AND BISON_FOUND AND FLEX_FOUND)
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
