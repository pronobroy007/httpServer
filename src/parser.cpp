#include "parser.h"

Parser::Parser() {}


std::string Parser::parseGetRequest(std::string filename)
{
    return filename;
}

std::string Parser::parseHexValue(std::string filename)
{
    /*********************************************************/
    //  Remove %hex_value to ascii form http get request
    /*********************************************************/
    ssize_t index = 0;
    do
    {
        index = filename.find("%", index);
        if(index == std::string::npos)
            index = 0;
        else
        {
            //Here val have hex number.
            std::string val(filename, index+1, 2);
            std::stringstream s(val);
            int integer;
            s >> std::hex >> integer;
            val = char(integer);

            filename.erase(index, 3);
            filename.insert(index, val);
        }
    }while(index);
    /*********************************************************/

    return filename;

}

Parser::~Parser() {}


