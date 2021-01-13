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
#include <mutex>
#include "./headers/server.h"
#include "./headers/Command.h"

#define SOCKET_ERROR (-1)
#define SERVER_BACKLOG 10

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

int die = 0; //ak 1 skonci program
int admin = 0; //socket admina
int max_players = MAX_PLAYERS;
int client_sockets[MAX_PLAYERS] {-1, -1, -1, -1, -1, -1};
bool starting = true;
std::mutex command_mutex;

int server(int argc, char *argv[]) {
    int server_socket;
    SA_IN server_addr;
    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to create socket!");
    //inicialize address struct
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[2]));
    check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)), "Bind Failed!");
    check(listen(server_socket, SERVER_BACKLOG), "Listen Failed!");
    std::cout << "Waiting for connections..." << std::endl;
    auto* table = new PokerTable();
    handle_connection(server_socket, table);
    broadcast("q: Hra skončila!");
    std::cout << "Server closed..." << std::endl;
    close(server_socket);
    return 0;
}

void handle_connection(int server_socket, PokerTable* table) {
    int client_socket, addr_size;
    SA_IN client_addr;
    auto thread_pool = new std::thread[MAX_PLAYERS];
    int thread_index = 0;

    for(;;) {
        if (thread_index == max_players) break;
        addr_size = sizeof(SA_IN);
        check((client_socket = accept(server_socket, (SA *) &client_addr, (socklen_t *) &addr_size)),"Accept failed.");

        bool success = false;
        for (int &client_sock : client_sockets) {
            if (client_sock == -1) {
                client_sock = client_socket;
                success = true;
                if (thread_index == 0) admin = client_sock;
                send(client_sock, thread_index == 0
                ? "Enter your name and number of players: "
                : "Enter your name: ", MAX_LEN, 0);
                std::cout << "Connected!" << std::endl;
                break;
            }
        }

        if (success) {
            thread_pool[thread_index] = std::thread(handle_client, client_socket, table);
            thread_index++;
        } else {
            send(client_socket, "q: ", 3, 0);
        }
    }

        while (table->getPlayersCount() < max_players) continue;
    char* str = new char[100];
    broadcast("Hra sa uspesne spustila");
    std::cout << "END CONNECTION PHASE" << std::endl;
    std::cout << "Players count: " << table->getPlayersCount() << std::endl;

    //startgame
    bool started = table->startGame(str);
    if (started) broadcast(table->getCurrentPlayer()->getName());

    //join threads
    for(int i = 0; i < MAX_PLAYERS; i++)
        if (thread_pool[i].joinable())
            thread_pool[i].join();

    delete[] thread_pool;
    for (int& client_sock : client_sockets) close(client_sock);
}


int check(int exp, const char* msg) {
    if (exp == SOCKET_ERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
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


void handle_client(int client_socket, PokerTable* table) {
    char name[MAX_LEN];

    recv(client_socket, name, MAX_LEN, 0);
    //set_name(id, name);
    if (admin == client_socket) {
        CMD_PAIR cmd = commandHandler(std::string(name));
        const char* changed = cmd.command.c_str();
        sprintf(name, "%s", changed);
        table->connectPlayer(name, client_socket);
        max_players = strToIntConverter(cmd.amount);
    }
    else table->connectPlayer(name, client_socket);

    std::cout << table->activePlayersToString() << std::endl;
    std::cout << name << ": connected... IS ADMIN " << (admin == client_socket) << std::endl;

    char *resp_output = new char[MAX_LEN];
    char *client_command = new char[MAX_LEN];

    // napoveda pre hraca
    command_mutex.lock();
    Command::commitAction("help", table, client_socket, resp_output);
    command_mutex.unlock();

    send(client_socket, resp_output, MAX_LEN, 0);
    bzero(resp_output, MAX_LEN);

    while (die == 0) {
        int received = recv(client_socket, client_command, MAX_LEN, 0);
        if (received > 0) {
            command_mutex.lock();
            bool result = Command::commitAction(client_command, table, client_socket, resp_output);
            command_mutex.unlock();

            if (result && strcmp(client_command, CMD_Q) == 0) {
                for (int & i : client_sockets)
                    if (i == client_socket) i = -1;
                    else if (i != -1) send(i, resp_output, MAX_LEN, 0);
                return;
            }

            //poslat vsetkym
            if (result) {
                char *out = new char[MAX_LEN];
                sprintf(out, "%s: %s", name, resp_output);
                broadcast(out);
                delete[] out;
            }
            //poslat spat
            else send(client_socket, resp_output, MAX_LEN, 0);
            bzero(resp_output, MAX_LEN);
            bzero(client_command, MAX_LEN);
            if (!table->isResumed() && !starting) die = 1;
        }
    }
    delete[] client_command;
    delete[] resp_output;
}

void broadcast(const char* msg){
    if (msg != nullptr && strlen(msg) > 0) {
        for (int &client_sock : client_sockets)
            if (client_sock != -1)
                send(client_sock, msg, MAX_LEN, 0);
        printf("%s\n", msg);
    }
}