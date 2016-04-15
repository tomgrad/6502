project(6502)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

aux_source_directory(. SRC_LIST)

add_executable(6502 src/txtclient.cpp src/6502.cpp)

add_executable(6502ncurses src/ncursesclient.cpp src/6502.cpp)
target_link_libraries(6502ncurses ncurses)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

add_executable(6502sdl src/sdlclient.cpp src/sdl6502.cpp src/6502.cpp)
target_link_libraries(6502sdl ${SDL2_LIBRARIES})
