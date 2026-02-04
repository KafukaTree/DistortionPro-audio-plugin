# DistortionPro - JUCE-based Distortion Plugin
# CMake Project File

cmake_minimum_required(VERSION 3.16)

# Detect OS
if(WIN32)
    set(PLATFORM_WINDOWS TRUE)
elseif(APPLE)
    set(PLATFORM_MACOS TRUE)
elseif(UNIX)
    set(PLATFORM_LINUX TRUE)
endif()

project(DistortionPro VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find JUCE
if(NOT JUCE_PATH)
    message(FATAL_ERROR "JUCE_PATH must be set to JUCE installation directory")
endif()

# Add JUCE as subdirectory
add_subdirectory(${JUCE_PATH} EXCLUDE_FROM_ALL)

# Plugin Configuration
set(PLUGIN_NAME "DistortionPro")
set(PLUGIN_VENDOR "DistortionPro")
set(PLUGIN_VERSION "1.0.0")
set(PLUGIN_DESCRIPTION "Professional distortion plugin with visual UI and presets")

# Enable plugin formats
# VST3 works on all platforms
# AU works on macOS only
set(PLUGIN_BUILD_VST3 ON)
set(PLUGIN_BUILD_AAX OFF)
set(PLUGIN_BUILD_VST OFF)  # VST2 is deprecated, use VST3
set(PLUGIN_BUILD_AU ON)

# Plugin identifiers (required for VST3)
# Use 4-character manufacturer code (registered with Steinberg)
set(MANUFACTURER_CODE "DSTP")  # 4-character manufacturer code for DistortionPro
set(PLUGIN_CODE "DPr1")  # 4-character plugin code

# Source files
set(SOURCE_FILES
    src/plugin/DistortionPro.cpp
    src/plugin/JuceWrapper.cpp
    src/plugin/DistortionPro.h
    src/dsp/DistortionProcessor.cpp
    src/dsp/DistortionProcessor.h
    src/dsp/DistortionAlgorithms.cpp
    src/dsp/DistortionAlgorithms.h
    src/dsp/Oversampler.cpp
    src/dsp/Oversampler.h
    src/presets/PresetManager.cpp
    src/presets/PresetManager.h
    src/ui/PluginEditor.cpp
    src/ui/PluginEditor.h
    src/ui/KnobComponent.cpp
    src/ui/KnobComponent.h
    src/ui/WaveformDisplay.cpp
    src/ui/WaveformDisplay.h
    src/ui/TypeSelector.cpp
    src/ui/TypeSelector.h
    src/ui/GainMeter.cpp
    src/ui/GainMeter.h
    src/ui/PresetSaveDialog.cpp
    src/ui/PresetSaveDialog.h
    src/ui/ABCompareComponent.cpp
    src/ui/ABCompareComponent.h
)

# Create JUCE plugin - Support multiple formats
# VST3, AU, and Standalone enabled
if(APPLE)
    set(PLUGIN_FORMATS VST3 AU Standalone)
elseif(WIN32)
    set(PLUGIN_FORMATS VST3 Standalone)
else()
    set(PLUGIN_FORMATS VST3 Standalone)
endif()
message(STATUS "Building with formats: ${PLUGIN_FORMATS}")

juce_add_plugin(${PLUGIN_NAME}
    COMPANY_NAME ${PLUGIN_VENDOR}
    VERSION ${PLUGIN_VERSION}
    DESCRIPTION ${PLUGIN_DESCRIPTION}
    FORMATS ${PLUGIN_FORMATS}
    NEEDS_MIDI_INPUT OFF
    NEEDS_MIDI_OUTPUT OFF
    IS_SYNTH OFF
    EDITOR_WANTS_KEYBOARD_FOCUS ON
    PLUGIN_MANUFACTURER_CODE ${MANUFACTURER_CODE}
    PLUGIN_CODE ${PLUGIN_CODE}
    MICROPHONE_PERMISSION_ENABLED OFF
)

# Required modules for standalone format
if(PLUGIN_FORMATS MATCHES "Standalone")
    target_link_libraries(${PLUGIN_NAME} PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_devices
    )
endif()

# Add VST3 specific definitions - Disable VST2 compatibility to avoid VST2 SDK dependency
if(PLUGIN_BUILD_VST3)
    target_compile_definitions(${PLUGIN_NAME} PRIVATE
        JUCE_VST3_CAN_REPLACE_VST2=0  # Disable VST2 compatibility
    )
endif()

target_sources(${PLUGIN_NAME} PRIVATE ${SOURCE_FILES})

# Set include directories
target_include_directories(${PLUGIN_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Copy presets to build directory
add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/presets
        $<TARGET_FILE_DIR:${PLUGIN_NAME}>/presets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PLUGIN_NAME}>/resources
)

# Print configuration summary
message(STATUS "=== DistortionPro Configuration ===")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "JUCE path: ${JUCE_PATH}")
message(STATUS "VST3: ${PLUGIN_BUILD_VST3}")
message(STATUS "AAX: ${PLUGIN_BUILD_AAX}")
message(STATUS "===================================")
