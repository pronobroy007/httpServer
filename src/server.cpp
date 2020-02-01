#include "server.h"

Server::Server(char* port)
{
    _port = port;
    bzero(&hints, sizeof(hints));
}

bool Server::init()
{
	//This is for Broke Pipe exception handler for sending to a client which is close.
	signal(SIGPIPE, SIG_IGN);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    /*
        ai_flags = AI_PASSIVE is set. This is telling getaddrinfo() that we want it to bind to 
        the wildcard address. That is, we are asking getaddrinfo() to set up the 
        address, so we listen on any available network interface.
    */
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(0, _port, &hints, &bind_addr);

    // Creating socket file descriptor 
    if ((_listen = socket(bind_addr->ai_family, bind_addr->ai_socktype, bind_addr->ai_protocol)) == 0) 
    { 
        perror("socket failed"); 
        return false;
    } 
       
    //if (bind(_listen, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) < 0 ) 
    if (bind(_listen, bind_addr->ai_addr, bind_addr->ai_addrlen) < 0 ) 
    { 
        perror("bind failed"); 
        return false;
    } 
    freeaddrinfo(bind_addr);

    if (listen(_listen, MAX_CONN) < 0) 
    { 
        perror("listen"); 
        return false;
    } 

    std::cout << "Wellcome to httpServer ... ... ..." << std::endl; 
    std::cout << "Lisiting port is " << _port << std::endl;
    return true;
}


bool Server::acceptClient()
{
    Client_info c;
    c.size = sizeof(c.client_addr);
    c.socket = accept(_listen,(struct sockaddr*) &c.client_addr, &c.size);
    if (!c.socket) 
    {
        perror("Faield to accept client");
        return false;
    }

    _client_info_vec.push_back(c);
    std::cout << "Connected : " << get_client_address(_client_info_vec.size() - 1) << std::endl << std::endl;
    return true;
}


bool Server::recvRequest(int index, char* msg, int len)
{
    int p = 0;
    char *ptr = (char*)msg;
    while (true)
    {
        int byte_recv = recv(_client_info_vec[index].socket, ptr + p, len - p, 0);
        //std::cout << "Recv byte : " << byte_recv << std::endl;
        if(byte_recv < 1)
            return sendErrorResponse(index, http.c400);

        //To Get Header only...................
        char *q = strstr(msg, "\r\n\r\n");
        if(q)
        {
            std::cout << "Request Header: " << std::endl;
            std::cout << "-----------------" << std::endl;
            std::cout << msg << std::endl;
            std::cout << "****************************************" << std::endl;

            if(strncmp("GET /", msg, 5) == 0)
            {
                //Get path from header.
                char* path = msg + 4;
                char* end = strstr(path, " ");
                if(!end)
                    return sendErrorResponse(index, http.c404); 
                else
                    *end = 0;                

                std::cout << "Path : " << path << std::endl;
                
                
                //Error Check.
                if(strlen(path) > 512)
                    return sendErrorResponse(index, http.c400);
                else if(strstr(path, ".."))
                    return sendErrorResponse(index, http.c404);
                //................................................................
                else
                    server_response(index, path);

            }else
                return sendErrorResponse(index, http.c404);

            break;
        }

        p += byte_recv;
    } 

    return true;
}


void Server::closeConnection()
{
    close(_listen);
}

Server::~Server(){}


