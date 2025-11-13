cmake_minimum_required(VERSION 3.10)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/glad.c
)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/libraries/include
)

# Find OpenGL and GLFW
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link all necessary libraries
target_link_libraries(${PROJECT_NAME}
    glfw
    ${OPENGL_LIBRARIES}
    dl
    pthread
)
