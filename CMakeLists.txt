cmake_minimum_required(VERSION 3.7)
project(poker_app)

set(CMAKE_CXX_STANDARD 11)

add_executable(poker_app src/main.cpp src/server.cpp src/client.cpp src/headers/server.h src/headers/client.h)

target_link_libraries(poker_app pthread)