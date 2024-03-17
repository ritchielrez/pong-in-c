cmake_minimum_required(VERSION 3.10.0)

project(pong-in-c VERSION 0.1.0 LANGUAGES CXX C)

include(cmake/CompilerWarnings.cmake)
include(cmake/StandardProjectSettings.cmake)
include(cmake/StaticAnalyzers.cmake)

add_executable(main src/main.c)

target_include_directories(main PRIVATE vendor/raylib-5.0/include)
target_link_directories(main PRIVATE vendor/raylib-5.0/lib)

target_link_libraries(main PRIVATE raylib)

if(WIN32)
	target_link_libraries(main PRIVATE opengl32 winmm)
	if(NOT MSVC)
		target_link_libraries(main PRIVATE mingw32)
	endif()
endif()
