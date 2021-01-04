#include <iostream>
#include <cstring>
#include "headers/PokerTable.h"

int main(int argc, char *argv[])
{
    // tento prikaz by sa mal za cely program vykonat len raz, a to po spusteni,
    srand(time(nullptr));

    /*if (std::string(argv[1]) == "server"){
        std::cout<< "SERVER" << std::endl;
        server(argc, argv);
    }
    else if (std::string(argv[1]) == "client"){
        std::cout<< "CLIENT" << std::endl;
        client(argc, argv);
    } else {
        std::cout << "NOTHING" << std::endl;
    }*/

    auto* pool = new PokerTable();
    char** players = static_cast<char**>(malloc(sizeof(char*) * 6));
    for (int i = 0; i < 6; i++) players[i] = new char[18];

    strncat(players[0], "Šimon", 18);
    strncat(players[0], "Paľo", 18);
    strncat(players[0], "Peter", 18);
    strncat(players[0], "Ivan", 18);
    strncat(players[0], "Ľudka", 18);
    strncat(players[0], "Radka", 18);

    for (int i = 0; i<6; i++) pool->connectPlayer(players[i]);
    free(players);
    players = nullptr;

    pool->startGame();

    char *msg = nullptr;
    pool->makeMove(CALL, msg);
    pool->makeMove(CALL, msg);
    pool->makeMove(CALL, msg);
    pool->makeMove(CHECK, msg);
    pool->makeMove(CHECK, msg);
    pool->makeMove(CHECK, msg);

    pool->makeMove(RAISE, msg);
    pool->makeMove(RAISE, msg);
    pool->makeMove(CALL, msg);
    pool->makeMove(CALL, msg);
    pool->makeMove(FOLD, msg);
    pool->makeMove(CALL, msg);

    pool->makeMove(CHECK, msg);
    pool->makeMove(CHECK, msg);
    pool->makeMove(RAISE, msg);
    pool->makeMove(RAISE, msg);
    pool->makeMove(CALL, msg);

    pool->makeMove(CHECK, msg);
    pool->makeMove(CHECK, msg);
    pool->makeMove(CHECK, msg);
    pool->makeMove(CHECK, msg);
    pool->makeMove(CHECK, msg);

    pool->chooseWinner();
}
