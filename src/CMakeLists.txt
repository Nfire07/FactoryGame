cmake_minimum_required(VERSION 4.1)
project(game CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(HEADER_DIRS
        headers
        headers/core
)

set(SOURCES
        implementations/biome.cpp
        implementations/generation.cpp
        implementations/grid.cpp
        implementations/guistyleimpl.cpp
        implementations/ore.cpp
        main.cpp
)

add_executable(FactoryGame ${SOURCES})
target_include_directories(FactoryGame PRIVATE ${HEADER_DIRS})

target_link_libraries(FactoryGame PRIVATE
        raylib
        opengl32
        gdi32
        winmm
        shell32
        user32
)