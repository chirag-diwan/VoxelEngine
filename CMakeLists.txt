cmake_minimum_required(VERSION 3.16)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect Sources
file(GLOB_RECURSE LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/libraries/include/*/*.cpp
)

set(SOURCES
    src/main.cpp
    src/glad.c
    ${LIB_SOURCES}
)

# Executable
add_executable(VoxelEngine ${SOURCES})

# Includes
target_include_directories(VoxelEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
)

# Shader copy
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/shaders
            ${CMAKE_BINARY_DIR}/shaders
)
add_dependencies(VoxelEngine copy_shaders)

# GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(VoxelEngine PRIVATE glfw)

# Linux OpenGL + X11
if(UNIX AND NOT APPLE)
    target_link_libraries(VoxelEngine PRIVATE
        GL
        X11
        Xrandr
        Xi
        pthread
        dl
    )
endif()
