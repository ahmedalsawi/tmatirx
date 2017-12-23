cmake_minimum_required (VERSION 2.6)
project (tmatrix)

## The version number.
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

## Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

## Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

## Get the date of day
execute_process(
  COMMAND date -u
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE DATE_STRING
  OUTPUT_STRIP_TRAILING_WHITESPACE
)


## Configure a header file to pass some of the CMake settings
configure_file(
"${CMAKE_SOURCE_DIR}/version.h.in"
"${CMAKE_BINARY_DIR}/version.h"
)


## Add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}")

## Add library

## Add the executable
add_executable(tmatrix tmatrix.c)
target_link_libraries(tmatrix ncurses)

## Add the install targets
install (TARGETS tmatrix DESTINATION bin)
