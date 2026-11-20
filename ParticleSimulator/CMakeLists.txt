cmake_minimum_required(VERSION 3.24)
project(UnknownGame LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CUDA_DIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v13.5")

add_subdirectory(utils)

file(GLOB SOURCES "src/*.cpp")
add_executable(UnknownGame ${SOURCES})

#target_compile_options(UnknownGame PRIVATE -O3)
target_include_directories(UnknownGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${CUDA_DIR}/include"
)

target_link_libraries(UnknownGame PRIVATE
    utils
    "${CUDA_DIR}/lib/x64/cuda.lib"
    "${CUDA_DIR}/lib/x64/nvrtc.lib"
)

target_link_options(UnknownGame PRIVATE "-Wl,--stack,16777216") # MAKE STACK 16 MB

file(GLOB NVRTC_DLLS
    "${CUDA_DIR}/bin/*nvrtc*.dll"
    "${CUDA_DIR}/bin/x64/*nvrtc*.dll"
)

add_custom_command(TARGET UnknownGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:UnknownGame> ${NVRTC_DLLS} $<TARGET_FILE_DIR:UnknownGame>
    COMMAND_EXPAND_LISTS)