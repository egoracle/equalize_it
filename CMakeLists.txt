cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME "SpectrumAnalyzer")
project(${PROJECT_NAME} VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)

add_subdirectory(public)

add_subdirectory(src)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
endif()
