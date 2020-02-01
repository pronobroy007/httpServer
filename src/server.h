#ifndef SERVER
#define SERVER

#define MAX_CONN 20
#define BUFFER_SIZE 1024 

#include "clientManager.h"

class Server : public ClientManager 
{
    public:
        int _listen; 

        Server(char* port);
            bool init();
            bool acceptClient();
            bool recvRequest(int index, char* msg, int len);
            void closeConnection();
        ~Server();

    private:
        char* _port;
        struct addrinfo hints;
        struct addrinfo *bind_addr;
};

#endif
