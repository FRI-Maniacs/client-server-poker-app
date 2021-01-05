#include <vector>
#include <mutex>

#ifndef POKER_APP_SERVER_H
#define POKER_APP_SERVER_H

typedef struct cmd_pair {
    std::string command;
    std::string amount;
} CMD_PAIR;

int server(int argc, char* argv[]);
void handle_client(int client_socket, int id);
void set_name(int id, char name[]);
void broadcast_message(const std::string& message, int sender_id);
void end_connection(int id);
int check(int exp, const char* msg);
CMD_PAIR commandHandler(const std::string& str);
int strToIntConverter(const std::string& str);

#endif
