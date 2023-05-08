file(
  GLOB_RECURSE PUBLIC_FILES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_LIST_DIR}/fonts/*.ttf"
  "${CMAKE_CURRENT_LIST_DIR}/icons/*.svg" "${CMAKE_CURRENT_LIST_DIR}/logo.svg")

juce_add_binary_data(public_binaries SOURCES ${PUBLIC_FILES})
