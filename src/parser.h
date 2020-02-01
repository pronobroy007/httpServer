#ifndef PARSER
#define PARSER

#include <iostream>
#include <string>
#include <sstream>

class Parser
{
    public:
        Parser();
            static std::string parseGetRequest(std::string getRequest);
            static std::string parseHexValue(std::string getRequest);
        ~Parser();
    
    private:
};

#endif
