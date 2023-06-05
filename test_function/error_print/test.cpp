#include <iostream>
#include <string>
#include <exception>

int main()
{
    try
    {
        int i;
        throw std::runtime_error("test " + i);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}