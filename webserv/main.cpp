#include "ConfigParser.hpp"

int main(int argc, char *argv[])
{
    try
    {
        Cluster     cluster;
        std::string config_file = "default.conf";

        parseConfigFile(config_file, cluster);
        cluster.printCluster();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}