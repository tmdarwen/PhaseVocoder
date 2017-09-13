#include_directories("${PROJECT_SOURCE_DIR}/Externals/yaml-cpp/include")

if(DEBUG_BUILD)
	add_definitions(-DDEBUG_BUILD)
endif(DEBUG_BUILD)

if(VERSION_NUMBER)
	add_definitions(-DVERSION_NUMBER=${VERSION_NUMBER})
endif(VERSION_NUMBER)

if(BUILD_NUMBER)
	add_definitions(-DBUILD_NUMBER=${BUILD_NUMBER})
endif(BUILD_NUMBER)

file(GLOB source_files [^.]*.h [^.]*.cpp)
include_directories("${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/yamlcpp-src/include")
add_executable(PhaseVocoder ${source_files})
target_link_libraries(PhaseVocoder AudioData Signal Utilities WaveFile yaml-cpp)
set_target_properties(PhaseVocoder PROPERTIES FOLDER Apps)

add_subdirectory(UT)


