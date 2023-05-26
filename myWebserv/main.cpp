#include "Webserv.hpp"
#include "ServerManager.hpp"

int main(int argc, char *argv[])
{
    ServerManager manager;

    try
    {
        (manager.getServerConfig()).createCluster(argv[1]);
        (manager.getServerConfig()).printConfig();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}