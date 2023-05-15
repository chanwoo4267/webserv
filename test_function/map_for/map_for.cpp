#include <iostream>
#include <map>
#include <string>

int main()
{
    std::map<int, std::string> m;

    for(int i=100; i<110; i++)
        m[i] = "test";

    for(int i=200; i<210; i++)
        m[i] = "qwer";

    std::map<int, std::string>::iterator it;
    for(it = m.begin(); it != m.end(); it++)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }
}