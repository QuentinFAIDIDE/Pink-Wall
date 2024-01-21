file(GLOB_RECURSE all_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
list(REMOVE_ITEM all_sources TestAudioBackend.cpp LoadCsv.cpp)
add_library(AudioBackend ${all_sources})

set_target_properties(AudioBackend PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_executable(TestAudioBackend TestAudioBackend.cpp LoadCsv.cpp)
target_link_libraries(TestAudioBackend AudioBackend)
add_test(NAME TestAudioBackend COMMAND TestAudioBackend)