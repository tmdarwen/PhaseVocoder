include_directories("${PROJECT_SOURCE_DIR}")
file(GLOB source_files [^.]*.h [^.]*.cpp "Source/[^.]*.h" "Source/[^.]*.cpp")
add_library(Utilities ${source_files})

add_subdirectory(UT)

set_target_properties(Utilities PROPERTIES FOLDER Libs)

