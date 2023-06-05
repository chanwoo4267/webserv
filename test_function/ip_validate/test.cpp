#include <iostream>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    std::string t_str = argv[1];

    if (inet_addr(t_str.c_str()) == INADDR_NONE)
        std::cout << "INVALID" << std::endl;
    else
        std::cout << "VALID" << std::endl;

    // validation 가능, 그러나 input이 1, 123. 1232564, 1.2.34 등도 전부 VALID하다고 나옴
}