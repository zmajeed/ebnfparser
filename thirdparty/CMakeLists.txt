# thirdparty/CMakeLists.txt


if(WIN32)
  message("Build getopt library for Windows")
  add_subdirectory(getopt)
endif()

enable_testing()
