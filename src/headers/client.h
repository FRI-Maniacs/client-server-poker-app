#ifndef POKER_APP_CLIENT_H
#define POKER_APP_CLIENT_H

int client(int argc, char* argv[]);
void catch_ctrl_c(int signal);
int eraseText(int cnt);
void recv_message(int client_socket);
void send_message(int client_socket);
#endif
