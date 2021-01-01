#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "./headers/server.h"

#define BUFFSIZE 4096
#define SOCKET_ERROR (-1)
#define SERVER_BACKLOG 10

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;



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

    //while (true) {
        printf("Waiting for connections...\n");
        addr_size = sizeof(SA_IN);
        check((client_socket = accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size)),
                "Accept Failed!");
        printf("Connected!\n");

        int *pclient = new int;
        *pclient = client_socket;
        std::thread th_client(handle_connection, pclient);
        th_client.join();

    //}
    close(server_socket);
    return 0;
}

void* handle_connection(void* p_client_socket) {
    int client_socket = *((int*)p_client_socket);
    free(p_client_socket);

    char buffer[BUFFSIZE];
    size_t bytes_read;
    size_t bytes_write;
    int msgsize = 0;

    bytes_read = read(client_socket, buffer, BUFFSIZE-1);
    check(bytes_read, "Read Failed!");

    printf("Client message: %s\n", buffer);

    const char *msg = "I got your message";

    bytes_write = write(client_socket, msg, strlen(msg) + 1);
    if (bytes_write < 0) {
        perror("Error writing to socket");
        return nullptr;
    }

    close(client_socket);
    printf("Closing connection...\n");

    return nullptr;
}

int check(int exp, const char* msg) {
    if (exp == SOCKET_ERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}

