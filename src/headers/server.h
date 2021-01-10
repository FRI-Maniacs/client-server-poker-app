#ifndef POKERV2_SERVER_H
#define POKERV2_SERVER_H

#include "PokerTable.h"
#include <mutex>
#define MAX_LEN 512

typedef struct cmd_pair {
    std::string command;
    std::string amount;
} CMD_PAIR;

typedef struct broadcast_buffer{
    std::mutex* mtx;
    char** msg;
    int count;
    int start;
} BROADCAST_BUFFER;

void handle_connection(int server_sock, PokerTable* table);
void handle_multi_clients(PokerTable* table);
void handle_receive_msg(PokerTable* table);




int server(int argc, char* argv[]);
int check(int exp, const char* msg);
void set_name(int id, const std::string& name);
int strToIntConverter(const std::string& str);
CMD_PAIR commandHandler(const std::string& str);
void broadcast_command(const std::string& message, int sender_id);
void broadcast_message(const std::string& message, int sender_id);
void broadcast_message(const std::string& message, PokerTable* table);
void broadcast_message(const char* message, PokerTable* table);
void handle_client(int client_socket, PokerTable* table);

#endif //POKERV2_SERVER_H