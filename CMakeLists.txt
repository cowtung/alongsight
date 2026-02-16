cmake_minimum_required(VERSION 3.15)

project(AlongSight VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch JUCE framework
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.9
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

# AlongSight application
juce_add_gui_app(AlongSight
    PRODUCT_NAME "AlongSight"
    COMPANY_NAME "AlongSight"
    BUNDLE_ID "com.alongsight.app"
    VERSION "1.0.0"
)

# Source files
target_sources(AlongSight PRIVATE
    Source/Main.cpp
    Source/MainComponent.cpp
    Source/AudioComponent.cpp
    Source/StaffDisplay.cpp
    Source/AudioMeter.cpp
)

# Compile definitions
target_compile_definitions(AlongSight PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:AlongSight,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:AlongSight,JUCE_VERSION>"
    DONT_SET_USING_JUCE_NAMESPACE=1
)

# Link JUCE modules
target_link_libraries(AlongSight PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# Binary data for resources (if needed later)
# juce_add_binary_data(AlongSightData SOURCES Resources/Images/icon.png)

# Copy resources to build directory
add_custom_command(TARGET AlongSight POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:AlongSight>/Resources
)
