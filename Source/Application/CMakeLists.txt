if(DEBUG_BUILD)
	add_definitions(-DDEBUG_BUILD)
endif(DEBUG_BUILD)

if(VERSION_NUMBER)
	add_definitions(-DVERSION_NUMBER=${VERSION_NUMBER})
endif(VERSION_NUMBER)

if(BUILD_NUMBER)
	add_definitions(-DBUILD_NUMBER=${BUILD_NUMBER})
endif(BUILD_NUMBER)

include_directories("${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/audiolib-src/Source")
include_directories("${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/yamlcpp-src/include")

file(GLOB source_files [^.]*.h [^.]*.cpp)
find_package(Threads)
add_executable(PhaseVocoder ${source_files})
include(${PROJECT_SOURCE_DIR}/CMakeSupport/CMakeLists.CompilerSettings.txt)
target_link_libraries(PhaseVocoder AudioData Signal ThreadSafeAudioFile Utilities WaveFile yaml-cpp ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(PhaseVocoder PROPERTIES FOLDER Apps)

add_subdirectory(UT)


