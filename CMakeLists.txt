cmake_minimum_required(VERSION 3.22)
project(RiowVisualizer VERSION 0.1.0)

# Build instructions and JUCE setup are in README.md.
include(FetchContent)
FetchContent_Declare(
  JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.6
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(RiowVisualizer
    COMPANY_NAME "Riow"
    BUNDLE_ID "com.riow.visualizer"
    PLUGIN_MANUFACTURER_CODE Riow
    PLUGIN_CODE Rv01
    FORMATS VST3 Standalone
    PRODUCT_NAME "Riow Visualizer"
    DESCRIPTION "Audio visualizer and MIDI note monitor"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(RiowVisualizer PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)
target_compile_features(RiowVisualizer PRIVATE cxx_std_17)
target_compile_definitions(RiowVisualizer PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)
target_link_libraries(RiowVisualizer PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
