#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "./headers/server.h"

int server(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t cli_len;

    //struct, ktory jednoznacne popisuje proces z pohladu netu
    struct sockaddr_in serv_addr, cli_addr;

    int n;
    char buffer[256];

    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr)); //Vynuluje obsah structu (v Linuxe)
    serv_addr.sin_family = AF_INET; //Aky typ socketov chcem pouzit (internetove sockety)
    serv_addr.sin_addr.s_addr = INADDR_ANY; //Z ktorych adries chceme prijimat sockety (vsetky adresy)
    serv_addr.sin_port = htons(atoi(argv[2])); //Na akom porte bude pocuvat

    //Systemove volanie socket definuje jeden socket na pouzivanie - return int
    //1.atribut 'AF_INET' => definujeme typ socketu na komunikaciu cez net
    //2.atribut 'SOCK_STREAM' => defunujeme pouzity protokol v tomto pripade TCP
    //3.atribut 0 => dodatocna config, vacsinou vzdy 0
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        //socket vracia < 0 ak sa nepodaril vytvorit
        //napr. kvoli nedostatku prostriedkov
        perror("Error creating socket");
        return 1;
    }

    //bind - spristupnuje socket na kumunikaciu
    //1.atribut 'sockfd' => na tento socket sa moze pripojit hocikaky iny proces z netu z lubovolnej IP
    //2.atribut 'struct' => konfiguracny struct na definovanie spojenia
    //3.atribut 'sizeof(struct)' => definuje aka velka je dana struktura
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }

    //listen() robi zo socketu pasivny socket t.j. pomocou neho sa bude nadvazovat komunikacia
    listen(sockfd, 5); //5 => max 5 clientov sa moze NARAZ v jednom okamihu pripojit
    cli_len = sizeof(cli_addr);

    //accept - vytvori socket pomocou ktoreho bude server komunikovat s clientom
    //1.atribut 'sockfd' => na tento socket sa moze pripojit hocikaky iny proces z netu z lubovolnej IP
    //2.atribut 'struct' => struct s info o clientovi (IP-address, PORT)
    //3.atribut 'sizeof(struct)' => definuje aka velka je dana struktura
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        return 3;
    }

    for (;;) {
        bzero(buffer, 256); //Vynuluj buffer

        //read() => obdoba func recieve()
        //citam spravu zo socketu co poslal client do buffra
        n = read(newsockfd, buffer, 255);
        if (n < 0) {
            perror("Error reading from socket");
            return 4;
        }
        if (std::string(buffer) == "q"){
            std::cout<<"Server quit" <<std::endl;
            break;
        }

        printf("Here is the message: %s\n", buffer);

        const char *msg = "I got your message"; //sprava od servera clientovi

        //do socketu zapisem spravu a dlzku spravy
        //strlen(msg)+1 => kvoli pridanemu characteru '\0', ktory znaci koniec stringu
        n = write(newsockfd, msg, strlen(msg) + 1);
        if (n < 0) {
            perror("Error writing to socket");
            return 5;
        }
    }
    close(newsockfd); //po skonceni komunikacie zavreme komunikacny socket
    close(sockfd);//zavreme spojovaci socket

    return 0;
}