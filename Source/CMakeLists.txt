cmake_minimum_required(VERSION 3.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(PhaseVocoder)

if(DEBUG_BUILD)
	add_definitions(-DDEBUG_BUILD)
endif(DEBUG_BUILD)

if(DEBUG_TO_LOG_FILE)
	add_definitions(-DDEBUG_BUILD)
	add_definitions(-DDEBUG_TO_LOG_FILE)
endif(DEBUG_TO_LOG_FILE)

if(MSVC)
	message("Building for Window...")
	add_definitions(-DTARGET_WINDOWS)
	set(CMAKE_CXX_FLAGS "/Zi /EHsc")    # Create PDB file
	set(CMAKE_CXX_FLAGS_RELEASE "/MT")
	set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
elseif(APPLE)
	message("Building for MacOS...")
	add_definitions(-DTARGET_MAC)
	set(CMAKE_CXX_FLAGS "-std=c++14")
elseif(UNIX)
	message("Building for Linux...")
	add_definitions(-DTARGET_LINUX)
	set(CMAKE_CXX_FLAGS "-std=c++14 -g")
endif(MSVC)

# "Externals" consists of GoogleTest and YamlCpp
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeSupport/CMakeLists.Externals.txt)
include_externals()

add_subdirectory(Application)
add_subdirectory(AudioData)
add_subdirectory(Signal)
add_subdirectory(ThreadSafeAudioFile)
add_subdirectory(Utilities)
add_subdirectory(WaveFile)
