//
// Created by Spravca on 6. 1. 2021.
//

#ifndef POKER_APP_COMMAND_H
#define POKER_APP_COMMAND_H

#define CMD_Q "q"
#define CMD_HELP "help"
#define CMD_CHECK "check"
#define CMD_CALL "call"
#define CMD_RAISE "raise"
#define CMD_ALL_IN "all_in"
#define CMD_FOLD "fold"
#define CMD_SHOW_CARDS "show_cards"
#define CMD_SHOW_TABLE "show_table"

#define CMD_START "start"
#define CMD_FINISH "finish"

#include "PokerTable.h"

class Command {
public:
    static void commitAction(const char* command, PokerTable* table, int p_id, char *msg);
};


#endif //POKER_APP_COMMAND_H
