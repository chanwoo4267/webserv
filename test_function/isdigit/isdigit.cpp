#include <iostream>
#include <cctype>

int main()
{
    std::cout << std::isdigit('1') << std::endl;
    std::cout << std::isdigit('0') << std::endl;
    std::cout << std::isdigit('-') << std::endl;
    std::cout << std::isdigit('+') << std::endl;
}