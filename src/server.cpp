#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <condition_variable>
#include "./headers/server.h"

#define MAX_LEN 200
#define THREAD_POOL_SIZE 2
#define SOCKET_ERROR (-1)
#define SERVER_BACKLOG 10

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

struct terminal
{
    int id;
    std::string name;
    int socket;
};


std::vector<terminal> clients;
std::mutex clients_mtx;


int server(int argc, char *argv[])
{
    int server_socket, client_socket, addr_size;
    SA_IN server_addr, client_addr;
    int connected_players = 0;
    int max_players = THREAD_POOL_SIZE;
    auto thread_pool = new std::thread[THREAD_POOL_SIZE];

    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
          "Failed to create socket!");

    //inicialize address struct
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[2]));

    check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)),
          "Bind Failed!");

    check(listen(server_socket, SERVER_BACKLOG), "Listen Failed!");

    //int client_id = 0; //number of connected clients
    std::cout << "Waiting for connections...\n" << std::endl;

    for (;;) {
        if (connected_players == max_players) break;
        addr_size = sizeof(SA_IN);
        check((client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size)),
              "Accept failed.");

        std::cout << "Connected!\n" << std::endl;

        thread_pool[connected_players] = std::thread(handle_client, client_socket, connected_players);
        clients.push_back({connected_players, std::string("Anonymous"), client_socket});
        connected_players++;
    }
    broadcast_message(std::string("Wait for players to join..."));
    for(int i = 0; i < THREAD_POOL_SIZE; i++) {
        if (thread_pool[i].joinable()) thread_pool[i].join();
    }
    //gamelogic
    broadcast_message("GAME\n");
    std::string on_turn = "Its your turn\n";
    std::string off_turn = " is on turn\n";

    //stavky
    for (int fase = 0; fase < 3; fase++) {
        for (auto &client : clients) {
            send(client.socket, on_turn.c_str(), on_turn.size(), 0);
            broadcast_message(std::string(client.name + off_turn), client.id);
            char response[MAX_LEN];
            int bytes_received = 0;
            while (bytes_received == 0) {
                bytes_received = recv(client.socket, response, sizeof(response), 0);
            }
            broadcast_command(std::string(response), client.id);
            bzero(response, MAX_LEN);
        }
        switch (fase){
            case 0:
                broadcast_message(std::string("PRVE TRI KARTA\n"));
                break;
            case 1:
                broadcast_message(std::string("STVRTA KARTA\n"));
                break;
            case 2:
                broadcast_message(std::string("PIATA KARTA\n"));
                break;
            default:
                std::cout << "DEFAULT\n" << std::endl;
        }
    }
    broadcast_message(std::string("END vyhodnotenie\n"));
    broadcast_message(std::string("niekto vyhral\n"));

    for(auto& client : clients) {
        close(client.socket);
        std::cout << client.name << " disconnected" << std::endl;
    }

    close(server_socket);
    std::cout << "Server closed..." << std::endl;
    return 0;
}

int check(int exp, const char* msg) {
    if (exp == SOCKET_ERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}
void set_name(int id, const std::string& name)
{
    for(auto& client : clients)
    {
        if(client.id == id)
        {
            client.name = name;
        }
    }
}
void broadcast_message(const std::string& message){
    for(auto& client : clients) {
        send(client.socket, message.c_str(), message.size(), 0);
    }
}
int strToIntConverter(const std::string& str) {
    std::stringstream converter(str);
    int amount;
    converter >> amount;
    return amount;
}
CMD_PAIR commandHandler(const std::string& str) {

    std::string delimiter = " ";
    std::string command = str.substr(0, str.find(delimiter));
    std::string amount = str.substr(str.find(delimiter) + delimiter.length(), str.length());

    return { command, amount };
}
void broadcast_command(const std::string& message, int sender_id)
{
    std::string msg = "EMPTY";
    std::string name;

    for (auto& client : clients) {
        if (client.id == sender_id) {
            name = client.name;
        }
    }
    auto cmd = commandHandler(message);
    auto amount = strToIntConverter(cmd.amount);

    if (cmd.command == "check") {
        msg = name + ": " + " checked...\n";
    } else if (cmd.command == "fold") {
        msg = name + ": " + " folded...\n";
    } else if (cmd.command == "allin") {
        msg = name + ": " + " get all-in...\n";
    } else if (cmd.command == "raise") {
        msg = name + ": " + " raised " + cmd.amount + "\n";
    } else {
        return;
    }

    broadcast_message(msg, sender_id);
}
void broadcast_message(const std::string& message, int sender_id)
{
    for(auto& client : clients)
    {
        if(client.id != sender_id)
        {
            send(client.socket, message.c_str(), message.size(), 0);
        }
    }
}
void handle_client(int client_socket, int id) {
    char name[MAX_LEN];
    recv(client_socket, name, sizeof(name), 0);
    set_name(id, name);
    std::cout << name << " connected" << std::endl;
}