#include "http.h"

HTTP::HTTP() 
{ c400 = "HTTP/1.1 400 Bad Request\r\n"
        "Connection: close\r\n"
        "Content-Length: 11\r\n\r\nBad Request";

    c404 = "HTTP/1.1 404 Not Found\r\n"
        "Connection: close\r\n"
        "Content-Length: 9\r\n\r\nNot Found";
}


std::string HTTP::indexHtml(const std::string full_path, ssize_t &cl, char* cdir)
{
   std::vector<std::string> files;
   if(!openDir(cdir, files))
       return "false";

    /*
    *   Reading index.html file and also
    *   Copy index.html inside a string.
    */
    std::ifstream input_file = openFile(full_path, cl);
    if(!input_file.is_open())
        return "false";

    std::string page(cl, ' ');
    input_file.read((char*)page.c_str(), cl);
    input_file.close();

    /*
    *  Get Item section from index.html. 
    */
    ssize_t fIndex = page.find("<!--C++-->");
    fIndex += 10;
    ssize_t lIndex = page.find("<!--C++-->", fIndex);
    std::string item(page, fIndex, lIndex-fIndex);
    /*
    * Erase item section form index.html.
    */
    page.erase(fIndex, lIndex-fIndex);

    /*
    *   Insert item's into index.html. 
    */
    for(int i = 0; i < files.size(); i++)
    {
        std::string strItem = addFiles(item, files[i], std::string(cdir));
        page.insert(fIndex, strItem); 
        fIndex += strItem.size();
    }

    return page;
}



std::string HTTP::addFiles(std::string item, std::string file, std::string pwd)
{
    ssize_t index;

    std::string str = "<p></p>";
    index = item.find(str);
    item.insert(index+3, file.c_str(), file.size());

    file = pwd + "/" + file;
    index = item.find("#");
    index = item.find("#", index+1);
    item.erase(index, 1); 
    item.insert(index, file.c_str(), file.size());

    return item;
}



std::ifstream HTTP::openFile(const std::string file_path, ssize_t &cl)
{
    //Open file to send response to server.
    std::ifstream input_file(file_path, std::ios::binary | std::ios::ate);
    if(!input_file.is_open())
    {
        std::cout << file_path << std::endl; 
        perror("Error to open file");
    }
    
    //Content Length..........
    cl = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    return input_file;
}


bool HTTP::openDir(char* cdir, std::vector<std::string> &files)
{
    DIR *dir;
    struct dirent *rd;
    dir = opendir(cdir);
    if(!dir)
    {
        std::cout << cdir << std::endl;
        std::cout << "Faield to open current directory" << std::endl;
        return false; 
    }

    int val = 0;
    while((rd=readdir(dir)))
        if(strcmp(rd->d_name, ".") != 0 && strcmp(rd->d_name, "..") != 0 && rd->d_name[0] != '.')
        {
            files.push_back(rd->d_name);
            std::cout << rd->d_type << std::endl; 
        }

    closedir(dir);
    return true;
}

void HTTP::httpHeader(const int index, const ssize_t cl, const std::string full_path)
{
    //Content Type..............
    std::string ct = get_content_type(full_path.c_str());

    char header[2048];
	bzero(header, 2048);
    sprintf(header, "HTTP/1.1 200 OK\r\n");
    sprintf(header + strlen(header), "Connection: close\r\n");   
    sprintf(header + strlen(header), "Content-Length: %s\r\n", std::to_string(cl).c_str());
    //Content type...............
    sprintf(header + strlen(header), "Content-Type: %s\r\n", ct.c_str());   
    sprintf(header + strlen(header), "\r\n");
    
    //Sending header............................................
    send(index, header, strlen(header), 0);
    
    //*************************************************************************
    std::cout << "Response header : " << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << header << std::endl;
    //*************************************************************************
}


std::string HTTP::get_content_type(const char* path) {
    std::string str;
    const char *last_dot = strrchr(path, '.');
    if (last_dot) {
        if (strcmp(last_dot, ".css") == 0)  str = "text/css";
        else if (strcmp(last_dot, ".csv") == 0)  str = "text/csv";
        else if (strcmp(last_dot, ".gif") == 0)  str = "image/gif";
        else if (strcmp(last_dot, ".htm") == 0)  str = "text/html";
        else if (strcmp(last_dot, ".html") == 0) str = "text/html";
        else if (strcmp(last_dot, ".ico") == 0)  str = "image/x-icon";
        else if (strcmp(last_dot, ".jpeg") == 0) str = "image/jpeg";
        else if (strcmp(last_dot, ".jpg") == 0)  str = "image/jpeg";
        else if (strcmp(last_dot, ".js") == 0)   str = "application/javascript";
        else if (strcmp(last_dot, ".json") == 0) str = "application/json";
        else if (strcmp(last_dot, ".png") == 0)  str = "image/png";
        else if (strcmp(last_dot, ".pdf") == 0)  str = "application/pdf";
        else if (strcmp(last_dot, ".svg") == 0)  str = "image/svg+xml";
        else if (strcmp(last_dot, ".txt") == 0)  str = "text/plain";
        else if (strcmp(last_dot, ".mp3") == 0)  str = "audio/mpeg";
        return str;
    }
    return "application/octet-stream";
}

HTTP::~HTTP() {}

