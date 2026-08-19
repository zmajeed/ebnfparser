# ebnfparser/CMakeLists.txt

cmake_minimum_required(VERSION 4.2)

project(ebnfparser)

# baseline compiler options
# really detest these ugly add_compile_definitions, add_compile_options with semicolons and quotes

# define macros
add_compile_definitions(
  "$<$<PLATFORM_ID:CYGWIN>:GTEST_HAS_PTHREAD=1;_POSIX_C_SOURCE=200809L;_GNU_SOURCE>"
)

# common options for C++ and C
add_compile_options(
  "$<$<CXX_COMPILER_ID:GNU>:-Wall;-Werror;-Wextra;-pthread>"
  "$<$<CXX_COMPILER_ID:MSVC>:-W4;-WX>"
  "$<$<CXX_COMPILER_ID:Clang>:-Wall;-Werror;-Wextra;-pthread>"
)

# C++ options, C++26
add_compile_options(
  "$<$<COMPILE_LANG_AND_ID:CXX,GNU>:-std=c++26;-fconcepts-diagnostics-depth=10>"
  "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:-std:c++latest>"
  "$<$<COMPILE_LANG_AND_ID:CXX,Clang>:-std=c++26;-Wno-unqualified-std-cast-call>"
)

# C options, C23
add_compile_options(
  "$<$<COMPILE_LANG_AND_ID:C,GNU>:-std=c23>"
  "$<$<COMPILE_LANG_AND_ID:C,MSVC>:-std:clatest>"
  "$<$<COMPILE_LANG_AND_ID:C,Clang>:-std=c23>"
)

# debug options
add_compile_options(
  "$<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:GNU>>:-O0;-ggdb3>"
  "$<$<AND:$<CONFIG:release>,$<CXX_COMPILER_ID:GNU>>:-Og>"
  "$<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:MSVC>>:-Od;-Zi>"
  "$<$<AND:$<CONFIG:debug>,$<CXX_COMPILER_ID:Clang>>:-O0;-ggdb3>"
  "$<$<AND:$<CONFIG:release>,$<CXX_COMPILER_ID:Clang>>:-Og>"
)

enable_testing()

# source directories

add_subdirectory(thirdparty)

add_subdirectory(src)
add_subdirectory(tests)

# external dependencies
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG main
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(googletest)

execute_process(
  COMMAND bash -c "git log -1 --pretty=format:\"%h %cd %s\n\" --date=format-local:%F_%T_%Z; git remote -v | head -1; git branch"
  WORKING_DIRECTORY ${googletest_SOURCE_DIR}
  OUTPUT_VARIABLE cmdout
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "googletest source:\n${cmdout}")

