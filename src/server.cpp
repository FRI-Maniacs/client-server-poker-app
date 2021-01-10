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
#include <condition_variable>
#include "./headers/server.h"
#include "./headers/PokerTable.h"
#include "./headers/Command.h"

#define SOCKET_ERROR (-1)
#define SERVER_BACKLOG 10

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;




int die = 0; //ak 1 skonci program
int admin = 0; //socket admina
int max_players = MAX_PLAYERS;
int client_sockets[MAX_PLAYERS] {-1, -1, -1, -1, -1, -1};
std::mutex broadcast_mtx;

BROADCAST_BUFFER buff = {
    &broadcast_mtx,
    static_cast<char**>(malloc(sizeof(char*)*7)),
    0,
    0
};

const char* read_from_buffer() {
    //std::cout << "READING START" << std::endl;
    std::unique_lock<std::mutex> loc(*buff.mtx);
    int pos = buff.start % MAX_PLAYERS;
    char* tmp = buff.count > 0 ? buff.msg[pos] : nullptr;
    buff.msg[pos] = nullptr;
    buff.start = pos + 1;
    buff.count--;
    loc.unlock();
    //std::cout << "READING END" << std::endl;
    return tmp;
}

void write_to_buffer(char* str) {
    std::cout << "WRITING START" << std::endl;

    std::unique_lock<std::mutex> loc(*buff.mtx);
    if (buff.count < MAX_PLAYERS) {
        buff.msg[(buff.start + buff.count) % MAX_PLAYERS] = str;
        buff.count++;
    }
    loc.unlock();
    std::cout << "WRITING END" << std::endl;

}


int server(int argc, char *argv[])
{
    int server_socket;
    SA_IN server_addr;


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

    std::cout << "Waiting for connections...\n" << std::endl;
    auto* table = new PokerTable();

    handle_connection(server_socket, table);

    std::cout << "Server closed..." << std::endl;
    close(server_socket);
    return 0;
}

void handle_connection(int server_socket, PokerTable* table) {
    int client_socket, addr_size;
    SA_IN client_addr;
    auto thread_pool = new std::thread[MAX_PLAYERS];
    int thread_index = 0;
    std::thread th_handle_broadcast(handle_multi_clients, table);

    for(;;) {
        if (thread_index == max_players) break;
        //najdi index prvej -1
        //ak sa nerovna -1 => accept
        addr_size = sizeof(SA_IN);
        check((client_socket = accept(server_socket, (SA *) &client_addr, (socklen_t *) &addr_size)),
              "Accept failed.");

        bool success = false;
        for (int &client_sock : client_sockets) {
            if (client_sock == -1) {
                client_sock = client_socket;
                success = true;
                if (thread_index == 0) {
                    std::string admin_msg = "Enter your name and number of players: ";
                    admin = client_sock;
                    send(client_sock, admin_msg.c_str(), admin_msg.size(), 0);
                } else {
                    std::string pleb_msg = "Enter your name: ";
                    send(client_sock, pleb_msg.c_str(), pleb_msg.size(), 0);
                }
                std::cout << "Connected!\n" << std::endl;
                break;
            }
        }

        if (success) {
            thread_pool[thread_index] = std::thread(handle_client, client_socket, table);
            thread_index++;
        } else {
            std::string exit_msg = "q: ";
            send(client_socket, exit_msg.c_str(), exit_msg.size(), 0);
        }
    }

    while (table->getPlayersCount() < max_players) continue;
    char* str = new char[100];
    sprintf(str, "%s", "Hra sa uspesne spustila");
    write_to_buffer(str);

    std::cout << "END CONNECTION PHASE" << std::endl;
    std::cout << "Players count: " << table->getPlayersCount() << std::endl;

    //startgame

    if (table->startGame(str)){
        std::cout << "GAME STARTED "<< str << std::endl;
        char *turn = new char[20];
        sprintf(turn, "%s", table->getCurrentPlayer()->getName());
        write_to_buffer(turn);
    } else {
        std::cout << "NOT STARTED"<< str << std::endl;

    }

    //join threads
    for(int i = 0; i < MAX_PLAYERS; i++) {
        if (thread_pool[i].joinable()) thread_pool[i].join();
    }
    if (th_handle_broadcast.joinable()) th_handle_broadcast.join();
    delete[] thread_pool;
    for (int& client_sock : client_sockets) {
        close(client_sock);
    }
}


int check(int exp, const char* msg) {
    if (exp == SOCKET_ERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}


/*void set_name(int id, const std::string& name)
{
    for(auto& client : clients)
    {
        if(client.id == id)
        {
            client.name = name;
        }
    }
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
 */
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
        table->connectPlayer(cmd.command.c_str(), client_socket);
        max_players = strToIntConverter(cmd.amount);
    } else {
        table->connectPlayer(name, client_socket);
    }
    std::cout << table->activePlayersToString() <<std::endl;
    std::cout << name << " connected... IS ADMIN " << (admin == client_socket) << std::endl;

    const int ID = table->getIdBySocket(client_socket);
    const char* NAME = name;

    char *resp_output = new char[MAX_LEN];
    char *client_command = new char[MAX_LEN];

    // napoveda pre hraca
    Command::commitAction("help", table, ID, resp_output);
    send(client_socket, resp_output, MAX_LEN, 0);
    bzero(resp_output, MAX_LEN);

    while (die == 0) {
        if (recv(client_socket, client_command, MAX_LEN, 0) > 0) {
            std::cout << "CMD: " << client_command << std::endl;
            bool result = Command::commitAction(client_command, table, ID, resp_output);
            std::cout << "RESULT>>" << result << std::endl;
            if (result) {
                //poslat vsetkym
                write_to_buffer(resp_output);
            } else {
                //posli spat s chybou
                send(client_socket, resp_output, MAX_LEN, 0);
            }
            bzero(resp_output, MAX_LEN);
            bzero(client_command, MAX_LEN);
            if (!table->isResumed()) {
                die = 1;
                // poslat broadcast q
            }
        }
    }
}

void handle_multi_clients(PokerTable* table) {
    while (die == 0) {
        const char* broadcast_msg = read_from_buffer();
        //std::cout << "BROADCAST before check " << broadcast_msg << std::endl;
        if (broadcast_msg != nullptr) {
            std::cout << "BROADCAST after check " << broadcast_msg << std::endl;
            for (int& client_sock : client_sockets) {
                if (client_sock != -1)
                    send(client_sock, broadcast_msg, MAX_LEN, 0);
            }
        }
        delete[] broadcast_msg;
    }
}

/*
 * std::thread th_connection(handle_connection, server_socket, table);
    //std::thread th_send(handle_send_msg, table);
    //std::thread th_recv(handle_receive_msg, table);

    if (th_connection.joinable()) {
        th_connection.join();
    }
//------------------------------------------
//pre kazdeho lcienta vztvor jeden thread
    /*int* playerSockets = table->getSockets();
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        if (playerSockets[i] != -1)
    }*/



/*char* str = new char[MAX_LEN];

if (table->startGame(str)){
    broadcast_message(str, table);
} else {
    std::cout << str << std::endl;
}*/







/*if (th_send.joinable()) {
    th_send.join();
}
if (th_recv.joinable()) {
    th_recv.join();
}*/
