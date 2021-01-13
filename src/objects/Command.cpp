//
// Created by Spravca on 6. 1. 2021.
//

#include <cstring>
#include <cstdio>
#include "../headers/Command.h"

const char *Command::help(const char *cmd, const char *desc) {
    int len1 = (int)strlen(cmd) + 2;
    if (len1 < 7) len1 = 7;
    char *result = new char[((len1 < 7) ? 7 : len1) + (int)strlen(desc)];
    sprintf(result, "%s: %s", cmd, desc);
    return result;
}

bool Command::commitAction(const char *command, PokerTable *table, int socket, char *msg) {
    msg[0] = '\0';
    if (strcmp(command, CMD_Q) == 0) {
        Player *p = table->getPlayerBySocket(socket);
        const char *name = p != nullptr ? p->getName() : "";
        if (table->isOnTurn(socket)) table->nextPlayer();
        bool disconnected = table->disconnectPlayer(socket);
        if (disconnected) sprintf(msg, "Hráč %s opustil hru", name);
        else sprintf(msg, "Hráč tu už dávno nie je!");
        return disconnected;
    }
    else if (strcmp(command, CMD_SHOW_CARDS) == 0) sprintf(msg, "%s", table->viewCards(table->getSeat(socket)));
    else if (strcmp(command, CMD_SHOW_TABLE) == 0) sprintf(msg, "%s", table->toString());
    else if (strcmp(command, CMD_HELP) == 0) {
        const char *check = Command::help(CMD_CHECK, "Obísť stávku!");
        const char *call = Command::help(CMD_CALL, "Dorovnať stávku!");
        const char *raise = Command::help(CMD_RAISE, "Zvýšiť stávku!");
        const char *allin = Command::help(CMD_ALL_IN, "Staviť všetko!");
        const char *fold = Command::help(CMD_FOLD, "Zložiť karty!");
        const char *tbl = Command::help(CMD_SHOW_TABLE, "Zobraziť detaily!");
        const char *cards = Command::help(CMD_SHOW_CARDS, "Zobraziť karty!");
        const char *help = Command::help(CMD_HELP, "Zobraziť nápovedu!");
        const char *q = Command::help(CMD_Q, "Ukončiť hru!");
        char *moves = new char[strlen(check) + strlen(call) + strlen(raise) + strlen(allin) + strlen(fold) + 8];
        char *other = new char[strlen(tbl) + strlen(cards) + strlen(help) + strlen(q) + 6];
        sprintf(moves, "%s\n\t%s\n\t%s\n\t%s\n\t%s", check, call, raise, allin, fold);
        sprintf(other, "%s\n\t%s\n\t%s\n\t%s", tbl, cards, help, q);
        sprintf(msg, "Ťahy\n\t%s\nOstatné\n\t%s", moves, other);
    }
    else {
        int check = strcmp(command, CMD_CHECK) == 0;
        int fold = strcmp(command, CMD_FOLD) == 0;
        int call = strcmp(command, CMD_CALL) == 0;
        int raise = strcmp(command, CMD_RAISE) == 0;
        int allIn = strcmp(command, CMD_ALL_IN) == 0;
        if (check || call || raise || allIn || fold) {
            if (!table->isOnTurn(socket)) {
                sprintf(msg, "Nie ste na ťahu!");
                return false;
            }
            bool mv = table->makeMove(check ? CHECK : call ? CALL : raise ? RAISE : allIn ? ALL_IN : FOLD, msg);
            if (mv) {
                table->nextPlayer();
                if (table->getActivePlayersCount() <= 1) table->finishGame();
                if (table->isFinished()) table->chooseWinner(msg);
            }
            return mv;
        }
    }
    return false;
}
