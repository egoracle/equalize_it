# JUCE
add_subdirectory(juce)

set_property(GLOBAL PROPERTY USE_FOLDERS YES PARENT_SCOPE)

option(JUCE_ENABLE_MODULE_SOURCE_GROUPS
       "Show all module sources in IDE projects" ON)

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
    juce::juce_gui_extra
    PARENT_SCOPE)

# Friz
juce_add_module("${CMAKE_CURRENT_LIST_DIR}/friz/Source/friz")

# Spline
set(SPLINE_PATH
    "${CMAKE_CURRENT_LIST_DIR}/spline/src"
    PARENT_SCOPE)
