cmake_minimum_required(VERSION 3.24)
project(utils LANGUAGES CXX)

set(EXT ${CMAKE_CURRENT_SOURCE_DIR}/externals)

# ----- SDL3 -----
set(SDL3_DIR ${EXT}/SDL3/x86_64-w64-mingw32/lib/cmake/SDL3)
find_package(SDL3 REQUIRED CONFIG)

# ----- ImGui -----
set(IMGUI_DIR ${EXT}/imgui)
add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC SDL3::SDL3)

# ----- utils -----
add_library(utils INTERFACE)
target_compile_features(utils INTERFACE cxx_std_23)
target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(utils INTERFACE SDL3::SDL3 imgui)