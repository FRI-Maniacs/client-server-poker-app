#include <iostream>
#include "./headers/server.h"
#include "./headers/client.h"


int main(int argc, char *argv[])
{
    if (std::string(argv[1]) == "server"){
        std::cout<< "SERVER" << std::endl;
        server(argc, argv);
    }
    else if (std::string(argv[1]) == "client"){
        std::cout<< "CLIENTX" << std::endl;
        client(argc, argv);
    } else {
        std::cout << "NOTHING" << std::endl;
    }

    return 0;
}
