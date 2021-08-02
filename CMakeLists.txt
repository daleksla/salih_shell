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

add_compile_options(-Wall -Wextra -pedantic)

include_directories(
	include
)

# Only do these if this is the main project, and not if it is included through add_subdirectory
#if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
#
#  # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
#
#  # Let's ensure -std=c++xx instead of -std=g++xx
#  set(CMAKE_CXX_EXTENSIONS OFF)
#
#  # Let's nicely support folders in IDEs
#  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#
#  # Docs only available if this is the main app
#  find_package(Doxygen)
#  if(Doxygen_FOUND)
#    add_subdirectory(docs)
#  else()
#    message(STATUS "Doxygen not found, not building docs")
#  endif()
#endif()

# Output libname matches target name, with the usual extensions on your system
add_library(environment src/environment.c)
# Adding something we can run - Output name matches target name
add_executable(source src/source.c)
# Make sure you link your targets with this command. It can also link libraries and
# even flags, so linking a target that does not exist will not give a configure-time error
target_link_libraries(source environment)
