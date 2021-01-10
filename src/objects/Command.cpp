//
// Created by Spravca on 6. 1. 2021.
//

#include <cstring>
#include <cstdio>
#include "../headers/Command.h"

bool Command::commitAction(const char *command, PokerTable *table, int p_id, char *msg) {
    printf("command is %s strlen(%zu)\n", command, strlen(command));

    msg[0] = '\0';
    if (strcmp(command, CMD_Q) == 0) {
        sprintf(msg, "Game over");
        return false;
    }
    else if (strcmp(command, CMD_SHOW_CARDS) == 0) {
        Player *p = table->getPlayerAt(p_id);
        sprintf(msg, "%s", table->viewCards(p_id));

        return false;
    }
    else if (strcmp(command, CMD_SHOW_TABLE) == 0) {
        sprintf(msg, "%s", table->toString());
        return false;
    }
    else if (strcmp(command, CMD_HELP) == 0) {
        sprintf(msg, "HELP!!!");
        return false;
    }
    else {
        int check = strcmp(command, CMD_CHECK) == 0;
        int call = strcmp(command, CMD_CALL) == 0;
        int raise = strcmp(command, CMD_RAISE) == 0;
        int allIn = strcmp(command, CMD_ALL_IN) == 0;
        int fold = strcmp(command, CMD_FOLD) == 0;
        if (check || call || raise || allIn || fold) {
            if (table->getCurrentPlayer()->getId() == p_id) {
                Move m = check ? CHECK : call ? CALL : raise ? RAISE : allIn ? ALL_IN : FOLD;
                printf("%d\n", m);
                if (table->makeMove(m, msg)) {
                    if (table->getActivePlayersCount() == 1) {
                        printf("I should finish\n");
                        table->finishGame();
                        //printf("AFTER FINISHh\n");
                        table->chooseWinner(msg);
                        return true;
                    }
                    else {
                        table->nextPlayer();
                        if (table->isFinished()) {
                            table->chooseWinner(msg);
                            //char* more = new char[256];
                            //table->chooseWinner(more);
                            //strncat(msg, "\n", 2);
                            //strncat(msg, more, strlen(more));
                        }
                        return true;
                    }
                }
            }
            else {
                sprintf(msg, "Ešte nie ste na rade!");
                return false;
            }
        }
    }
    return false;
}