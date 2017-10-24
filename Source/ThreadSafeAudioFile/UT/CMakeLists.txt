include_directories("${PROJECT_SOURCE_DIR}/Externals/googletest/googletest/include")
file(GLOB source_files [^.]*.h [^.]*.cpp)
add_executable(ThreadSafeAudioFile-UT ${source_files})
target_link_libraries(ThreadSafeAudioFile-UT gtest gtest_main AudioData ThreadSafeAudioFile Utilities WaveFile)
include(${PROJECT_SOURCE_DIR}/CMakeSupport/CMakeLists.CompilerSettings.txt)
add_custom_command(TARGET ThreadSafeAudioFile-UT POST_BUILD COMMAND ThreadSafeAudioFile-UT --output-on-failure)
set_target_properties(ThreadSafeAudioFile-UT PROPERTIES FOLDER Libs)

