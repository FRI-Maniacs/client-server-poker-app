//
// Created by Spravca on 6. 1. 2021.
//

#include <cstring>
#include <cstdio>
#include "../headers/Command.h"

void Command::commitAction(const char *command, PokerTable *table, char* msg) {
    msg[0] = '\0';
    if (strcmp(command, CMD_Q) == 0) {
        sprintf(msg, "%s", command);
    }
    else if (strcmp(command, CMD_START) == 0) {
        table->startGame(msg);
    }
    else if (strcmp(command, CMD_FINISH) == 0) {
        if (table->isResumed()) {
            table->finishGame();
            table->chooseWinner(msg);
        }
        else sprintf(msg, "Žiadna hra nebeží");
    }
    else if (strcmp(command, CMD_SHOW_CARDS) == 0) {
        auto* p = table->getCurrentPlayer();
        if (p != nullptr) sprintf(msg, "%s", p->viewCards());
    }
    else if (strcmp(command, CMD_SHOW_TABLE) != 0) {
        sprintf(msg, "%s", table->toString());
    }
    else if (strcmp(command, CMD_CHECK) == 0) {
        int valid = table->makeMove(CHECK, msg);
        if (valid) table -> nextPlayer();
    }
    else if (strcmp(command, CMD_CALL) == 0) {
        int valid = table->makeMove(CALL, msg);
        if (valid) table -> nextPlayer();
    }
    else if (strcmp(command, CMD_RAISE) == 0) {
        int valid = table->makeMove(RAISE, msg);
        if (valid) table -> nextPlayer();
    }
    else if (strcmp(command, CMD_ALL_IN) == 0) {
        int valid = table->makeMove(ALL_IN, msg);
        if (valid) table -> nextPlayer();
    }
    else if (strcmp(command, CMD_FOLD) == 0) {
        int valid = table->makeMove(FOLD, msg);
        if (valid) table -> nextPlayer();
    }
    else if (strcmp(command, CMD_HELP) == 0) {
        // napoveda
    }
}
