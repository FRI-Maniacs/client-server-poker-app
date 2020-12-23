cmake_minimum_required(VERSION 3.7)
project(poker_app)

set(CMAKE_CXX_STANDARD 11)

add_executable(poker_app main.cpp server.cpp client.cpp)

target_link_libraries(poker_app pthread)