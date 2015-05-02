project(6502)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread")
aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SRC_LIST})

#add_executable(6502 txtclient.cpp)

#add_executable(6502ncurses ncursesclient.cpp)
#target_link_libraries(6502ncurses ncurses)

add_executable(6502sdl sdlclient.cpp sdl6502.cpp 6502.cpp)
target_link_libraries(6502sdl SDL2)

# for msys2:
# g++ 6502.cpp sdl6502.cpp sdlclient.cpp -o 6502sdl.exe --std=c++11 -lmingw32 -lSDL2main -lSDL2
