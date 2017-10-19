include_directories("${PROJECT_SOURCE_DIR}")
file(GLOB source_files [^.]*.h [^.]*.cpp "Source/[^.]*.h" "Source/[^.]*.cpp")
add_library(ThreadSafeAudioFile ${source_files})

add_subdirectory(UT)

set_target_properties(ThreadSafeAudioFile PROPERTIES FOLDER Libs)

