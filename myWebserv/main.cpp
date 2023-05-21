#include "Webserv.hpp"

int main()
{
    Config config_cluster;

    try
    {
        config_cluster.createCluster("default.conf");
        config_cluster.printConfig();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}