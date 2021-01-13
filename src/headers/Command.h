//
// Created by Spravca on 6. 1. 2021.
//

#ifndef POKER_APP_COMMAND_H
#define POKER_APP_COMMAND_H

#define CMD_CHECK "check"
#define CMD_CALL "call"
#define CMD_RAISE "raise"
#define CMD_ALL_IN "allin"
#define CMD_FOLD "fold"
#define CMD_SHOW_TABLE "table"
#define CMD_SHOW_CARDS "cards"
#define CMD_HELP "help"
#define CMD_Q "q"

#include "PokerTable.h"

class Command {
public:
    static bool commitAction(const char* command, PokerTable* table, int socket, char *msg);

    static const char *help(const char *cmd, const char *desc);
};


#endif //POKER_APP_COMMAND_H
