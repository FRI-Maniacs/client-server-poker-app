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
#include "./headers/server.h"

#define MAX_LEN 200
#define SOCKET_ERROR (-1)
#define SERVER_BACKLOG 10

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

struct terminal
{
    int id;
    std::string name;
    int socket;
    std::thread th;
};

std::vector<terminal> clients;
std::mutex clients_mtx, cout_mtx;




int server(int argc, char *argv[])
{

    int server_socket, client_socket, addr_size;
    SA_IN server_addr, client_addr;

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

    int client_id = 0; //number of connected clients
    std::cout << "Waiting for connections...\n" << std::endl;


    while (client_id < 2) {
        addr_size = sizeof(SA_IN);
        check((client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size)),
              "Accept failed.");

        std::cout << "Connected!\n" << std::endl;

        client_id++;
        std::lock_guard<std::mutex> guard(clients_mtx);
        std::thread th_client(handle_client, client_socket, client_id);
        clients.push_back({client_id, std::string("Anonymous"), client_socket, std::move(th_client)});
        clients_mtx.unlock();
    }




    for (auto& client : clients)
    {
        if(client.th.joinable())
            client.th.join();
    }

    close(server_socket);
    std::cout << "Server closed..." << std::endl;
    return 0;
}


// Set name of client
void set_name(int id, char name[])
{
    for(auto& client : clients)
    {
        if(client.id == id)
        {
            client.name = std::string(name);
        }
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

void broadcast_message(const std::string& message, int sender_id)
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
        msg = name + ": " + " checked...";
    } else if (cmd.command == "fold") {
        msg = name + ": " + " folded...";
    } else if (cmd.command == "allin") {
        msg = name + ": " + " get all-in...";
    } else if (cmd.command == "raise") {
        msg = name + ": " + " raised " + cmd.amount;
    } else {
        return;
    }
    
    for(auto& client : clients)
    {
        if(client.id != sender_id)
        {
            send(client.socket, msg.c_str(), msg.size(), 0);
        }
    }
}

void end_connection(int id)
{
    for (int i = 0; i < clients.size(); i++)
    {
        if(clients[i].id == id)
        {
            std::lock_guard<std::mutex> guard(clients_mtx);
            clients[i].th.detach();
            clients.erase(clients.begin() + i);
            close(clients[i].socket);
            break;
        }
    }
}

int check(int exp, const char* msg) {
    if (exp == SOCKET_ERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}

void handle_client(int client_socket, int id) {

    char name[MAX_LEN], client_msg[MAX_LEN];
    recv(client_socket, name, sizeof(name),0);
    set_name(id, name);
    std::cout << name << std::endl;
    std::string welcome_message = std::string(name) + " has joined";
    broadcast_message(welcome_message, id);

    while(true)
    {
        int bytes_received = recv(client_socket, client_msg, sizeof(client_msg),0);
        check(bytes_received, "Read Failed!");

        if(std::string(client_msg) == "q")
        {
            // Display leaving message
            std::string message = std::string(name) + std::string(" has left");
            broadcast_message(message,id);
            end_connection(id);
            return;
        }
        broadcast_message(std::string(client_msg), id);
    }
}

