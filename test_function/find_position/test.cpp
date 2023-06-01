#include <iostream>
#include <string>

int main()
{
    std::string str = "127.0.0.1:8002";

    size_t pos = str.find(':');
    std::cout << str.substr(0, pos) << std::endl << str.substr(pos + 1) << std::endl;
}