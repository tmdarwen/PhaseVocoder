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

#ExternalProject_Add(yaml-cpp
#    URL https://github.com/jbeder/yaml-cpp
#    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/yaml-cpp
#    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
#)

#include_directories("${PROJECT_SOURCE_DIR}")
#link_directories("${CMAKE_BINARY_DIR}/yamlcpp-build/Debug")
#file(GLOB source_files [^.]*.h [^.]*.cpp)
#add_executable(PhaseVocoder ${source_files})
#include_directories("${CMAKE_BINARY_DIR}/yamlcpp-src/include")
##add_dependencies(PhaseVocoder yaml-cpp)
#target_link_libraries(PhaseVocoder libyaml-cppmtd AudioData Signal Utilities WaveFile)
##set(YAML_CPP_DIR ${PROJECT_SOURCE_DIR}/Externals/yaml-cpp/)
##find_package(yaml-cpp REQUIRED)
##if(NOT yaml-cpp_FOUND)
##   message(FATAL_ERROR "Problem: yaml-cpp not found")
##endif(NOT yaml-cpp_FOUND)
##include_directories(${yaml-cpp_INCLUDES})
##target_link_libraries(PhaseVocoder ${yaml-cpp_LIBS} AudioData Signal Utilities WaveFile)
##target_link_libraries(PhaseVocoder Common Wave Signal Utilities)

file(GLOB source_files [^.]*.h [^.]*.cpp)
include_directories("${PROJECT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/yamlcpp-src/include")
#add_subdirectory("${CMAKE_BINARY_DIR}/yamlcpp-build/Debug")
add_executable(PhaseVocoder ${source_files})
#target_link_libraries(PhaseVocoder "${CMAKE_BINARY_DIR}/yamlcpp-build/Debug/libyaml-cppmtd.lib" AudioData Signal Utilities WaveFile yaml-cpp)
target_link_libraries(PhaseVocoder AudioData Signal Utilities WaveFile yaml-cpp)
#add_dependencies(PhaseVocoder "${CMAKE_BINARY_DIR}/yamlcpp-build/yaml-cpp")
#add_dependencies(PhaseVocoder  "${CMAKE_BINARY_DIR}/yamlcpp-download")
#add_dependencies(PhaseVocoder YAML_CPP)
set_target_properties(PhaseVocoder PROPERTIES FOLDER Apps)

add_subdirectory(UT)


