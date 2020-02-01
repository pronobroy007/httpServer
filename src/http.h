#ifndef HTTPCLASS
#define HTTPCLASS

#include "headerFile.h"
#include <fstream>
#include <dirent.h>

class HTTP
{
    public:
        std::string c400, c404;

        HTTP();
            void httpHeader(const int socket, const ssize_t cl, const std::string full_path);
            std::string get_content_type(const char* path);
            std::string indexHtml(const std::string full_path, ssize_t &cl, char* cdir);

            std::ifstream openFile(const std::string file_path, ssize_t &cl);
            bool openDir(char* cdir, std::vector<std::string> &files);
        ~HTTP();
    
    private:
        inline std::string addFiles(std::string item, std::string file, std::string pwd);
};

#endif
