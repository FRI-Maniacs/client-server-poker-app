#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cerrno>
#include <thread>
#include <csignal>
#include "./headers/client.h"
#include <mutex>

#define MAX_LEN 200

std::thread t_send, t_recv;
bool exit_flag = false;
int client_socket;
struct sockaddr_in serv_addr;
struct hostent* server;

int client(int argc, char* argv[]) {

    if ((server = gethostbyname(argv[2])) == nullptr) {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr)); //Vynuluj struct
    serv_addr.sin_family = AF_INET; //definuje ze pouzivame internetove sockety
    bcopy(
            (char*)server->h_addr,//object ktory obsahuje IP servera
            (char*)&serv_addr.sin_addr.s_addr, //IP adresa servera kde sa chcem pripojit
            server->h_length //velkost premennej
    );
    serv_addr.sin_port = htons(atoi(argv[3])); //port na ktorom pocuva server


    if((client_socket = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket: ");
        exit(-1);
    }

    if((connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in))) == -1)
    {
        perror("connect: ");
        exit(-1);
    }

    signal(SIGINT, catch_ctrl_c);
    char name[MAX_LEN];
    std::cout<<"Enter your name and amount of players: ";
    std::cin.getline(name,MAX_LEN);
    send(client_socket, name, sizeof(name),0);

    std::thread t1(send_message, client_socket);
    std::thread t2(recv_message, client_socket);

    t_send = move(t1);
    t_recv = move(t2);

    if (t_send.joinable())
        t_send.join();
    if (t_recv.joinable())
        t_recv.join();

    return 0;
}


void catch_ctrl_c(int signal)
{
    char str[MAX_LEN] = "q";
    send(client_socket, str, sizeof(str),0);
    exit_flag=true;
    t_send.detach();
    t_recv.detach();

    close(client_socket);
    exit(signal);
}


int eraseText(int cnt)
{
    char back_space = 8;
    for(int i = 0; i < cnt; i++)
    {
        std::cout << back_space;
    }
}

// Send message to everyone
void send_message(int client_sock)
{
    while(true)
    {
        std::cout << "You: ";
        char msg[MAX_LEN];
        std::cin.getline(msg, MAX_LEN);

        send(client_sock, msg, sizeof(msg), 0);
        if (std::string(msg) == "q") {
            exit_flag = true;
            t_recv.detach();
            close(client_sock);
            return;
        }
    }
}

// Receive message
void recv_message(int client_sock)
{
    while(true)
    {
        if(exit_flag)
            return;
        char msg[MAX_LEN];

        int bytes_received = recv(client_sock, msg ,sizeof(msg),0);
        if(bytes_received <= 0) continue;

        eraseText(6);
        std::cout<< msg << std::endl;

        std::cout << "You : ";
        fflush(stdout);
        bzero(msg, MAX_LEN);
    }
}
