add_subdirectory(AudioBackend)

target_sources("${PROJECT_NAME}"
    PRIVATE
        PluginEditor.h
        PluginEditor.cpp
        PluginProcessor.h
        PluginProcessor.cpp)

# Add subdirectories with source files, if present
