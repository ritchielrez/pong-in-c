cmake_minimum_required(VERSION 3.14) # For FetchContent which is used by cpm

project(pong-in-c VERSION 0.1.0 LANGUAGES CXX C)

include(cmake/CompilerWarnings.cmake)
include(cmake/StandardProjectSettings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/cpm.cmake)

add_executable(main src/main.c)

CPMAddPackage(
  NAME raylib
  URL  https://github.com/raysan5/raylib/archive/refs/tags/5.0.zip
)

target_link_libraries(main PRIVATE raylib)
