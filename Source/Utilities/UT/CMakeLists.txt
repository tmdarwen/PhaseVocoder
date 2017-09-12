include_directories("${PROJECT_SOURCE_DIR}/Externals/googletest/googletest/include")
file(GLOB source_files [^.]*.h [^.]*.cpp)
add_executable(Utilities-UT ${source_files})
target_link_libraries(Utilities-UT gtest gtest_main Utilities)
file(GLOB TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestFiles/*)
file(COPY ${TEST_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET Utilities-UT POST_BUILD COMMAND Utilities-UT --output-on-failure)

set_target_properties(Utilities-UT PROPERTIES FOLDER Libs)

