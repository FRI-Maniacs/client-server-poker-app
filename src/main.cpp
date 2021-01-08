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

    auto* table = new PokerTable();

    int p1 = table->connectPlayer("Šimon");
    int p2 = table->connectPlayer("Paľo");
    int p3 = table->connectPlayer("Peter");
    int p4 = table->connectPlayer("Ivan");
    int p5 = table->connectPlayer("Radka");
    int p6 = table->connectPlayer("Ľudka");
    printf("%d %d %d %d %d %d\n%s\n", p1, p2, p3, p4, p5, p6, table->toString());

    char *msg = new char[512];
    char *cmd = new char[30];

    Command::commitAction("start", table, 0, msg);

    for (int i = 0; i < 3; i++)
        Command::commitAction("call", table, i, msg);
    for (int i = 3; i < 6; i++)
        Command::commitAction("check", table, i, msg);
    for (int i = 0; i < 3; i++) {
        Command::commitAction("raise", table, 0, msg);
        for (int j = 1; j < 6; j++)
            Command::commitAction("call", table, j, msg);
    }

    do {
        cmd[0] = '\0';
        scanf("%s", cmd);
        int player = -1;
        auto* p = table->getCurrentPlayer();
        if (p != nullptr) {
            player = p->getId();
            printf("%d", player);
        }
        Command::commitAction(cmd, table, player, msg);
        printf("%s\n", msg);
    } while (strcmp(cmd, CMD_Q) != 0);
    return 0;
}
