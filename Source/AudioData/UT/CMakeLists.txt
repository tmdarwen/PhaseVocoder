#set(project_name AudioData-UT)
include_directories("${PROJECT_SOURCE_DIR}/External/googletest/googletest/include")
file(GLOB source_files [^.]*.h [^.]*.cpp "../Source/[^.]*.h" "../Source/[^.]*.cpp")
add_executable(AudioData-UT ${source_files})
target_link_libraries(AudioData-UT Utilities gtest)
add_custom_command(TARGET AudioData-UT POST_BUILD COMMAND AudioData-UT --output-on-failure)

set_target_properties(AudioData-UT PROPERTIES FOLDER Libs)

