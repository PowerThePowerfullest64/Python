cmake_minimum_required(VERSION 3.24)
project(UnknownGame LANGUAGES CXX)

add_subdirectory(utils)

add_executable(game main.cpp)
target_link_libraries(game PRIVATE utils)

add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:game> $<TARGET_FILE_DIR:game>
    COMMAND_EXPAND_LISTS)