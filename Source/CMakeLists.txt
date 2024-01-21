add_subdirectory(AudioBackend)

target_sources("${PROJECT_NAME}"
    PRIVATE
        BackgroundImage.h
        BackgroundImage.cpp
        PluginEditor.h
        PluginEditor.cpp
        PluginProcessor.h
        PluginProcessor.cpp)