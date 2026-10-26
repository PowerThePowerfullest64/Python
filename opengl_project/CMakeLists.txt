cmake_minimum_required(VERSION 3.15)
project(OpenGLProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the packages we installed via vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(glad REQUIRED)
find_package(glm CONFIG REQUIRED)

# Define your executable name and source file
add_executable(gameengine main.cpp shader.cpp)

# Link GLFW, GLAD, and GLM to your app
target_link_libraries(gameengine PRIVATE 
    glfw
    glad::glad
    glm::glm
)