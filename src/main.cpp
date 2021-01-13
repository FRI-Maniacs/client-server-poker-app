#include <iostream>
#include <cstring>
#include "headers/PokerTable.h"
#include "headers/server.h"
#include "headers/client.h"

int main(int argc, char *argv[]) {
    srand(time(nullptr));
    int type = strcmp(argv[1], "server") == 0 ? 1 : strcmp(argv[1], "client") == 0 ? 2 : 0;
    printf("%s\n", type == 0 ? "nothing" : argv[1]);
    if (type == 1) server(argc, argv);
    if (type == 2) client(argc, argv);
    return 0;
}