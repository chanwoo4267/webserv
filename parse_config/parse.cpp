#include "parse.hpp"

int main(void)
{
    std::ifstream ifs;

    ifs.open("default.conf");
    if (ifs.fail())
        throw std::runtime_error("ifs open error");
    
    std::string line;
    while (getline(ifs, line))
    {
        // skip if read line is empty
        if (!line.length())
            continue;
        
        
    }
}