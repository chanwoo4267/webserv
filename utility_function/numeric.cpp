#include "utility.hpp"

bool isNumber(std::string input)
{
    for(size_t i = 0; i < input.length(); i++)
    {
        if (!std::isdigit(input[i]))
            return false;
    }
    return true;
}

unsigned int ft_stoi(std::string input)
{
    //to be implemented
}

