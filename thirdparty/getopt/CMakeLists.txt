# thirdparty/getopt/CMakeLists.txt

if(NOT WIN32)
  return()
endif()

project(getopt_thirdparty)

add_library(getoptlib OBJECT getopt.c getopt_long.c)
set_source_files_properties(getopt.c getopt_long.c PROPERTIES LANGUAGE C)

target_include_directories(getoptlib PUBLIC .)

