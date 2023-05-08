cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME "equalize_it")
project(${PROJECT_NAME} VERSION 0.0.1)

set(COMPANY_NAME "SmEgDm")
set(PLUGIN_MANUFACTURER_CODE "Smeg")
set(PRODUCT_NAME "Equalize It")
set(PLUGIN_CODE "Eqit")
set(FORMATS Standalone VST3)
set(COPY_PLUGIN_AFTER_BUILD TRUE)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)
add_subdirectory(public)
add_subdirectory(src)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
endif()
