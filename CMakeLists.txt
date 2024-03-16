cmake_minimum_required(VERSION 3.10.0)

project(pong-in-c VERSION 0.1.0 LANGUAGES CXX C)

include(cmake/CompilerWarnings.cmake)
include(cmake/StandardProjectSettings.cmake)
include(cmake/StaticAnalyzers.cmake)

add_executable(main src/main.c)

target_include_directories(main PRIVATE vendor/raylib-5.0/include)
target_link_directories(main PRIVATE vendor/raylib-5.0/lib)

if(WINDOWS)
	target_include_directories(main PRIVATE vendor/glfw-3.4.bin.WIN64/include)
	if(NOT MSVC)
		target_include_directories(main PRIVATE vendor/glfw-3.4.bin.WIN64/lib-mingw-w64)
	endif()
endif()

target_link_libraries(main PRIVATE raylib opengl32 winmm)

if(WINDOWS)
	if(NOT MSVC)
		target_link_libraries(main PRIVATE mingw32)
	endif()
endif()