cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)
project(pawntificate)

include(CTest)

enable_testing()

# in-tree dependencies
add_subdirectory(ext/googletest EXCLUDE_FROM_ALL)

# compile time options and features used by all of the targets
add_compile_options(
  -std=c++2a
  -stdlib=libc++
  -fcolor-diagnostics
  # TODO: see if this can be enabled
  # -Wl,--fatal-warnings
  -Wall
  -Wextra
  -Werror
  -pedantic
)

# sub projects
add_library(cxx INTERFACE)
target_include_directories(cxx INTERFACE include)

# main project
add_library(pawntificate
  ${CMAKE_SOURCE_DIR}/src/pawntificate/board.cpp
  ${CMAKE_SOURCE_DIR}/src/pawntificate/evaluate.cpp
)
target_include_directories(pawntificate PUBLIC include)
target_link_libraries(pawntificate PUBLIC cxx)

# subprojects
add_subdirectory(test)
add_subdirectory(tools)
