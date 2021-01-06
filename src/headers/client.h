#ifndef POKERV2_CLIENT_H
#define POKERV2_CLIENT_H

int client(int argc, char* argv[]);
void send_message(int client_sock);
void recv_message(int client_sock);
int eraseText(int cnt);
void catch_ctrl_c(int signal);


#endif //POKERV2_CLIENT_H
