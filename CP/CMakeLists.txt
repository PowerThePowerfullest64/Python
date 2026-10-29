cmake_minimum_required(VERSION 3.24)
project(UnknownGame LANGUAGES CXX)

add_subdirectory(utils)

add_executable(game main.cpp)

target_include_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(game PRIVATE utils)
target_link_options(game PRIVATE "-Wl,--stack,16777216") # MAKE STACK 16 MB

add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:game> $<TARGET_FILE_DIR:game>
    COMMAND_EXPAND_LISTS)