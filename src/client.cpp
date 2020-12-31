#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "./headers/client.h"

int client(int argc, char* argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 1;
    }

    server = gethostbyname(argv[2]); //vrati IP servera na zaklade DNS
    if (server == nullptr)
    {
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

    //vytvaram spojovaci socket ako na server-side
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 3;
    }

    //pripoj sa cez vytvoreny socket na server, o ktorom udaje su v structe serv_addr
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to socket");
        return 4;
    }
    for(;;) {
        printf("Please enter a message: ");
        bzero(buffer, 256); //vynuluj buffer
        fgets(buffer, 255, stdin); //do buffera ulozi input pouzivatela

        if (std::string(buffer) == "q"){
        if (std::string(buffer) == "q"){
            std::cout<<"Client quit" <<std::endl;
        }
            break;
        }
        n = write(sockfd, buffer, strlen(buffer)); //zapis spravu do socketu o velkosti buffera
        if (n < 0) {
            perror("Error writing to socket");
            return 5;
        }

        bzero(buffer, 256); //vynuluj buffer
        n = read(sockfd, buffer, 255); //citaj spravu zo socketu a uloz ju do buffera
        if (n < 0) {
            perror("Error reading from socket");
            return 6;
        }

        printf("%s\n", buffer);
    }

    close(sockfd); //zavri spojovaci socket

    return 0;
}

