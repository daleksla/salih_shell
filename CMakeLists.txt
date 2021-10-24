cmake_minimum_required(VERSION 3.11)

# Project name and a few useful settings. Other commands can pick up the results
project(
  salih_shell
  DESCRIPTION "My attempt to create a terminal shell in pure C"
  LANGUAGES C
)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-Wall -Wextra -pedantic -g)

include_directories(
	include
)

find_package(Doxygen)
if(Doxygen_FOUND)
  execute_process(COMMAND doxygen .config.txt)
else()
  message(STATUS "Doxygen not found, not building docs")
endif()

# Output libname matches target name, with the usual extensions on your system
add_library(display src/display.c)
add_library(parser src/parser.c)
add_library(variables src/variables.c)
add_library(aliases src/aliases.c)
add_library(execution src/execution.c)
# Adding something we can run - Output name matches target name
add_executable(salih_shell src/source.c)
# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error
target_link_libraries(salih_shell display parser aliases variables execution)
