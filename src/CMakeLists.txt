cmake_minimum_required(VERSION 4.1)
project(game C)

set(CMAKE_C_STANDARD 11)

include_directories(headers)
include_directories(headers/core)


set(SOURCES
        implementations/biome.c
        implementations/generation.c
        implementations/grid.c
        implementations/guistyleimpl.c
        implementations/ore.c
        main.c
)

add_executable(FactoryGame ${SOURCES})

target_link_libraries(FactoryGame PRIVATE
        raylib
        opengl32
        gdi32
        winmm
        shell32
        user32
)