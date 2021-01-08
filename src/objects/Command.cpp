//
// Created by Spravca on 6. 1. 2021.
//

#include <cstring>
#include <cstdio>
#include "../headers/Command.h"

void Command::commitAction(const char *command, PokerTable *table, int p_id, char *msg) {
    msg[0] = '\0';
    if (strcmp(command, CMD_Q) == 0) {
        sprintf(msg, "Game over");
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
        Player *p = table->getCurrentPlayer();
        if (p != nullptr && p->getId() == p_id) sprintf(msg, "%s", p->viewCards());
        else if (p != nullptr) sprintf(msg, "Nie sú to vaše karty!");
        else sprintf(msg, "Hráč nie je k dispozícii!");
    }
    else if (strcmp(command, CMD_SHOW_TABLE) == 0) {
        sprintf(msg, "%s", table->toString());
    }
    else if (strcmp(command, CMD_HELP) == 0) {
        // napoveda
    }
    else {
        int check = strcmp(command, CMD_CHECK) == 0;
        int call = strcmp(command, CMD_CALL) == 0;
        int raise = strcmp(command, CMD_RAISE) == 0;
        int allIn = strcmp(command, CMD_ALL_IN) == 0;
        int fold = strcmp(command, CMD_FOLD) == 0;
        if (check || call || raise || allIn || fold) {
            Move m = check ? CHECK : call ? CALL : raise ? RAISE : allIn ? ALL_IN : FOLD;
            if (table->makeMove(m, msg)) {
                table->nextPlayer();
                if (table->isFinished()) {
                    char* more = new char[256];
                    table->chooseWinner(more);
                    printf("%s", more);
                    strncat(msg, "\n\n", 2);
                    strncat(msg, more, strlen(more));
                }
            }
        }
    }
}
