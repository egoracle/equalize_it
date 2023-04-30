juce_add_plugin(
  ${PROJECT_NAME}
  COPY_PLUGIN_AFTER_BUILD
  TRUE
  PLUGIN_MANUFACTURER_CODE
  Pamp
  PLUGIN_CODE
  P001
  FORMATS
  ${FORMATS}
  COMPANY_NAME
  ${COMPANY_NAME}
  PRODUCT_NAME
  ${PRODUCT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/*.cpp
     ${CMAKE_CURRENT_LIST_DIR}/*.hpp)
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "")

source_group(
  TREE ${CMAKE_CURRENT_LIST_DIR}
  PREFIX ""
  FILES ${SRC_FILES})

target_compile_definitions(
  ${PROJECT_NAME}
  PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0
         PROJECT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE ${JUCE_DEPENDENCIES} public_binaries friz
  PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_lto_flags
         juce::juce_recommended_warning_flags)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/app ${CMAKE_CURRENT_LIST_DIR}/shared
         ${CMAKE_CURRENT_LIST_DIR}/widgets ${CMAKE_CURRENT_LIST_DIR}/pages
         ${CMAKE_CURRENT_LIST_DIR}/../external/spline/src)
