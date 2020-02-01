#include "clientManager.h"


ClientManager::ClientManager()
{ 
    bzero(response, BUFFER);
    projectDir = "/home/linux/Documents/program/app/bin";
}

int ClientManager::get_client(fd_set &reads)
{
    for(int i = 0; i < _client_info_vec.size(); i++)
    {
        if(FD_ISSET(_client_info_vec[i].socket, &reads))
            return i;
    }
    return  -1;
}


bool ClientManager::drop_client(int index)
{
    _client_info_vec.erase(_client_info_vec.begin() + index);
    return false;
}


std::string ClientManager::get_client_address(int index)
{
    std::string ip;
    char buff[100];
    getnameinfo((struct sockaddr*)&_client_info_vec[index].client_addr,
            _client_info_vec[index].size,
            buff, sizeof(buff), 0, 0,
            NI_NUMERICHOST);
    ip = buff;
    return ip;
}


fd_set ClientManager::wait_on_client(int sock)
{
    fd_set master;
    FD_ZERO(&master);
    FD_SET(sock, &master);
    int max_sock = sock;

    for(int i = 0; i < _client_info_vec.size(); i++)
    {
        int s = _client_info_vec[i].socket;
        FD_SET(s, &master);
        if (s > max_sock)
            max_sock = s;
    }

    if (select(max_sock+1, &master, 0, 0, 0) < 0) {
        perror("select() failed");
        exit(1);
    }

    return master;
}



void ClientManager::server_response(int index, char* path)
{
    ssize_t cl = 0;
    char cdir[255];
    bzero(cdir, 255);

    char full_path[255];
    bzero(full_path, 255);

    //*************************************************************************
    //For path ...............................
    if(strcmp(path, "/") == 0)
    {
        path = (char*)"/index.html";
        sprintf(full_path, "%s/public%s", projectDir.c_str(), path);
        getcwd(cdir,255);
    }
    else if(strstr(path, "/linux/"))
    {
        std::string filename(path);
        filename = Parser::parseHexValue(filename);
        sprintf(full_path, "%s", filename.c_str());
    }
    else
        sprintf(full_path, "%s/public%s", projectDir.c_str(), path);

    std::cout << full_path << std::endl;
    //*************************************************************************


    //*************************************************************************
    //Sending index.html.................................................
    if(strstr(full_path, "public/index.html"))
    {
        std::string page = http.indexHtml(full_path, cl, cdir);
        if(page == "false")
            sendErrorResponse(index, http.c400);
        else
        {
            //Sending header.
            http.httpHeader(_client_info_vec[index].socket, page.size(), full_path);
            //Sending index.html
            send(_client_info_vec[index].socket, page.c_str(), page.size(), 0);
        }
        
    }
    //Sending file.................................................
    else
    {
        //Open file to send response to server.
        std::ifstream input_file = http.openFile(full_path, cl);
        if(!input_file.is_open())
            sendErrorResponse(index, http.c400);
        else
        {
            //Sending header.
            http.httpHeader(_client_info_vec[index].socket, cl, full_path);

            //Sending file.
            while(!input_file.eof())
            {
               input_file.read(response, BUFFER);
               //send file data. 
               send(_client_info_vec[index].socket, response, BUFFER, 0);
            } 
            //Closing file.
            input_file.close();
        
        }
    }

    send(_client_info_vec[index].socket, "\r\n\r\n", 4, 0);
    std::cout << "**********************************" << std::endl;
    //*************************************************************************
}


bool ClientManager::sendErrorResponse(int index, std::string errorCode)
{
    send(_client_info_vec[index].socket, errorCode.c_str(), errorCode.size(), 0);
    drop_client(index);
    return false;
}


ClientManager::~ClientManager()
{
}
