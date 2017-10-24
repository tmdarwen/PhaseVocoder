include_directories("${PROJECT_SOURCE_DIR}")
file(GLOB source_files [^.]*.h [^.]*.cpp "Source/[^.]*.h" "Source/[^.]*.cpp")
add_library(WaveFile ${source_files})

target_link_libraries(WaveFile AudioData Signal Utilities)

include(${PROJECT_SOURCE_DIR}/CMakeSupport/CMakeLists.CompilerSettings.txt)

set_target_properties(WaveFile PROPERTIES FOLDER Libs)

add_subdirectory(UT)
