cmake_minimum_required(VERSION 3.7)
project(poker_app)

set(CMAKE_CXX_STANDARD 11)

add_executable(poker_app src/main.cpp src/server.cpp src/client.cpp "src/headers/server.h" "src/headers/client.h" src/objects/Card.cpp src/headers/Card.h src/objects/PokerTable.cpp src/headers/PokerTable.h src/objects/Player.cpp src/headers/Player.h src/enums/Face.h src/enums/Suit.h src/enums/HandRanking.h src/objects/PokerHand.cpp src/headers/PokerHand.h src/enums/Moves.h src/enums/Seat.h)

target_link_libraries(poker_app pthread)