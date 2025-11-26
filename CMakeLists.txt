cmake_minimum_required(VERSION 3.16)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -DNDEBUG" CACHE STRING "" FORCE)


file(GLOB_RECURSE LIB_SOURCES
    ${CMAKE_SOURCE_DIR}/libraries/include/*/*.cpp
)

set(SOURCES
    src/main.cpp
    src/glad.c
    ${LIB_SOURCES}
)


add_executable(VoxelEngine ${SOURCES})


target_include_directories(VoxelEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
)


add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/shaders
            ${CMAKE_BINARY_DIR}/shaders
)
add_dependencies(VoxelEngine copy_shaders)


find_package(glfw3 3.3 REQUIRED)
target_link_libraries(VoxelEngine PRIVATE glfw)


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
