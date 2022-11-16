cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME "Equalizer")

set(FORMATS VST3 Standalone)

file(STRINGS VERSION CURRENT_VERSION)

project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})

set(CMAKE_XCODE_GENERATE_SCHEME OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET
    "10.13"
    CACHE STRING "Support macOS down to High Sierra")

set_property(GLOBAL PROPERTY USE_FOLDERS YES)

option(JUCE_ENABLE_MODULE_SOURCE_GROUPS
       "Show all module sources in IDE projects" ON)

add_subdirectory(external/juce)

juce_add_plugin(
  ${PROJECT_NAME}
  COMPANY_NAME
  smegdm
  BUNDLE_ID
  ""
  COPY_PLUGIN_AFTER_BUILD
  TRUE
  PLUGIN_MANUFACTURER_CODE
  Pamp
  PLUGIN_CODE
  P001
  FORMATS
  ${FORMATS}
  PRODUCT_NAME
  ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
target_sources(${PROJECT_NAME} PRIVATE ${SourceFiles})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "")

source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PREFIX ""
  FILES ${SourceFiles})

foreach(target ${FORMATS} "All")
  if(TARGET ${PROJECT_NAME}_${target})
    set_target_properties(
      ${PROJECT_NAME}_${target}
      PROPERTIES
        FOLDER "Targets"
        XCODE_SCHEME_EXECUTABLE
        "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/extras/AudioPluginHost/Builds/MacOSX/build/Debug/AudioPluginHost.app"
        XCODE_GENERATE_SCHEME ON)
  endif()
endforeach()

set(JUCE_DEPENDENCIES
    juce::juce_core
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_audio_formats
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_gui_extra)

target_compile_definitions(
  ${PROJECT_NAME}
  PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0
         PROJECT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE ${JUCE_DEPENDENCIES}
  PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_lto_flags
         juce::juce_recommended_warning_flags)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
endif()
