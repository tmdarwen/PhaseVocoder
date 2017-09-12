include_directories("${PROJECT_SOURCE_DIR}/Externals/googletest/googletest/include")
file(GLOB source_files [^.]*.h [^.]*.cpp)
add_executable(Signal-UT ${source_files})
target_link_libraries(Signal-UT gtest AudioData Signal Utilities WaveFile)

file(GLOB WAV_TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestAudio/*.wav)
file(COPY ${WAV_TEST_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB WAV_TEST_RESULTS ${CMAKE_CURRENT_SOURCE_DIR}/TestAudioResults/*.wav)
file(COPY ${WAV_TEST_RESULTS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET Signal-UT POST_BUILD COMMAND Signal-UT --output-on-failure)

set_target_properties(Signal-UT PROPERTIES FOLDER Libs)

