include_directories("${PROJECT_SOURCE_DIR}/External/googletest/googletest/include")
file(GLOB source_files [^.]*.h [^.]*.cpp)
add_executable(WaveFile-UT ${source_files})
target_link_libraries(WaveFile-UT gtest WaveFile Signal)
file(GLOB WAV_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.wav)
file(COPY ${WAV_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET WaveFile-UT POST_BUILD COMMAND WaveFile-UT --output-on-failure)

set_target_properties(WaveFile-UT PROPERTIES FOLDER Libs)

