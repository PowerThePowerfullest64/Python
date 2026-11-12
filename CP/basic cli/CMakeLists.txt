cmake_minimum_required(VERSION 3.24)
project(UnknownGame LANGUAGES CXX)

add_subdirectory(utils)

file(GLOB SOURCES "src/*.cpp")
add_executable(UnknownGame ${SOURCES})

target_include_directories(UnknownGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(UnknownGame PRIVATE utils)
target_link_options(UnknownGame PRIVATE "-Wl,--stack,16777216") # MAKE STACK 16 MB

add_custom_command(TARGET UnknownGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:UnknownGame> $<TARGET_FILE_DIR:UnknownGame>
    COMMAND_EXPAND_LISTS)