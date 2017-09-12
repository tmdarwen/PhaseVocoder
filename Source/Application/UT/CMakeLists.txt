include_directories("${PROJECT_SOURCE_DIR}/Externals/googletest/googletest/include")
include_directories("${PROJECT_SOURCE_DIR}/Externals/yaml-cpp/include")

file(GLOB source_files [^.]*.h [^.]*.cpp 
	../PhaseVocoderMediator.h 
	../PhaseVocoderMediator.cpp 
	../PhaseVocoderSettings.h 
	../PhaseVocoderSettings.cpp 
	../CommandLineArguments.h 
	../CommandLineArguments.cpp 
	../Transients.h 
	../Transients.cpp
	../TransientConfigFile.h 
	../TransientConfigFile.cpp)

#link_directories("${CMAKE_BINARY_DIR}/yamlcpp-build/Debug")
add_executable(PhaseVocoderApp-UT ${source_files})
#target_link_libraries(PhaseVocoderApp-UT gtest libyaml-cppmtd AudioData Signal Utilities WaveFile)
target_link_libraries(PhaseVocoderApp-UT gtest yaml-cpp AudioData Signal Utilities WaveFile)

file(GLOB WAV_TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestAudio/*.wav)
file(COPY ${WAV_TEST_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB WAV_TEST_RESULTS ${CMAKE_CURRENT_SOURCE_DIR}/TestAudioResults/*.wav)
file(COPY ${WAV_TEST_RESULTS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB YAML_TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestTransientConfigFiles/*.yaml)
file(COPY ${YAML_TEST_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET PhaseVocoderApp-UT POST_BUILD COMMAND PhaseVocoderApp-UT --output-on-failure)

set_target_properties(PhaseVocoderApp-UT PROPERTIES FOLDER Apps)

