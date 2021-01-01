#ifndef POKER_APP_SERVER_H
#define POKER_APP_SERVER_H

int server(int argc, char* argv[]);
void* handle_connection(void* client_socket);
int check(int exp, const char* msg);
#endif
