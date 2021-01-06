#include <iostream>
#include <cstring>
#include "headers/PokerTable.h"
#include "headers/Card.h"
#include "headers/Command.h"

int main(int argc, char *argv[]) {
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
    strncat(players[1], "Paľo", 18);
    strncat(players[2], "Peter", 18);
    strncat(players[3], "Ivan", 18);
    strncat(players[4], "Ľudka", 18);
    strncat(players[5], "Radka", 18);

    for (int i = 0; i<6; i++) pool->connectPlayer(players[i]);
    free(players);
    players = nullptr;

    char *msg = new char[512];
    char *cmd = new char[30];

    do {
        cmd[0] = '\0';
        scanf("%s", cmd);
        Command::commitAction(cmd, pool, msg);
        printf("%s\n", msg);
    } while (strcmp(msg, CMD_Q) != 0);
    return 0;
}
