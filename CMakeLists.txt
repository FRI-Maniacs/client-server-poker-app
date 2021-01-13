cmake_minimum_required(VERSION 3.7)
project(poker_app)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        poker_app
        src/enums/Face.h
        src/enums/HandRanking.h
        src/enums/Moves.h
        src/enums/Suit.h
        src/headers/client.h
        src/headers/server.h
        src/headers/Card.h
        src/headers/Player.h
        src/headers/PokerHand.h
        src/headers/PokerTable.h
        src/headers/Command.h
        src/objects/Card.cpp
        src/objects/Player.cpp
        src/objects/PokerHand.cpp
        src/objects/PokerTable.cpp
        src/objects/Command.cpp
        src/server.cpp
        src/client.cpp
        src/main.cpp
)

target_link_libraries(poker_app pthread)