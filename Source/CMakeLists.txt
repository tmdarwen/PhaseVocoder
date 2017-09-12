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

# Get GoogleTest
configure_file(CMakeLists.GoogleTest.txt googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download")
execute_process(COMMAND "${CMAKE_COMMAND}" --build .  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
if(MSVC)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # Prevent GoogleTest from overriding options when building with MSVC
endif(MSVC)
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src" "${CMAKE_BINARY_DIR}/googletest-build")

# Get yaml-cpp
option(YAML_CPP_BUILD_TOOLS "Enable testing and parse tools" OFF)
option(MSVC_SHARED_RT "MSVC: Build with shared runtime libs (/MD)" OFF)
configure_file(CMakeLists.YamlCpp.txt yamlcpp-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/yamlcpp-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/yamlcpp-download" )
add_subdirectory("${CMAKE_BINARY_DIR}/yamlcpp-src" "${CMAKE_BINARY_DIR}/yamlcpp-build")
 
add_subdirectory(Application)
add_subdirectory(AudioData)
add_subdirectory(Signal)
add_subdirectory(Utilities)
add_subdirectory(WaveFile)
