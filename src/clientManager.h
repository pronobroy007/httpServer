#ifndef CLIENTMANAGER  
#define CLIENTMANAGER 

#include "http.h"
#include "parser.h"
#include <fstream>
#include <sstream>


#define BUFFER 10024

struct Client_info{
    int socket;
    struct sockaddr_storage client_addr;
    socklen_t size;
};

class ClientManager
{
    public:
        ClientManager();
            //Insert Client_info struct and Return socket.
            int get_client(fd_set &reads);
            //Close an established connection and remove from Client_info struct.
            bool drop_client(int index);
            //Return client IP.
            std::string get_client_address(int index);
            fd_set wait_on_client(int server);
            //Send response to client and drop connection.
            void server_response(int index, char* path);

            //Error msg.
            bool sendErrorResponse(int index, std::string errorCode);
        ~ClientManager();

    protected:
       std::vector<Client_info> _client_info_vec; 
       char response[BUFFER];
       HTTP http;
       std::string projectDir;
};

#endif
