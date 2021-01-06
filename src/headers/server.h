#ifndef POKERV2_SERVER_H
#define POKERV2_SERVER_H

typedef struct cmd_pair {
    std::string command;
    std::string amount;
} CMD_PAIR;

int server(int argc, char* argv[]);
int check(int exp, const char* msg);
void set_name(int id, const std::string& name);
int strToIntConverter(const std::string& str);
CMD_PAIR commandHandler(const std::string& str);
void broadcast_command(const std::string& message, int sender_id);
void broadcast_message(const std::string& message, int sender_id);
void broadcast_message(const std::string& message);
void handle_client(int client_socket, int id);

#endif //POKERV2_SERVER_H
