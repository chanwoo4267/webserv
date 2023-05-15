#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() 
{
    std::string path;
    std::cin >> path;

    std::ifstream config(path.c_str());

    std::stringstream ss;
    ss << config.rdbuf();

    std::cout << ss.str() << std::endl;
}